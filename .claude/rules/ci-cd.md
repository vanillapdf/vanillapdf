# CI/CD and Release

## GitHub Actions Workflows

- `nightly-check.yml` - Full platform matrix testing (Linux, Windows, macOS, Android)
- `coverage.yml` - Code coverage analysis with Codecov integration
- `stack-sanitizer.yml` - Address sanitizer testing for memory safety
- `codeql.yml` - Security analysis with GitHub CodeQL
- `build-nuget.yml` / `build-deb-package.yml` / `build-brew-package.yml` - Package building
- `github-pages.yml` - Documentation deployment
- `update-vcpkg.yml` - Automated monthly vcpkg updates (uses vanillapdf-bot)
- `create-vcpkg-pr.yml` - Manual vcpkg update workflow (uses vanillapdf-bot)
- `update-homebrew.yml` - Homebrew formula PR workflow (uses vanillapdf-bot)
- `create-conan-pr.yml` - Conan Center Index PR workflow (uses vanillapdf-bot)
- `release.yml` - Release automation workflow (uses vanillapdf-bot)
- `backport.yml` - Automatic backporting of merged PRs to release branches

## Build Matrix

Builds are tested on:
- Windows: 2022, 2025 (x86/x64, MSVC 17)
- Linux: Ubuntu 22.04/24.04, Rocky 8/9, Fedora 41/42 (x64/ARM64)
- macOS: 13 (x64), 14/15 (ARM64)
- Android: arm64, armv7, x86, x86_64

## Release Process

**Branch Structure:**
- `main` - Development branch (default)
- `release/X.Y` - Release branches for major.minor versions
- Tags: `vX.Y.Z` format on release branches

**Workflow:**
- Major/Minor: Create `release/X.Y` from `main`
- Patch: Work on existing `release/X.Y`
- Hotfixes: Branch from release branch if needed

## Homebrew Formula

The Homebrew formula is maintained in `homebrew/vanillapdf.rb` and submitted to `Homebrew/homebrew-core` during releases.

**Development:**
- Formula source: `homebrew/vanillapdf.rb`
- Integration test: `examples/homebrew-integration/`
- Test locally: `brew install --build-from-source --HEAD ./homebrew/vanillapdf.rb`

**Release workflow:**
- `update-homebrew.yml` creates PRs to `Homebrew/homebrew-core` after production gate
- Requires fork: `vanillapdf/homebrew-core`
- Updates URL and SHA256 automatically from release tag

## Backporting PRs to Release Branches

To backport a merged PR to a release branch:

1. Add a label `backport release/X.Y` to the PR (e.g., `backport release/2.2`)
2. When the PR is merged, the backport workflow automatically:
   - Cherry-picks the commit to the target branch
   - Creates a new PR with title `[Backport release/X.Y] <original title>`
   - Adds `backported release/X.Y` label to the new PR
3. If cherry-pick fails (conflicts), manually create a backport PR

Available labels:
- `backport release/2.2` - Request backport to release/2.2 branch
- `backport release/2.1` - Request backport to release/2.1 branch
- `backported release/2.2` - PR was backported to release/2.2
- `backported release/2.1` - PR was backported to release/2.1

For new release branches, create labels following the patterns `backport <branch>` and `backported <branch>`.

**Automated Workflows Using vanillapdf-bot:**
- All vcpkg-related automation
- Release processes and package updates
- Monthly maintenance tasks
- Any workflow that creates commits or PRs automatically
