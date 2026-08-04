#!/usr/bin/env python3
"""
Port Update Script

A standalone Python script to update the vcpkg port files (vcpkg.json and portfile.cmake)
with the version and SHA512 hash from the latest release tag.

Usage:
    python scripts/update_port.py [options]

Requirements:
    - git
    - Python 3.6+
    - curl or urllib (for downloading tarball)
"""

import os
import sys
import json
import re
import subprocess
import argparse
import hashlib
import tempfile
import urllib.request
from pathlib import Path
from typing import Optional, Tuple


class PortUpdater:
    def __init__(self, repo_root: Path):
        self.repo_root = repo_root
        self.port_dir = repo_root / "ports" / "vanillapdf"
        self.vcpkg_json_path = self.port_dir / "vcpkg.json"
        self.portfile_path = self.port_dir / "portfile.cmake"

    def run_command(self, cmd: list, cwd: Path = None, check: bool = True) -> subprocess.CompletedProcess:
        """Run a command and return the result."""
        cwd = cwd or self.repo_root
        print(f"Running: {' '.join(cmd)} (in {cwd})")
        # Capture stdout only - stderr is inherited so git's own error messages
        # reach the log instead of being swallowed with the exit status.
        return subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, text=True, check=check)

    def get_latest_release_tag(self) -> Tuple[str, str]:
        """Get the latest release tag and version from the repository."""
        print("Fetching latest tags...")
        self.run_command(['git', 'fetch', '--tags'])

        # Get tags sorted by version
        result = self.run_command([
            'git', 'tag', '--sort=-version:refname', '-l', 'v*'
        ])

        tags = result.stdout.strip().split('\n')
        version_pattern = re.compile(r'^v(\d+\.\d+\.\d+)$')

        for tag in tags:
            match = version_pattern.match(tag.strip())
            if match:
                version = match.group(1)
                print(f"Found latest release tag: {tag} (version: {version})")
                return tag, version

        raise RuntimeError("No valid release tags found (expected format: vX.Y.Z)")

    def get_current_port_version(self) -> str:
        """Get the current version from port vcpkg.json."""
        if not self.vcpkg_json_path.exists():
            raise FileNotFoundError(f"Port vcpkg.json not found at {self.vcpkg_json_path}")

        with open(self.vcpkg_json_path, 'r', encoding='utf-8') as f:
            data = json.load(f)
            return data.get('version', 'unknown')

    def download_and_hash_tarball(self, tag: str) -> str:
        """Download the GitHub release tarball and calculate its SHA512 hash."""
        tarball_url = f"https://github.com/vanillapdf/vanillapdf/archive/refs/tags/{tag}.tar.gz"
        print(f"Downloading tarball from: {tarball_url}")

        with tempfile.NamedTemporaryFile(suffix='.tar.gz', delete=False) as tmp_file:
            tmp_path = tmp_file.name

        try:
            # Download the tarball
            urllib.request.urlretrieve(tarball_url, tmp_path)

            # Calculate SHA512
            sha512 = hashlib.sha512()
            with open(tmp_path, 'rb') as f:
                for chunk in iter(lambda: f.read(8192), b''):
                    sha512.update(chunk)

            hash_value = sha512.hexdigest()
            print(f"Calculated SHA512: {hash_value}")
            return hash_value

        finally:
            # Clean up temporary file
            if os.path.exists(tmp_path):
                os.unlink(tmp_path)

    def update_vcpkg_json(self, version: str) -> None:
        """Update the version in port vcpkg.json."""
        print(f"Updating {self.vcpkg_json_path} with version {version}...")

        with open(self.vcpkg_json_path, 'r', encoding='utf-8') as f:
            data = json.load(f)

        data['version'] = version

        with open(self.vcpkg_json_path, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=4, ensure_ascii=False)
            f.write('\n')

        self.run_command(['git', 'add', str(self.vcpkg_json_path)])
        print(f"Updated vcpkg.json to version {version}")

    def update_portfile_sha512(self, sha512: str) -> None:
        """Update the SHA512 hash in portfile.cmake."""
        print(f"Updating {self.portfile_path} with SHA512...")

        with open(self.portfile_path, 'r', encoding='utf-8') as f:
            content = f.read()

        # Replace SHA512 hash (128 hex characters)
        new_content = re.sub(
            r'SHA512\s+[a-fA-F0-9]{128}',
            f'SHA512 {sha512}',
            content
        )

        if new_content == content:
            print("Warning: SHA512 pattern not found in portfile.cmake")
            return

        with open(self.portfile_path, 'w', encoding='utf-8') as f:
            f.write(new_content)

        self.run_command(['git', 'add', str(self.portfile_path)])
        print(f"Updated portfile.cmake SHA512")

    def create_update_branch(self, version: str) -> str:
        """Create a new branch for the port update."""
        # The release/ prefix is reserved for real release branches, which are
        # protected and reject direct pushes - see automated/update-vcpkg-*.
        branch_name = f"automated/update-port-{version}"
        print(f"Creating branch: {branch_name}")

        # Check if we're on a clean state
        result = self.run_command(['git', 'status', '--porcelain'], check=False)
        if result.stdout.strip():
            print("Warning: Working directory has uncommitted changes")

        self.run_command(['git', 'checkout', '-b', branch_name])
        return branch_name

    def commit_changes(self, version: str, sha512: str) -> None:
        """Commit the port update changes."""
        commit_message = f"""chore: Update vcpkg port to version {version}

- Updated ports/vanillapdf/vcpkg.json version to {version}
- Updated ports/vanillapdf/portfile.cmake SHA512 hash

SHA512: {sha512}

🤖 Generated with update_port.py script"""

        print("Committing changes...")
        self.run_command(['git', 'commit', '-s', '-m', commit_message])

    def push_branch(self, branch_name: str) -> None:
        """Push the update branch to remote."""
        print(f"Pushing branch {branch_name} to remote...")
        self.run_command(['git', 'push', '-u', 'origin', branch_name])


