# Real-World FetchContent Integration Example

This example tests the **actual end-user experience** of consuming VanillaPDF via CMake's `FetchContent` from the GitHub repository.

## What it tests:

1. **Real Git Integration**: Fetches VanillaPDF from GitHub (not local source)
2. **Hybrid Dependency Resolution**: Tests using system packages + vcpkg for optimal build speed
3. **Configuration**: Verifies correct dependency-only configuration:
   - `VANILLAPDF_INTERNAL_VCPKG=OFF` (parent project manages vcpkg)
   - System packages for major deps (OpenSSL, JPEG, OpenJPEG, zlib)
   - vcpkg for packages not available as system deps (spdlog, nlohmann-json)
   - `VANILLAPDF_ENABLE_PACKAGING=OFF` (no packaging for dependency usage)
   - `VANILLAPDF_ENABLE_TESTS=OFF` (no tests for dependency usage)
4. **Complete Build Chain**: From dependency installation to final executable linking

## Running the example:

### Prerequisites:
Install system dependencies (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install -y libssl-dev libjpeg-turbo8-dev zlib1g-dev libopenjp2-7-dev libopenjp2-tools
```

### Basic usage:
```bash
cd examples/fetchcontent-integration
cmake -S . -B build
cmake --build build
```

### Testing specific commits/tags:
```bash
# Test against a specific release
cmake -S . -B build -DVANILLAPDF_GIT_TAG=v2.1.0

# Test against a specific commit (useful for CI)
cmake -S . -B build -DVANILLAPDF_GIT_TAG=abc123def

# Test against a branch
cmake -S . -B build -DVANILLAPDF_GIT_TAG=main
```

### Automatic Git tag selection:
- **CI environment**: Uses `$GITHUB_SHA` (current commit being tested)
- **Manual override**: Uses `VANILLAPDF_GIT_TAG` if specified
- **Fallback**: Uses `main` (latest development branch)

## Expected behavior:

- ✅ CMake configuration succeeds
- ✅ System dependencies detected (OpenSSL, JPEG, OpenJPEG, zlib)
- ✅ vcpkg manages minimal dependencies (spdlog, nlohmann-json)
- ✅ No packaging features enabled (no CPack conflicts)
- ✅ All optional features work (encryption, JPEG, JPEG2000)
- ✅ Fast build due to system package usage

## Why This Matters:

This test catches real-world integration issues that users would encounter:
- **Network failures**: Git cloning or dependency fetching problems
- **Version conflicts**: Issues with specific Git tags or branches
- **Dependency resolution**: Real vcpkg bootstrapping and package building
- **Cross-platform issues**: Platform-specific integration problems

## Key Benefits Verified:

- ✅ **Minimal configuration**: Parent project manages its own dependencies
- ✅ **Hybrid approach**: System packages for speed, vcpkg for unavailable deps
- ✅ **Network resilient**: Handles real Git and dependency fetching
- ✅ **Version pinnable**: Can use specific tags, branches, or commits
- ✅ **Full features**: All VanillaPDF features available by default
- ✅ **Fast builds**: System packages avoid long vcpkg compilation times

**This is the actual experience external users will have.**