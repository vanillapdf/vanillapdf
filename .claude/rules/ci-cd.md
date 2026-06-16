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

Add label `backport release/X.Y` to a PR before merging. The workflow cherry-picks and creates a backport PR automatically. If cherry-pick fails, manually create the backport.

## Homebrew

Formula template in `homebrew/vanillapdf.rb.in` (the concrete `.rb` is generated at release time). Releases auto-PR to `Homebrew/homebrew-core` via `update-homebrew.yml`.
