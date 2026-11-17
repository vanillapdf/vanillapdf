#!/usr/bin/env python3
"""
Verify the DEB or RPM packages produced by CPack.

The release ships exactly two Linux packages per format: the runtime package
(libvanillapdf) and the development package (libvanillapdf-dev on Debian,
libvanillapdf-devel on RPM). The runtime package must carry LICENSE.txt and
NOTICE.md, which Apache-2.0 section 4(d) requires to be redistributed with
the library. Install components that are not assigned to a component group
turn into extra packages that nothing depends on (#620); this script catches
that before the packages are published.

Usage:
    python3 scripts/verify_linux_packages.py --format deb --directory build/linux-x64-gcc
    python3 scripts/verify_linux_packages.py --format rpm --directory build

Requirements:
    - Python 3.6+
    - dpkg-deb (DEB) or rpm (RPM) on PATH
"""

import argparse
import subprocess
import sys
from pathlib import Path
from typing import List


# Package name prefixes, matched against the file names CPack produces with
# DEB-DEFAULT / RPM-DEFAULT: libvanillapdf_<version>_<arch>.deb,
# libvanillapdf-<version>-<release>.<arch>.rpm
EXPECTED_PACKAGES = {
    "deb": {"runtime": "libvanillapdf_", "development": "libvanillapdf-dev_"},
    "rpm": {"runtime": "libvanillapdf-", "development": "libvanillapdf-devel-"},
}

REQUIRED_RUNTIME_FILES = [
    "/usr/share/vanillapdf/LICENSE.txt",
    "/usr/share/vanillapdf/NOTICE.md",
]


def list_package_files(package_format: str, package: Path) -> List[str]:
    """Return the absolute paths of the files in a package."""
    if package_format == "deb":
        command = ["dpkg-deb", "--fsys-tarfile", str(package)]
        tar_stream = subprocess.run(command, check=True, stdout=subprocess.PIPE).stdout
        listing = subprocess.run(["tar", "-t"], input=tar_stream, check=True, stdout=subprocess.PIPE).stdout
        # tar lists the members as ./usr/..., drop the leading dot
        return [line[1:] for line in listing.decode().splitlines() if line.startswith("./")]

    command = ["rpm", "-qlp", str(package)]
    listing = subprocess.run(command, check=True, stdout=subprocess.PIPE).stdout
    return listing.decode().splitlines()


def classify(package_format: str, package: Path) -> str:
    """Return 'runtime' or 'development' for a package file, or '' if unexpected."""
    prefixes = EXPECTED_PACKAGES[package_format]

    # The development prefix also starts with the runtime one, so test it first
    if package.name.startswith(prefixes["development"]):
        return "development"

    runtime_prefix = prefixes["runtime"]
    if package.name.startswith(runtime_prefix):
        version_start = package.name[len(runtime_prefix):len(runtime_prefix) + 1]
        if version_start.isdigit():
            return "runtime"

    return ""


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0].strip())
    parser.add_argument("--format", required=True, choices=sorted(EXPECTED_PACKAGES), dest="package_format")
    parser.add_argument("--directory", required=True, type=Path, help="Directory holding the packages")
    arguments = parser.parse_args()

    package_format = arguments.package_format
    packages = sorted(arguments.directory.glob("*." + package_format))
    errors = []

    print(f"Packages in {arguments.directory}:")
    for package in packages:
        print(f"  {package.name}")

    found = {}
    for package in packages:
        kind = classify(package_format, package)
        if not kind:
            errors.append(f"Unexpected package {package.name}")
            continue
        if kind in found:
            errors.append(f"More than one {kind} package: {found[kind].name}, {package.name}")
            continue
        found[kind] = package

    for kind in EXPECTED_PACKAGES[package_format]:
        if kind not in found:
            errors.append(f"No {kind} package found")

    if "runtime" in found:
        runtime = found["runtime"]
        contents = set(list_package_files(package_format, runtime))
        for required in REQUIRED_RUNTIME_FILES:
            if required not in contents:
                errors.append(f"{runtime.name} does not contain {required}")

    for error in errors:
        # GitHub Actions annotation; plain text everywhere else
        print(f"::error::{error}")

    if errors:
        return 1

    print("Package set and runtime package contents are as expected")
    return 0


if __name__ == "__main__":
    sys.exit(main())
