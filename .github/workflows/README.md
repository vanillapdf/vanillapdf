# GitHub Workflows Organization

This directory contains all GitHub Actions workflows organized by category.

## Folder Structure

### 📁 `examples/`
Workflows related to testing examples and integration patterns:
- `fetchcontent-integration.yml` - Tests FetchContent integration example

### 📁 `testing/`
Core testing and validation workflows:
- `nightly-check.yml` - Comprehensive cross-platform testing
- `sanity-check.yml` - Quick validation tests
- `coverage.yml` - Code coverage analysis
- `stack-sanitizer.yml` - Memory safety testing
- `codeql.yml` - Security analysis

### 📁 `packaging/`
Package building and distribution workflows:
- `build-nuget.yml` - Build NuGet packages
- `build-deb-package.yml` - Build Debian packages
- `build-brew-package.yml` - Build Homebrew packages
- `nightly-nuget.yml` - Nightly NuGet package builds

### 📁 `maintenance/`
Release management and maintenance workflows:
- `release.yml` - Full release automation
- `github-release.yml` - GitHub release creation
- `github-pages.yml` - Documentation deployment
- `create-vcpkg-pr.yml` - vcpkg port updates

## Workflow Dependencies

Some workflows reference others using the `uses:` keyword:
- `release.yml` calls workflows from `packaging/` and `maintenance/`
- `nightly-nuget.yml` calls `packaging/build-nuget.yml`

All path references have been updated to reflect the new folder structure.