#!/usr/bin/env python3
"""
Prepare Conan Center Index recipe files.

Copies the vanillapdf Conan recipe to a conan-center-index checkout and
generates conandata.yml and config.yml for the given version.

Usage:
    python scripts/prepare_cci_recipe.py --version 2.3.0 --sha256 abc123 \\
        --source-dir vanillapdf --cci-dir conan-center-index
"""

import argparse
import os
import shutil

import yaml

CCI_STRIP_BEGIN = "# cci-strip-begin"
CCI_STRIP_END = "# cci-strip-end"


def copy_recipe(src, dst):
    """Copy conanfile.py, dropping the blocks marked as checkout-only.

    Conan Center builds every recipe from the released tarball referenced by
    conandata.yml, so the export_sources() path that local builds use must not
    reach the submitted recipe. Marking the blocks keeps conan/conanfile.py the
    single source of truth instead of maintaining a second copy.
    """
    with open(src) as f:
        lines = f.readlines()

    kept = []
    stripping = False
    stripped_any = False
    for number, line in enumerate(lines, start=1):
        marker = line.strip()
        if marker == CCI_STRIP_BEGIN:
            if stripping:
                raise ValueError(f"{src}:{number}: nested {CCI_STRIP_BEGIN}")
            stripping = True
            stripped_any = True
            continue
        if marker == CCI_STRIP_END:
            if not stripping:
                raise ValueError(f"{src}:{number}: {CCI_STRIP_END} without {CCI_STRIP_BEGIN}")
            stripping = False
            continue
        if not stripping:
            kept.append(line)

    if stripping:
        raise ValueError(f"{src}: unterminated {CCI_STRIP_BEGIN}")

    # A recipe that lost its markers would silently ship the local build path,
    # which is exactly what review rejects - fail loudly instead.
    if not stripped_any:
        raise ValueError(f"{src}: no {CCI_STRIP_BEGIN} block found")

    recipe = "".join(kept)
    if "export_sources" in recipe:
        raise ValueError(f"{src}: export_sources survived the strip")

    with open(dst, "w") as f:
        f.write(recipe)


def load_yaml(path):
    """Load a YAML mapping from path.

    A missing file is expected for a first-time submission and yields an empty
    mapping. A file that is present but malformed (unparseable, or parsing to
    something other than a mapping) is a hard error: silently returning {} would
    drop every previously published version from the regenerated recipe.
    """
    if not os.path.exists(path):
        return {}
    with open(path) as f:
        data = yaml.safe_load(f)
    if data is None:
        return {}
    if not isinstance(data, dict):
        raise ValueError(f"{path} did not parse to a YAML mapping")
    return data


def dump_yaml(path, data):
    with open(path, "w") as f:
        yaml.safe_dump(data, f, sort_keys=False, default_flow_style=False)


def main():
    parser = argparse.ArgumentParser(description="Prepare CCI recipe files")
    parser.add_argument("--version", required=True, help="Release version (e.g. 2.3.0)")
    parser.add_argument("--sha256", required=True, help="SHA256 of the source archive")
    parser.add_argument("--source-dir", required=True, help="Path to vanillapdf repo checkout")
    parser.add_argument("--cci-dir", required=True, help="Path to conan-center-index checkout")
    args = parser.parse_args()

    recipe_dir = os.path.join(args.cci_dir, "recipes", "vanillapdf", "all")
    test_pkg_dir = os.path.join(recipe_dir, "test_package")
    config_path = os.path.join(args.cci_dir, "recipes", "vanillapdf", "config.yml")

    # Create directory structure
    os.makedirs(test_pkg_dir, exist_ok=True)

    # Copy conanfile.py (single source of truth), minus the local build path
    copy_recipe(
        os.path.join(args.source_dir, "conan", "conanfile.py"),
        os.path.join(recipe_dir, "conanfile.py"),
    )

    # Copy test_package
    for name in ("conanfile.py", "CMakeLists.txt", "test_package.c"):
        shutil.copy2(
            os.path.join(args.source_dir, "conan", "test_package", name),
            os.path.join(test_pkg_dir, name),
        )

    # Generate conandata.yml. Merge into any existing file so older releases
    # keep their source entries — CCI expects every published version to remain
    # buildable, and cloning the fork off upstream/master carries the existing
    # recipe once vanillapdf is registered.
    conandata_path = os.path.join(recipe_dir, "conandata.yml")
    conandata = load_yaml(conandata_path)
    conandata.setdefault("sources", {})[args.version] = {
        "url": f"https://github.com/vanillapdf/vanillapdf/archive/refs/tags/v{args.version}.tar.gz",
        "sha256": args.sha256,
    }
    dump_yaml(conandata_path, conandata)

    # Generate config.yml. Merge so previously published versions are preserved.
    config = load_yaml(config_path)
    config.setdefault("versions", {})[args.version] = {"folder": "all"}
    dump_yaml(config_path, config)

    print(f"CCI recipe prepared for vanillapdf/{args.version}")
    print(f"  Recipe dir: {recipe_dir}")
    print(f"  Config: {config_path}")


if __name__ == "__main__":
    main()
