#!/usr/bin/env python3
"""Tests for update_vcpkg.py"""

import json
import subprocess
import unittest
from pathlib import Path
from unittest.mock import MagicMock, call, patch

from update_vcpkg import VcpkgUpdater


def make_result(stdout="", stderr="", returncode=0):
    """Create a mock CompletedProcess."""
    result = MagicMock(spec=subprocess.CompletedProcess)
    result.stdout = stdout
    result.stderr = stderr
    result.returncode = returncode
    return result


class TestPushBranch(unittest.TestCase):
    """Tests for push_branch — the method that failed in CI."""

    def setUp(self):
        self.repo_root = Path("/fake/repo")
        self.updater = VcpkgUpdater(self.repo_root)
        self.updater.run_command = MagicMock(return_value=make_result())

    def test_fetches_remote_tracking_ref_before_push(self):
        """Fetch must happen before push so --force-with-lease has a baseline."""
        self.updater.push_branch("automated/update-vcpkg-2026.03.18")

        calls = self.updater.run_command.call_args_list
        self.assertEqual(len(calls), 2)

        # First call: fetch remote-tracking ref
        fetch_call = calls[0]
        self.assertEqual(fetch_call, call(
            ['git', 'fetch', 'origin',
             'refs/heads/automated/update-vcpkg-2026.03.18'
             ':refs/remotes/origin/automated/update-vcpkg-2026.03.18'],
            check=False,
        ))

        # Second call: push with --force-with-lease
        push_call = calls[1]
        self.assertEqual(push_call, call(
            ['git', 'push', '--force-with-lease', 'origin',
             'automated/update-vcpkg-2026.03.18'],
        ))

    def test_fetch_failure_does_not_prevent_push(self):
        """When the remote branch doesn't exist yet, fetch fails but push proceeds."""
        fetch_result = make_result(returncode=128, stderr="fatal: couldn't find remote ref")
        push_result = make_result()
        self.updater.run_command = MagicMock(side_effect=[fetch_result, push_result])

        # Should not raise
        self.updater.push_branch("automated/update-vcpkg-2026.04.01")

        self.assertEqual(self.updater.run_command.call_count, 2)
        # Verify push was still called
        push_call = self.updater.run_command.call_args_list[1]
        self.assertIn('--force-with-lease', push_call[0][0])

    def test_push_failure_raises(self):
        """If the push itself fails, the error should propagate."""
        fetch_result = make_result()
        push_error = subprocess.CalledProcessError(1, 'git push')
        self.updater.run_command = MagicMock(side_effect=[fetch_result, push_error])

        with self.assertRaises(subprocess.CalledProcessError):
            self.updater.push_branch("automated/update-vcpkg-2026.03.18")


class TestCreateUpdateBranch(unittest.TestCase):

    def setUp(self):
        self.repo_root = Path("/fake/repo")
        self.updater = VcpkgUpdater(self.repo_root)
        self.updater.run_command = MagicMock(return_value=make_result())

    def test_branch_name_format(self):
        branch = self.updater.create_update_branch("2026.03.18")
        self.assertEqual(branch, "automated/update-vcpkg-2026.03.18")

    def test_uses_checkout_force_create(self):
        self.updater.create_update_branch("2026.03.18")
        self.updater.run_command.assert_called_once_with(
            ['git', 'checkout', '-B', 'automated/update-vcpkg-2026.03.18']
        )


class TestGetLatestVcpkgVersion(unittest.TestCase):

    def setUp(self):
        self.repo_root = Path("/fake/repo")
        self.updater = VcpkgUpdater(self.repo_root)

    def test_picks_first_valid_version_tag(self):
        tags_output = "2026.03.18\n2026.02.27\n2026.01.15\n"
        commit_output = "abc123def456\n"

        self.updater.run_command = MagicMock(side_effect=[
            make_result(),  # git fetch --tags
            make_result(stdout=tags_output),  # git tag --sort
            make_result(stdout=commit_output),  # git rev-list
        ])

        tag, commit = self.updater.get_latest_vcpkg_version()
        self.assertEqual(tag, "2026.03.18")
        self.assertEqual(commit, "abc123def456")

    def test_skips_non_version_tags(self):
        tags_output = "some-feature-tag\nv1.0.0\n2026.03.18\n2026.02.27\n"
        commit_output = "abc123\n"

        self.updater.run_command = MagicMock(side_effect=[
            make_result(),  # git fetch --tags
            make_result(stdout=tags_output),  # git tag --sort
            make_result(stdout=commit_output),  # git rev-list
        ])

        tag, _ = self.updater.get_latest_vcpkg_version()
        self.assertEqual(tag, "2026.03.18")

    def test_raises_when_no_version_tags(self):
        self.updater.run_command = MagicMock(side_effect=[
            make_result(),  # git fetch --tags
            make_result(stdout="some-tag\nv1.0\n"),  # git tag --sort
        ])

        with self.assertRaises(RuntimeError):
            self.updater.get_latest_vcpkg_version()


