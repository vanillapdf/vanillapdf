#!/usr/bin/env python3
"""
Vcpkg Update Script

A standalone Python script to update vcpkg to the latest version and optionally create a pull request.
This script can be run manually whenever you want to check for and apply vcpkg updates.

Usage:
    python scripts/update_vcpkg.py [options]

Requirements:
    - git
    - gh CLI (for PR creation)
    - Python 3.6+
"""

import os
import sys
import json
import re
import subprocess
import argparse
from pathlib import Path
from typing import Optional, Tuple, Dict, Any


class VcpkgUpdater:
    def __init__(self, repo_root: Path, vcpkg_root: Path = None):
        self.repo_root = repo_root
        self.vcpkg_root = vcpkg_root or repo_root / "external" / "vcpkg"
        self.vcpkg_json_path = repo_root / "vcpkg.json.in"

    def run_command(self, cmd: list, cwd: Path = None, check: bool = True) -> subprocess.CompletedProcess:
        """Run a command and return the result."""
        cwd = cwd or self.repo_root
        print(f"Running: {' '.join(cmd)} (in {cwd})")
        return subprocess.run(cmd, cwd=cwd, capture_output=True, text=True, check=check)

    def get_current_vcpkg_version(self) -> str:
        """Get the current vcpkg version from the submodule."""
        try:
            result = self.run_command(['git', 'describe', '--tags'], cwd=self.vcpkg_root, check=False)
            if result.returncode == 0:
                return result.stdout.strip()
            else:
                return "unknown"
        except Exception as e:
            print(f"Warning: Could not get current vcpkg version: {e}")
            return "unknown"

    def get_latest_vcpkg_version(self) -> Tuple[str, str]:
        """Get the latest vcpkg version and commit hash."""
        print("Fetching latest vcpkg tags...")
        self.run_command(['git', 'fetch', '--tags'], cwd=self.vcpkg_root)

        # Get latest version tag
        result = self.run_command([
            'git', 'tag', '--sort=-version:refname'
        ], cwd=self.vcpkg_root)

        tags = result.stdout.strip().split('\n')
        version_pattern = re.compile(r'^\d{4}\.\d{2}\.\d{2}$')
        latest_tag = None

        for tag in tags:
            if version_pattern.match(tag):
                latest_tag = tag
                break

        if not latest_tag:
            raise RuntimeError("No valid version tags found in vcpkg repository")

        # Get commit hash for the tag
        result = self.run_command([
            'git', 'rev-list', '-n', '1', latest_tag
        ], cwd=self.vcpkg_root)

        latest_commit = result.stdout.strip()

        return latest_tag, latest_commit

    def check_update_needed(self, force: bool = False) -> Tuple[bool, str, str]:
        """Check if vcpkg update is needed."""
        current_version = self.get_current_vcpkg_version()
        latest_version, latest_commit = self.get_latest_vcpkg_version()

        print(f"Current vcpkg version: {current_version}")
        print(f"Latest vcpkg version: {latest_version}")

        if current_version != latest_version or force:
            return True, latest_version, latest_commit
        else:
            print("No update needed - already on latest version")
            return False, latest_version, latest_commit

    def update_vcpkg_submodule(self, version: str) -> None:
        """Update the vcpkg submodule to the specified version."""
        print(f"Updating vcpkg submodule to {version}...")

        # Checkout the specific tag
        self.run_command(['git', 'checkout', version], cwd=self.vcpkg_root)

        # Stage the submodule change
        self.run_command(['git', 'add', 'external/vcpkg'])

    def update_vcpkg_json_baseline(self, commit_hash: str) -> None:
        """Update the builtin-baseline in vcpkg.json.in."""
        print(f"Updating vcpkg.json.in baseline to {commit_hash}...")

        if not self.vcpkg_json_path.exists():
            raise FileNotFoundError(f"vcpkg.json.in not found at {self.vcpkg_json_path}")

        # Read current vcpkg.json.in
        with open(self.vcpkg_json_path, 'r', encoding='utf-8') as f:
            vcpkg_config = json.load(f)

        # Update builtin-baseline
        vcpkg_config['builtin-baseline'] = commit_hash

        # Write back to file
        with open(self.vcpkg_json_path, 'w', encoding='utf-8') as f:
            json.dump(vcpkg_config, f, indent=2, ensure_ascii=False)
            f.write('\n')  # Add trailing newline

        # Stage the change
        self.run_command(['git', 'add', 'vcpkg.json.in'])

        print(f"Successfully updated vcpkg.json.in baseline to {commit_hash}")

    def create_update_branch(self, version: str) -> str:
        """Create a new branch for the vcpkg update."""
        branch_name = f"automated/update-vcpkg-{version}"
        print(f"Creating branch: {branch_name}")

        self.run_command(['git', 'checkout', '-b', branch_name])
        return branch_name

    def commit_changes(self, current_version: str, new_version: str) -> None:
        """Commit the vcpkg update changes."""
        commit_message = f"""Update vcpkg to latest tag {new_version}

- Updated vcpkg submodule from {current_version} to {new_version}
- Updated builtin-baseline in vcpkg.json.in to match new commit hash
- Manual update to ensure latest dependency versions

🤖 Generated with update_vcpkg.py script"""

        print(f"Committing changes...")
        self.run_command(['git', 'commit', '-s', '-m', commit_message])

    def push_branch(self, branch_name: str) -> None:
        """Push the update branch to remote."""
        print(f"Pushing branch {branch_name} to remote...")
        self.run_command(['git', 'push', 'origin', branch_name])

    def create_pull_request(self, current_version: str, new_version: str, commit_hash: str, branch_name: str) -> Optional[str]:
        """Create a pull request for the vcpkg update."""
        title = f"🤖 Update vcpkg to latest tag {new_version}"

        body = f"""## 🔄 Manual vcpkg Update

This is a manual update of the vcpkg dependency manager using the update_vcpkg.py script.

### Changes
- **vcpkg version**: `{current_version}` → `{new_version}`
- **Submodule commit**: Updated to `{commit_hash}`
- **vcpkg.json.in baseline**: Updated to match new commit hash

### What's included
- Latest dependency versions and security patches
- Bug fixes and performance improvements in vcpkg itself
- Updated package definitions for better compatibility

### Testing checklist
- [ ] All existing builds pass with new vcpkg version
- [ ] Unit tests pass without regressions
- [ ] Integration tests complete successfully
- [ ] No breaking changes in dependency APIs

### Notes
This PR was created using the `scripts/update_vcpkg.py` script.

---
🤖 Generated with update_vcpkg.py script"""

        try:
            print("Creating pull request...")
            result = self.run_command([
                'gh', 'pr', 'create',
                '--title', title,
                '--body', body,
                '--base', 'main',
                '--head', branch_name
            ])

            # Extract PR URL from output
            pr_url = result.stdout.strip()
            print(f"✅ Successfully created pull request: {pr_url}")
            return pr_url

        except subprocess.CalledProcessError as e:
            print(f"⚠️ Could not create pull request automatically: {e}")
            print(f"📝 Branch '{branch_name}' has been pushed with the vcpkg update")
            print(f"👤 Please manually create a pull request from this branch to main")
            return None


