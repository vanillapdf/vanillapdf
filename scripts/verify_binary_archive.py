#!/usr/bin/env python3
"""
Verify a binary archive (ZIP or TGZ) produced by CPack for a GitHub release.

The release attaches one archive per platform: a ZIP on Windows and a
.tar.gz on Linux and macOS. Each archive holds the shared library, the
public headers, the CMake config package and the license files, under a
single top-level directory named after the archive.

The script checks that exactly one archive exists in the directory, that all
of its files are under the top-level directory, and that the required files
are at their expected paths. Then it consumes the archive the way a user
would: it extracts the archive, builds examples/system-package-integration
against it with find_package, and runs the resulting test.

Usage:
    python3 scripts/verify_binary_archive.py --directory build/linux-x64-gcc --platform linux --build-type Release
    python scripts/verify_binary_archive.py --directory build/windows-x86-msvc-18-static --platform windows --build-type Release --cmake-arg=-A --cmake-arg=Win32

Requirements:
    - Python 3 with tarfile extraction filters (3.12, or a security release
      that backports them)
    - CMake and a C++ compiler (for the consumer build)
"""

import argparse
import subprocess
import sys
import tarfile
import tempfile
import zipfile
from pathlib import Path, PurePosixPath
from typing import Callable, List, NamedTuple, Optional, Set


# Files every archive must hold, relative to its top-level directory. Each
# entry lists the accepted locations: the library directory is lib or lib64
# depending on the distribution (CMAKE_INSTALL_LIBDIR), nothing else varies.
REQUIRED_FILES = [
    ["include/vanillapdf/c_vanillapdf_api.h"],
    ["include/vanillapdf/c_vanillapdf_config.h"],
    ["lib/cmake/vanillapdf/vanillapdfConfig.cmake", "lib64/cmake/vanillapdf/vanillapdfConfig.cmake"],
    ["share/vanillapdf/LICENSE.txt"],
    ["share/vanillapdf/NOTICE.md"],
]

# The shared library, and on Windows also the import library that consumers
# link against; the same format as REQUIRED_FILES
PLATFORM_FILES = {
    "windows": [
        ["bin/libvanillapdf.dll"],
        ["lib/libvanillapdf.lib"],
    ],
    "linux": [
        ["lib/libvanillapdf.so", "lib64/libvanillapdf.so"],
    ],
    "macos": [
        ["lib/libvanillapdf.dylib"],
    ],
}

REPOSITORY_ROOT = Path(__file__).resolve().parent.parent
CONSUMER_SOURCE = REPOSITORY_ROOT / "examples" / "system-package-integration"


def list_zip_members(archive: Path) -> List[PurePosixPath]:
    """Return the paths of the files in a ZIP archive."""
    members = []
    with zipfile.ZipFile(archive) as opened:
        for info in opened.infolist():
            if info.is_dir():
                continue
            members.append(PurePosixPath(info.filename))

    return members


def list_tar_members(archive: Path) -> List[PurePosixPath]:
    """Return the paths of the files and links in a .tar.gz archive."""
    members = []
    with tarfile.open(archive) as opened:
        for member in opened.getmembers():
            if member.isdir():
                continue
            # PurePosixPath drops the leading './' that tar member names may carry
            members.append(PurePosixPath(member.name))

    return members


def extract_zip(archive: Path, destination: Path) -> None:
    """Extract a ZIP archive into the destination directory."""
    # zipfile itself drops absolute paths and '..' from member names
    with zipfile.ZipFile(archive) as opened:
        opened.extractall(destination)


def extract_tar(archive: Path, destination: Path) -> None:
    """Extract a .tar.gz archive into the destination directory."""
    # The data filter rejects absolute paths and links that leave the destination
    with tarfile.open(archive) as opened:
        opened.extractall(destination, filter="data")


class ArchiveFormat(NamedTuple):
    suffix: str
    list_members: Callable[[Path], List[PurePosixPath]]
    extract: Callable[[Path, Path], None]


ZIP_FORMAT = ArchiveFormat(".zip", list_zip_members, extract_zip)
TAR_FORMAT = ArchiveFormat(".tar.gz", list_tar_members, extract_tar)

