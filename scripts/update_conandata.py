#!/usr/bin/env python3
"""
Conan Data Update Script

Updates conan/conandata.yml with a new version entry by downloading the
release archive and calculating its SHA256 hash.

conan/conandata.yml is gitignored, not committed: `conan create conan/`
always builds via export_sources() from the local checkout, so this file is
never read in that flow. It exists only to locally exercise the
download-based source() path (the one Conan Center Index actually uses, via
its own freshly-generated conandata.yml - see create-conan-pr.yml). Run this
script to regenerate it whenever you need that.

Usage:
    python scripts/update_conandata.py                    # Auto-detect latest tag
    python scripts/update_conandata.py --version 2.3.0    # Specific version
    python scripts/update_conandata.py --dry-run           # Show what would change

Requirements:
    - Python 3.6+
    - PyYAML (pip install pyyaml) or falls back to manual YAML handling
"""

import os
import sys
import re
import argparse
import hashlib
import tempfile
import subprocess
import urllib.request
from pathlib import Path
from typing import Optional, Tuple


def run_command(cmd: list, cwd: Path = None, check: bool = True) -> subprocess.CompletedProcess:
    """Run a command and return the result."""
    cwd = cwd or Path.cwd()
    print(f"Running: {' '.join(cmd)} (in {cwd})")
    return subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, check=check)


def get_latest_release_tag(repo_root: Path) -> Tuple[str, str]:
    """Get the latest release tag and version from the repository."""
    print("Fetching latest tags...")
    run_command(['git', 'fetch', '--tags'], cwd=repo_root)

    result = run_command([
        'git', 'tag', '--sort=-version:refname', '-l', 'v*'
    ], cwd=repo_root)

    tags = result.stdout.strip().split('\n')
    version_pattern = re.compile(r'^v(\d+\.\d+\.\d+)$')

    for tag in tags:
        match = version_pattern.match(tag.strip())
        if match:
            version = match.group(1)
            print(f"Found latest release tag: {tag} (version: {version})")
            return tag, version

    raise RuntimeError("No valid release tags found (expected format: vX.Y.Z)")


def download_and_hash(version: str) -> str:
    """Download the GitHub release archive and calculate its SHA256 hash."""
    tarball_url = f"https://github.com/vanillapdf/vanillapdf/archive/refs/tags/v{version}.tar.gz"
    print(f"Downloading archive from: {tarball_url}")

    with tempfile.NamedTemporaryFile(suffix='.tar.gz', delete=False) as tmp_file:
        tmp_path = tmp_file.name

    try:
        urllib.request.urlretrieve(tarball_url, tmp_path)

        sha256 = hashlib.sha256()
        with open(tmp_path, 'rb') as f:
            for chunk in iter(lambda: f.read(8192), b''):
                sha256.update(chunk)

        hash_value = sha256.hexdigest()
        print(f"Calculated SHA256: {hash_value}")
        return hash_value

    finally:
        if os.path.exists(tmp_path):
            os.unlink(tmp_path)


def read_conandata(conandata_path: Path) -> str:
    """Read the conandata.yml file."""
    if conandata_path.exists():
        with open(conandata_path, 'r', encoding='utf-8') as f:
            return f.read()
    return "sources:\n"


def update_conandata(conandata_path: Path, version: str, sha256: str) -> str:
    """Update conandata.yml with a new version entry.

    Uses simple string manipulation to avoid PyYAML dependency.
    """
    content = read_conandata(conandata_path)
    url = f"https://github.com/vanillapdf/vanillapdf/archive/refs/tags/v{version}.tar.gz"

    new_entry = f'  "{version}":\n    url: "{url}"\n    sha256: "{sha256}"\n'

    # Check if version already exists
    if f'"{version}":' in content:
        # Replace existing entry - match from version key to next version or end
        pattern = rf'  "{re.escape(version)}":\n    url: "[^"]*"\n    sha256: "[^"]*"\n'
        content = re.sub(pattern, new_entry, content)
        print(f"Updated existing entry for version {version}")
    else:
        # Append new entry after "sources:" line
        content = content.rstrip('\n') + '\n' + new_entry
        print(f"Added new entry for version {version}")

    return content


def main():
    parser = argparse.ArgumentParser(
        description="Update conan/conandata.yml with version SHA256 hash"
    )
    parser.add_argument('--version', type=str, default=None,
                       help='Version to add (e.g., 2.3.0). Default: auto-detect from latest tag')
    parser.add_argument('--sha256', type=str, default=None,
                       help='SHA256 hash (skip download if provided)')
    parser.add_argument('--dry-run', action='store_true',
                       help='Show what would be done without making changes')
    parser.add_argument('--repo-root', type=Path, default=None,
                       help='Path to repository root (default: auto-detect)')

    args = parser.parse_args()

    # Auto-detect repository root
    if args.repo_root:
        repo_root = args.repo_root.resolve()
    else:
        script_dir = Path(__file__).resolve().parent
        repo_root = script_dir.parent

    conandata_path = repo_root / "conan" / "conandata.yml"

    # Determine version
    if args.version:
        version = args.version
    else:
        _, version = get_latest_release_tag(repo_root)

    print(f"\nVersion: {version}")
    print(f"Conandata path: {conandata_path}")

    # Determine SHA256
    if args.sha256:
        sha256 = args.sha256
        print(f"Using provided SHA256: {sha256}")
    elif args.dry_run:
        sha256 = "0" * 64
        print("DRY RUN: Using placeholder SHA256")
    else:
        sha256 = download_and_hash(version)

    # Generate updated content
    new_content = update_conandata(conandata_path, version, sha256)

    if args.dry_run:
        print(f"\nDRY RUN: Would write to {conandata_path}:")
        print("---")
        print(new_content)
        print("---")
        return

    # Write updated file
    conandata_path.parent.mkdir(parents=True, exist_ok=True)
    with open(conandata_path, 'w', encoding='utf-8') as f:
        f.write(new_content)

    print(f"\nUpdated {conandata_path}")
    print(f"Version: {version}")
    print(f"SHA256: {sha256}")


if __name__ == '__main__':
    main()