def main():
    parser = argparse.ArgumentParser(
        description="Update vcpkg port files with latest release version and SHA512"
    )
    parser.add_argument('--dry-run', action='store_true',
                       help='Show what would be done without making changes')
    parser.add_argument('--no-push', action='store_true',
                       help='Skip pushing the branch to remote')
    parser.add_argument('--no-branch', action='store_true',
                       help='Make changes on current branch instead of creating new one')
    parser.add_argument('--repo-root', type=Path, default=None,
                       help='Path to repository root (default: auto-detect)')

    args = parser.parse_args()

    # Auto-detect repository root
    if args.repo_root:
        repo_root = args.repo_root.resolve()
    else:
        # Try to find repo root from script location
        script_dir = Path(__file__).resolve().parent
        repo_root = script_dir.parent

    # Validate repository structure
    if not (repo_root / '.git').exists():
        print(f"Error: {repo_root} is not a git repository")
        sys.exit(1)

    if not (repo_root / 'ports' / 'vanillapdf').exists():
        print(f"Error: ports/vanillapdf directory not found at {repo_root}")
        sys.exit(1)

    try:
        updater = PortUpdater(repo_root)

        # Get latest release tag
        tag, version = updater.get_latest_release_tag()
        current_version = updater.get_current_port_version()

        print(f"\nCurrent port version: {current_version}")
        print(f"Latest release version: {version}")

        if current_version == version:
            print("\nPort is already up to date!")
            return

        if args.dry_run:
            print(f"\nDRY RUN: Would update port from {current_version} to {version}")
            print(f"DRY RUN: Would download tarball for {tag} and calculate SHA512")
            if not args.no_branch:
                print(f"DRY RUN: Would create branch 'automated/update-port-{version}'")
            if not args.no_push:
                print("DRY RUN: Would push branch to remote")
            return

        # Download tarball and calculate SHA512
        sha512 = updater.download_and_hash_tarball(tag)

        # Create branch if requested
        branch_name = None
        if not args.no_branch:
            branch_name = updater.create_update_branch(version)

        # Update files
        updater.update_vcpkg_json(version)
        updater.update_portfile_sha512(sha512)

        # Commit changes
        updater.commit_changes(version, sha512)

        # Push if requested
        if not args.no_push and branch_name:
            updater.push_branch(branch_name)

        print(f"\n✅ Successfully updated port from {current_version} to {version}")
        if branch_name:
            print(f"📂 Changes committed to branch: {branch_name}")
        print(f"🔑 SHA512: {sha512}")

        if not args.no_push and branch_name:
            print(f"\n📝 Create a pull request from '{branch_name}' to 'main'")

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == '__main__':
    main()