ARCHIVE_FORMAT = {
    "windows": ZIP_FORMAT,
    "linux": TAR_FORMAT,
    "macos": TAR_FORMAT,
}


def find_archive(directory: Path, suffix: str) -> Optional[Path]:
    """Return the only archive with the suffix in the directory, or None."""
    archives = []
    for path in sorted(directory.iterdir()):
        if path.name.endswith(suffix):
            archives.append(path)

    if len(archives) == 1:
        return archives[0]

    names = ", ".join(path.name for path in archives) or "none"
    print(f"::error::Expected exactly one {suffix} archive in {directory}, found: {names}")
    return None


def find_files_outside(members: List[PurePosixPath], top_level: str) -> List[PurePosixPath]:
    """Return the members that are not under the top-level directory."""
    outside = []
    for member in members:
        if member.parts[0] != top_level:
            outside.append(member)

    return outside


def strip_top_level(members: List[PurePosixPath]) -> Set[PurePosixPath]:
    """Return the member paths relative to the top-level directory."""
    relative_paths = set()
    for member in members:
        relative_parts = member.parts[1:]
        relative_paths.add(PurePosixPath(*relative_parts))

    return relative_paths


def find_missing_files(relative_paths: Set[PurePosixPath], required_files: List[List[str]]) -> List[str]:
    """Return the required files found at none of their accepted locations."""
    missing = []
    for locations in required_files:
        found = False
        for location in locations:
            if PurePosixPath(location) in relative_paths:
                found = True
                break

        if not found:
            missing.append(" or ".join(locations))

    return missing


def run(command: List[str]) -> None:
    print("+ " + " ".join(command), flush=True)
    subprocess.run(command, check=True)


def consume(prefix: Path, build_type: str, cmake_arguments: List[str], work_directory: Path) -> None:
    """Build and run the consumer example against the extracted archive."""
    build_directory = work_directory / "consumer-build"

    configure = [
        "cmake",
        "-S", str(CONSUMER_SOURCE),
        "-B", str(build_directory),
        "-DCMAKE_BUILD_TYPE=" + build_type,
        "-DCMAKE_PREFIX_PATH=" + str(prefix),
    ]
    configure.extend(cmake_arguments)

    run(configure)
    run(["cmake", "--build", str(build_directory), "--config", build_type])
    run(["ctest", "--test-dir", str(build_directory), "-C", build_type, "--output-on-failure"])


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--directory", required=True, type=Path, help="Directory holding the archive")
    parser.add_argument("--platform", required=True, choices=sorted(ARCHIVE_FORMAT))
    parser.add_argument("--build-type", required=True,
                        help="Configuration the archive was built in; the consumer is built the same way")
    parser.add_argument("--cmake-arg", action="append", default=[], dest="cmake_arguments",
                        help="Extra argument for configuring the consumer project (repeatable)")
    arguments = parser.parse_args()

    platform = arguments.platform
    archive_format = ARCHIVE_FORMAT[platform]

    archive = find_archive(arguments.directory, archive_format.suffix)
    if archive is None:
        return 1

    members = archive_format.list_members(archive)
    print(f"Contents of {archive.name}:")
    for member in members:
        print(f"  {member}")

    # CPack puts every file under one directory named after the archive
    top_level = archive.name[:-len(archive_format.suffix)]
    outside = find_files_outside(members, top_level)
    for member in outside:
        print(f"::error::{member} is outside the top-level directory {top_level}")

    if outside:
        return 1

    relative_paths = strip_top_level(members)
    required_files = REQUIRED_FILES + PLATFORM_FILES[platform]
    missing = find_missing_files(relative_paths, required_files)
    for file in missing:
        print(f"::error::{archive.name} does not contain {file}")

    if missing:
        return 1

    with tempfile.TemporaryDirectory(prefix="vanillapdf-archive-") as temporary:
        work_directory = Path(temporary)
        extracted = work_directory / "extracted"
        archive_format.extract(archive, extracted)

        try:
            consume(extracted / top_level, arguments.build_type, arguments.cmake_arguments, work_directory)
        except subprocess.CalledProcessError as error:
            print(f"::error::Consuming {archive.name} failed: {error}")
            return 1

    print(f"{archive.name} has the expected contents and builds a consumer project")
    return 0


if __name__ == "__main__":
    sys.exit(main())
