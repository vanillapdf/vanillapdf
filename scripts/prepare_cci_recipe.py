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


def main():
    parser = argparse.ArgumentParser(description="Prepare CCI recipe files")
    parser.add_argument("--version", required=True, help="Release version (e.g. 2.3.0)")
    parser.add_argument("--sha256", required=True, help="SHA256 of the source archive")
    parser.add_argument("--source-dir", required=True, help="Path to vanillapdf repo checkout")
    parser.add_argument("--cci-dir", required=True, help="Path to conan-center-index checkout")
    args = parser.parse_args()

    recipe_dir = os.path.join(args.cci_dir, "recipes", "vanillapdf", "all")
    test_pkg_dir = os.path.join(recipe_dir, "test_package", "src")
    config_path = os.path.join(args.cci_dir, "recipes", "vanillapdf", "config.yml")

    # Create directory structure
    os.makedirs(test_pkg_dir, exist_ok=True)

    # Copy conanfile.py (single source of truth)
    shutil.copy2(
        os.path.join(args.source_dir, "conan", "conanfile.py"),
        os.path.join(recipe_dir, "conanfile.py"),
    )

    # Copy test_package
    for name in ("conanfile.py", "CMakeLists.txt"):
        shutil.copy2(
            os.path.join(args.source_dir, "conan", "test_package", name),
            os.path.join(recipe_dir, "test_package", name),
        )
    src_dir = os.path.join(args.source_dir, "conan", "test_package", "src")
    for entry in os.listdir(src_dir):
        shutil.copy2(os.path.join(src_dir, entry), test_pkg_dir)

    # Generate conandata.yml
    with open(os.path.join(recipe_dir, "conandata.yml"), "w") as f:
        f.write(f'sources:\n')
        f.write(f'  "{args.version}":\n')
        f.write(f'    url: "https://github.com/vanillapdf/vanillapdf/archive/refs/tags/v{args.version}.tar.gz"\n')
        f.write(f'    sha256: "{args.sha256}"\n')

    # Generate config.yml
    with open(config_path, "w") as f:
        f.write(f'versions:\n')
        f.write(f'  "{args.version}":\n')
        f.write(f'    folder: all\n')

    print(f"CCI recipe prepared for vanillapdf/{args.version}")
    print(f"  Recipe dir: {recipe_dir}")
    print(f"  Config: {config_path}")


if __name__ == "__main__":
    main()
