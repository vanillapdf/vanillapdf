# CI/CD and Release

## GitHub Actions Workflows

- `nightly-check.yml` - Full platform matrix testing (Linux, Windows, macOS, Android)
- `coverage.yml` - Code coverage with Codecov
- `stack-sanitizer.yml` - Address sanitizer testing
- `codeql.yml` - Security analysis
- `build-nuget.yml` / `build-deb-package.yml` / `build-brew-package.yml` - Package building
- `github-pages.yml` - Documentation deployment
- `update-vcpkg.yml` / `create-vcpkg-pr.yml` - vcpkg updates (vanillapdf-bot)
- `update-homebrew.yml` - Homebrew formula PRs (vanillapdf-bot)
- `create-conan-pr.yml` - Conan Center PRs (vanillapdf-bot)
- `release.yml` - Release automation (vanillapdf-bot)
- `backport.yml` - Auto-backport merged PRs to release branches

## Build Matrix

Windows 2022/2025 (x86/x64), Linux Ubuntu 22.04/24.04, Rocky 8/9, Fedora 41/42 (x64/ARM64), macOS 13 (x64), 14/15 (ARM64), Android (arm64, armv7, x86, x86_64)

## Release Process

- `main` → development, `release/X.Y` → release branches, tags `vX.Y.Z`
- Major/Minor: create `release/X.Y` from `main`. Patch: work on existing `release/X.Y`

## Backporting

Add label `backport release/X.Y` to a PR before merging. The workflow cherry-picks and creates a backport PR automatically. If cherry-pick fails, manually create the backport.

## Homebrew

Formula in `homebrew/vanillapdf.rb`. Test: `brew install --build-from-source --HEAD ./homebrew/vanillapdf.rb`. Releases auto-PR to `Homebrew/homebrew-core` via `update-homebrew.yml`.
