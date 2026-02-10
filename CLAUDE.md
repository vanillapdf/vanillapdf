# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Quick Reference

```bash
# Configure and build (Windows)
cmake --preset windows-x64-msvc-17
cmake --build --preset windows-x64-msvc-17

# Configure and build (Linux/macOS)
cmake --preset linux-x64-gcc      # or macos-arm64
cmake --build --preset linux-x64-gcc

# Run all tests (Windows - must specify build config)
ctest --preset windows-x64-msvc-17 --build-config Debug

# Run specific test
ctest --preset windows-x64-msvc-17 --build-config Debug -R "TestName" --output-on-failure

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
- Base branches on `main` (or `release/*` for hotfixes)

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
- Do not insert structurally different code into a group of similarly-looking lines without separating it with a blank line and a comment. Uniform blocks (e.g., a series of assignments) should stay visually cohesive.
- Prefer `= value` assignment over `{value}` brace initialization for member variables (e.g., `int m_count = 0;` not `int m_count{0};`)
- When two consecutive single-line `if` statements can be combined with `||`, prefer the combined form: `return (A) || (B);` instead of `if (A) return true; return B;`

### Contribution Guidelines

- All commits must be signed off with `git commit -s`
- Base PRs on `main` branch
- Match existing coding style
- Test changes with available presets
- See CONTRIBUTING.md for full guidelines