class TestCheckUpdateNeeded(unittest.TestCase):

    def setUp(self):
        self.repo_root = Path("/fake/repo")
        self.updater = VcpkgUpdater(self.repo_root)

    def test_update_needed_when_versions_differ(self):
        self.updater.get_latest_vcpkg_version = MagicMock(
            return_value=("2026.03.18", "abc123"))
        self.updater.get_current_vcpkg_version = MagicMock(
            return_value="2026.02.27")

        needed, current, latest, commit = self.updater.check_update_needed()
        self.assertTrue(needed)
        self.assertEqual(current, "2026.02.27")
        self.assertEqual(latest, "2026.03.18")

    def test_no_update_when_same_version(self):
        self.updater.get_latest_vcpkg_version = MagicMock(
            return_value=("2026.03.18", "abc123"))
        self.updater.get_current_vcpkg_version = MagicMock(
            return_value="2026.03.18")

        needed, _, _, _ = self.updater.check_update_needed()
        self.assertFalse(needed)

    def test_force_update_when_same_version(self):
        self.updater.get_latest_vcpkg_version = MagicMock(
            return_value=("2026.03.18", "abc123"))
        self.updater.get_current_vcpkg_version = MagicMock(
            return_value="2026.03.18")

        needed, _, _, _ = self.updater.check_update_needed(force=True)
        self.assertTrue(needed)


class TestHasStagedChanges(unittest.TestCase):

    def setUp(self):
        self.repo_root = Path("/fake/repo")
        self.updater = VcpkgUpdater(self.repo_root)

    def test_returns_true_when_changes_exist(self):
        self.updater.run_command = MagicMock(
            return_value=make_result(returncode=1))
        self.assertTrue(self.updater.has_staged_changes())

    def test_returns_false_when_clean(self):
        self.updater.run_command = MagicMock(
            return_value=make_result(returncode=0))
        self.assertFalse(self.updater.has_staged_changes())


class TestCommitChanges(unittest.TestCase):

    def setUp(self):
        self.repo_root = Path("/fake/repo")
        self.updater = VcpkgUpdater(self.repo_root)

    def test_skips_commit_when_no_changes(self):
        self.updater.has_staged_changes = MagicMock(return_value=False)
        self.updater.run_command = MagicMock()

        result = self.updater.commit_changes("2026.02.27", "2026.03.18")
        self.assertFalse(result)
        self.updater.run_command.assert_not_called()

    def test_commits_when_changes_exist(self):
        self.updater.has_staged_changes = MagicMock(return_value=True)
        self.updater.run_command = MagicMock(return_value=make_result())

        result = self.updater.commit_changes("2026.02.27", "2026.03.18")
        self.assertTrue(result)

        cmd = self.updater.run_command.call_args[0][0]
        self.assertEqual(cmd[:3], ['git', 'commit', '-s'])
        self.assertIn('2026.03.18', cmd[-1])

    def test_commit_uses_signoff(self):
        self.updater.has_staged_changes = MagicMock(return_value=True)
        self.updater.run_command = MagicMock(return_value=make_result())

        self.updater.commit_changes("old", "new")
        cmd = self.updater.run_command.call_args[0][0]
        self.assertIn('-s', cmd)


class TestCreatePullRequest(unittest.TestCase):

    def setUp(self):
        self.repo_root = Path("/fake/repo")
        self.updater = VcpkgUpdater(self.repo_root)

    def test_reuses_existing_pr(self):
        existing_pr = make_result(stdout="https://github.com/org/repo/pull/42\n")
        self.updater.run_command = MagicMock(return_value=existing_pr)

        url = self.updater.create_pull_request("old", "new", "abc", "branch")
        self.assertEqual(url, "https://github.com/org/repo/pull/42")
        # Should only call gh pr list, not gh pr create
        self.assertEqual(self.updater.run_command.call_count, 1)

    def test_creates_pr_when_none_exists(self):
        no_pr = make_result(stdout="", returncode=0)
        new_pr = make_result(stdout="https://github.com/org/repo/pull/43\n")
        self.updater.run_command = MagicMock(side_effect=[no_pr, new_pr])

        url = self.updater.create_pull_request("old", "new", "abc", "branch")
        self.assertEqual(url, "https://github.com/org/repo/pull/43")
        self.assertEqual(self.updater.run_command.call_count, 2)

    def test_pr_create_targets_main_branch(self):
        no_pr = make_result(stdout="", returncode=0)
        new_pr = make_result(stdout="https://github.com/org/repo/pull/43\n")
        self.updater.run_command = MagicMock(side_effect=[no_pr, new_pr])

        self.updater.create_pull_request("old", "new", "abc", "branch")
        create_cmd = self.updater.run_command.call_args_list[1][0][0]
        base_idx = create_cmd.index('--base')
        self.assertEqual(create_cmd[base_idx + 1], 'main')


class TestUpdateVcpkgJsonBaseline(unittest.TestCase):

    def setUp(self):
        self.repo_root = Path("/fake/repo")
        self.updater = VcpkgUpdater(self.repo_root)
        self.updater.run_command = MagicMock(return_value=make_result())

    def test_updates_baseline_in_json(self):
        vcpkg_data = {
            "name": "vanillapdf",
            "builtin-baseline": "old_hash_123"
        }

        with patch("builtins.open", unittest.mock.mock_open(
                read_data=json.dumps(vcpkg_data))) as mock_file:
            with patch.object(Path, 'exists', return_value=True):
                self.updater.update_vcpkg_json_baseline("new_hash_456")

        # Verify the written data contains the new baseline
        write_calls = mock_file().write.call_args_list
        written = "".join(c[0][0] for c in write_calls)
        parsed = json.loads(written)
        self.assertEqual(parsed['builtin-baseline'], "new_hash_456")

    def test_raises_when_file_missing(self):
        with patch.object(Path, 'exists', return_value=False):
            with self.assertRaises(FileNotFoundError):
                self.updater.update_vcpkg_json_baseline("abc")


if __name__ == '__main__':
    unittest.main()
