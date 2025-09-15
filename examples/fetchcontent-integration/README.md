# Real-World FetchContent Integration Example

This example tests the **actual end-user experience** of consuming VanillaPDF via CMake's `FetchContent` from the GitHub repository.

## What it tests:

1. **Real Git Integration**: Fetches VanillaPDF from GitHub (not local source)
2. **Network Dependency Resolution**: Tests actual vcpkg bootstrapping and dependency fetching
3. **Auto-detection**: Verifies correct configuration in real-world scenario:
   - `VANILLAPDF_MANAGE_DEPS=ON` (manages own dependencies via internal vcpkg)
   - `VANILLAPDF_ENABLE_PACKAGING=OFF` (no packaging conflicts)
4. **Complete Build Chain**: From Git clone to final executable linking

## Running the example:

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
- ✅ VanillaPDF manages its own dependencies (vcpkg bootstrap)
- ✅ No packaging features enabled (no CPack conflicts)
- ✅ All optional features work (encryption, JPEG, JPEG2000)
- ✅ Parent project has no dependency management requirements

## Why This Matters:

This test catches real-world integration issues that users would encounter:
- **Network failures**: Git cloning or dependency fetching problems
- **Version conflicts**: Issues with specific Git tags or branches
- **Dependency resolution**: Real vcpkg bootstrapping and package building
- **Cross-platform issues**: Platform-specific integration problems

## Key Benefits Verified:

- ✅ **Zero configuration**: Just `FetchContent_Declare` and it works
- ✅ **Self-contained**: No external vcpkg setup required from parent
- ✅ **Network resilient**: Handles real Git and dependency fetching
- ✅ **Version pinnable**: Can use specific tags, branches, or commits
- ✅ **Full features**: All VanillaPDF features available by default

**This is the actual experience external users will have.**