def main():
    parser = argparse.ArgumentParser(description="Update vcpkg to the latest version")
    parser.add_argument('--force', action='store_true',
                       help='Force update even if already on latest version')
    parser.add_argument('--no-pr', action='store_true',
                       help='Skip pull request creation')
    parser.add_argument('--dry-run', action='store_true',
                       help='Show what would be done without making changes')
    parser.add_argument('--repo-root', type=Path, default=Path.cwd(),
                       help='Path to repository root (default: current directory)')

    args = parser.parse_args()

    # Validate repository structure
    repo_root = args.repo_root.resolve()
    if not (repo_root / '.git').exists():
        print(f"Error: {repo_root} is not a git repository")
        sys.exit(1)

    if not (repo_root / 'external' / 'vcpkg').exists():
        print(f"Error: vcpkg submodule not found at {repo_root}/external/vcpkg")
        sys.exit(1)

    if not (repo_root / 'vcpkg.json.in').exists():
        print(f"Error: vcpkg.json.in not found at {repo_root}/vcpkg.json.in")
        sys.exit(1)

    try:
        updater = VcpkgUpdater(repo_root)

        # Check if update is needed
        update_needed, latest_version, latest_commit = updater.check_update_needed(args.force)
        current_version = updater.get_current_vcpkg_version()

        if not update_needed:
            print("No update needed.")
            return

        if args.dry_run:
            print(f"DRY RUN: Would update vcpkg from {current_version} to {latest_version}")
            print(f"DRY RUN: Would update baseline to {latest_commit}")
            print(f"DRY RUN: Would create branch 'automated/update-vcpkg-{latest_version}'")
            if not args.no_pr:
                print("DRY RUN: Would create pull request")
            return

        # Perform the update
        branch_name = updater.create_update_branch(latest_version)
        updater.update_vcpkg_submodule(latest_version)
        updater.update_vcpkg_json_baseline(latest_commit)
        updater.commit_changes(current_version, latest_version)
        updater.push_branch(branch_name)

        if not args.no_pr:
            pr_url = updater.create_pull_request(current_version, latest_version, latest_commit, branch_name)

        print(f"\n✅ Successfully updated vcpkg from {current_version} to {latest_version}")
        print(f"📂 Changes committed to branch: {branch_name}")

    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == '__main__':
    main()