# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Quick Reference

```bash
# Configure and build (Windows)
cmake --preset windows-x64-msvc-18
cmake --build --preset windows-x64-msvc-18

# Configure and build (Linux/macOS)
cmake --preset linux-x64-gcc      # or macos-arm64
cmake --build --preset linux-x64-gcc

# Run all tests (Windows - must specify build config)
ctest --preset windows-x64-msvc-18 --build-config Debug

# Run specific test
ctest --preset windows-x64-msvc-18 --build-config Debug -R "TestName" --output-on-failure

# Initialize submodules (required before first build)
git submodule sync --recursive && git submodule update --init --recursive
```

## Project Naming Conventions

- **Official/Marketing**: "Vanilla.PDF" (with dot) - documentation, user-facing messages
- **Code/Technical**: "vanillapdf" (lowercase, no dot) - code, commits, technical discussions
- **Avoid**: "VanillaPDF" (mixed case without dot)

## Repository Workflow (CRITICAL)

**Branch Targets:**
- **Default branch**: `main` - ALL pull requests should target this branch
- **Historical reference**: `master` - legacy branch for historical reference ONLY
- When using `gh pr create`, always specify `--base main` or omit the base flag (defaults to main)

**ALL changes MUST go through a branch and pull request:**
- NEVER commit directly to `main` or `release/*` branches (protected)
- Create a new branch for every change: `feature/description` or `fix/description`
- The `release/` prefix is **reserved for release branches only** (e.g. `release/2.1`, `release/2.2`). Never use it for feature, bump, or fix branches.
- Base branches on `main` (or `release/*` for hotfixes)

**`git push` must always be a separate, explicit command:**
- NEVER chain `git push` with `&&` after a commit or other commands
- Push triggers CI pipelines; it must be a deliberate, standalone action
- Wrong: `git add ... && git commit -m "..." && git push`
- Right: commit first, then push as a separate step after confirming

**Pull request bodies (gh on PowerShell):**
- NEVER use `gh pr create --body @-` (or `--body-file -`). In this PowerShell harness nothing is piped to stdin, so `gh` stores the literal string `@-` as the body. This has silently shipped empty PR descriptions.
- Pass the body inline with a single-quoted here-string: `gh pr create --base main --title "..." --body @'` … `'@`. Inside a single-quoted here-string, apostrophes are literal — do NOT double them (`GitHub's`, not `GitHub''s`); the only restriction is that a line may not begin with `'@`.
- Alternatively write the body to a temp file and use `--body-file <path>`.
- ALWAYS verify after creation: `gh pr view <num> --json body --jq '.body'` and confirm the body is the intended text (not `@-`, not empty, apostrophes intact). The same applies to `gh pr edit --body` and `gh issue create --body`.

## GitHub Issues

**Always add labels** when creating GitHub issues using `gh issue create`. Use appropriate labels from the repository to categorize issues for grouping and filtering. Common labels:
- `bug` — Something isn't working
- `enhancement` — New feature or request
- `technical-debt` — Code quality and refactoring issues
- `documentation` — Improvements or additions to documentation
- `cmake` / `build-system` / `vcpkg` — Build-related issues
- `ci-cd` / `github_actions` — CI/CD issues
- `performance` — Performance improvements
- `priority-high` / `priority-medium` / `priority-low` — Priority levels

Example:
```bash
gh issue create --title "Fix iterator lifetime" --label "bug,technical-debt" --body "..."
```

## GitHub Actions Security (CRITICAL)

**Always pin GitHub Actions to full commit SHAs**, never to mutable tags like `@v4`. This prevents supply-chain attacks and satisfies OpenSSF Scorecard's Pinned-Dependencies check.

```yaml
# BAD — mutable tag, flagged by Scorecard
uses: actions/checkout@v4

# GOOD — immutable SHA with tag comment
uses: actions/checkout@11bd71901bbe5b1630ceea73d27597364c9af683  # v4.2.2
```

To resolve a tag to its commit SHA:
```bash
# Lightweight tags: SHA is the commit directly
gh api repos/OWNER/REPO/git/refs/tags/TAG --jq '.object.sha'

# Annotated tags: dereference the tag object to get the commit SHA
gh api repos/OWNER/REPO/git/tags/$(gh api repos/OWNER/REPO/git/refs/tags/TAG --jq '.object.sha') --jq '.object.sha'
```

Always include the original tag name as a comment (`# v4.2.2`) so the pinned version remains human-readable.

## Automation Bot

The repository uses `vanillapdf-bot` (info@vanillapdf.com) for automated operations:
- Monthly vcpkg updates, release automation, vcpkg PRs to Microsoft, Homebrew PRs

When creating GitHub Actions workflows that commit or create PRs:
```yaml
- name: Configure Git
  run: |
    git config --global user.name "vanillapdf-bot"
    git config --global user.email "info@vanillapdf.com"
```

## vcpkg Submodule Management

**NEVER modify `external/vcpkg/`** - it's a Git submodule updated monthly by vanillapdf-bot.

For vcpkg port development, work in `ports/vanillapdf/` (not `external/vcpkg/ports/`).

## Development Practices

### Code Style

- C++17 standard with extensions disabled
- Uses precompiled headers (`precompiled.h`)
- Visual Studio .natvis files for debugging support
- Follow existing patterns in similar classes
- See `.claude/rules/coding-style.md` for detailed coding style preferences

### Contribution Guidelines

- All commits must be signed off with `git commit -s`
- Base PRs on `main` branch
- Match existing coding style
- Test changes with available presets
- See CONTRIBUTING.md for full guidelines
