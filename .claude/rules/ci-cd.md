# CI/CD and Release

## GitHub Actions Workflows

- `sanity-check.yml` - Fast per-PR build and test gate
- `nightly-check.yml` - Full platform matrix testing (Linux, Windows, macOS, Android)
- `coverage.yml` - Code coverage with Codecov
- `sanitizers.yml` - ASan, UBSan, and TSan checks (one workflow, 3 parallel jobs after shared vcpkg-setup)
- `fuzzing.yml` - Fuzz testing (builds `vanillapdf.fuzzer`)
- `codeql.yml` - Security analysis
- `scorecard.yml` - OpenSSF Scorecard supply-chain analysis
- `dependency-review.yml` - Dependency review on pull requests
- `conformance-check.yml` - PDF conformance validation
- `signature-interop-check.yml` - Digital signature interoperability checks
- `examples-integration.yml` - Integration testing of usage examples
- `validate-workflows.yml` - Lints/validates the workflow YAML files
- `build-nuget.yml` / `nightly-nuget.yml` - NuGet package building (release / nightly)
- `build-deb-package.yml` / `build-rpm-package.yml` / `build-brew-package.yml` - Distro package building
- `github-pages.yml` - Documentation deployment
- `update-vcpkg.yml` / `create-vcpkg-pr.yml` - vcpkg updates (vanillapdf-bot)
- `update-homebrew.yml` - Homebrew formula PRs (vanillapdf-bot)
- `create-conan-pr.yml` - Conan Center PRs (vanillapdf-bot)
- `release.yml` / `github-release.yml` - Release automation (vanillapdf-bot)
- `backport.yml` - Auto-backport merged PRs to release branches

## Build Matrix

Windows 2022/2025 (x86/x64), Linux Ubuntu 22.04/24.04, Rocky 8/9, Fedora 41/42 (x64/ARM64), macOS 13 (x64), 14/15 (ARM64), Android (arm64, armv7, x86, x86_64)

## Release Process

- `main` → development, `release/X.Y` → release branches, tags `vX.Y.Z`
- Major/Minor: create `release/X.Y` from `main`. Patch: work on existing `release/X.Y`

## Backporting

Add label `backport release/X.Y` to a PR **before merging**. On merge, `backport.yml` cherry-picks the squash commit onto `release/X.Y` and opens a backport PR automatically.

**Label lifecycle** (driven by `backport.yml` + `.github/scripts/backport.sh`):
- `backport release/X.Y` — request: backport this PR to that branch.
- `backported release/X.Y` — applied automatically when the backport PR **merges** (removes the `backport` label). Only fires when the backport PR title is `[Backport release/X.Y] …` and its body contains `Backport of #<original>` — keep that format so the automation runs.
- `backport-failed release/X.Y` — applied automatically when the auto cherry-pick **fails**. This is the queryable signal that a manual backport is owed.

**Find backports that still need attention:**
```bash
gh pr list --state all --label "backport-failed release/2.2"
```
The label is not auto-cleared, so remove it by hand once the manual backport is done (or as part of doing it).

**Manual backport** (when the auto cherry-pick conflicts — common when the release branch predates a feature the PR was built on):
```bash
git fetch origin
git checkout -b backport/<PR>-release-2.2 origin/release/2.2
git cherry-pick -x <squash-merge-commit-of-original-PR>   # resolve conflicts
# Drop parts that don't belong on the older branch (e.g. code for features not on release/X.Y).
git push origin backport/<PR>-release-2.2
gh pr create --base release/2.2 \
  --title "[Backport release/2.2] <original title> (#<PR>)" \
  --body-file <body>   # body must contain: Backport of #<PR> to `release/2.2`
```
Build and run the affected tests against the release branch before opening the PR — release branches diverge from `main`.

**When creating a new `release/X.Y` branch**, also create its three labels (`backport release/X.Y`, `backported release/X.Y`, `backport-failed release/X.Y`) — they are created manually, and `backport.sh` applies `backport-failed` with a bare `gh` call that assumes the label exists.

**Retiring a release line (EOL):** when a `release/X.Y` line is no longer supported, prune only the *actionable* labels and keep the historical one.
- **Delete** `backport release/X.Y` (a request trigger — no meaning once the line is dead) and `backport-failed release/X.Y` (an action-needed signal — moot for an EOL line).
- **Keep `backported release/X.Y` forever** — it is the historical record of what shipped where.
- **Leave the `release/X.Y` branch in place** — the `deletion` rule in the "Protect release branches" ruleset blocks removal anyway, and the branch is harmless history.

The `backported release/X.Y` label is the fast filter, but it is not the system of record: every backport also lands as a PR titled `[Backport release/X.Y] …` (body `Backport of #<original>`) plus a commit on the release branch, so the full trail survives even a label deletion:
```bash
gh pr list --state all --search "[Backport release/2.1] in:title"   # reconstruct a line's history without the label
```
Note: **deleting a label erases it from every PR it was on, and recreating the label does not restore those associations** — so never delete `backported release/X.Y`.

## Homebrew

Formula template in `homebrew/vanillapdf.rb.in` (the concrete `.rb` is generated at release time). Releases auto-PR to `Homebrew/homebrew-core` via `update-homebrew.yml`.
