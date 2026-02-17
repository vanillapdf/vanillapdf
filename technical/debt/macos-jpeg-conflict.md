# macOS JPEG Library Conflict

**Priority:** Medium
**Component:** CI/Build System
**Status:** Fixed in build configuration
**GitHub Issue:** [#125](https://github.com/vanillapdf/vanillapdf/issues/125)

## Problem Summary

macOS CI runners fail with error: `"Wrong JPEG library version: library is 62, caller expects 80"`

## Root Cause Analysis

### The Issue
- **Compile-time**: CMake's `find_package(JPEG)` finds system JPEG headers (version 80)
- **Runtime**: Dynamic linker loads vcpkg JPEG libraries (version 62)
- **Result**: Header/runtime version mismatch causes runtime errors

### Why It Happens
1. macOS GitHub runners have Homebrew JPEG packages pre-installed
2. CMake searches system paths (`/usr/local/lib`, `/opt/homebrew/lib`) before vcpkg paths
3. System JPEG headers declare version 80 compatibility
4. vcpkg provides JPEG libraries with version 62 compatibility
5. Application compiled against v80 headers fails when loaded with v62 runtime

### Affected Runners
- **macOS-13 (Intel x64)**: Libraries in `/usr/local/lib/`
- **macOS-15 (Apple Silicon ARM64)**: Libraries in `/opt/homebrew/lib/`
- **Both architectures**: Same 28 symlinks removed by `brew unlink`

## Current Workaround

### Status
The Homebrew `brew unlink ...` workaround has been removed from CI workflows.
The build is now fixed via vcpkg/CMake configuration (see below) so compile-time and runtime
consistently use vcpkg-provided JPEG.

## Proper Solutions

### Option 1: vcpkg Toolchain Configuration (RECOMMENDED)
```json
// cmake/presets/shared.json
"cacheVariables": {
  "VCPKG_PREFER_SYSTEM_LIBS": "OFF"
}
```

**Pros:** Uses vcpkg's intended mechanism, clean, affects all dependencies
**Cons:** May need testing with other dependencies

### Option 2: Explicit vcpkg Paths
```cmake
if (NOT VANILLAPDF_EXTERNAL_JPEG)
    find_package(libjpeg-turbo CONFIG REQUIRED
        PATHS ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}
        NO_DEFAULT_PATH)
endif()
```

**Pros:** Explicit control, guaranteed to work
**Cons:** More verbose, needs per-dependency configuration

### Option 3: CMake Find Root Path Mode
```cmake
if (NOT VANILLAPDF_EXTERNAL_JPEG)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    find_package(libjpeg-turbo CONFIG REQUIRED)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
endif()
```

**Pros:** Standard CMake approach
**Cons:** May affect other find_package calls

## Implementation Plan

### Phase 1: Immediate (DONE)
- ✅ Deploy workaround to unblock other PRs
- ✅ Add TODO comments with clear references
- ✅ Document the technical debt

### Phase 2: Proper Fix (TODO)
1. **Test Option 1** on CI
   - Ensure `VCPKG_PREFER_SYSTEM_LIBS=OFF` is present in `cmake/presets/shared.json`
   - Verify on both Intel and Apple Silicon runners
   - Verify no regressions with other dependencies

2. **Fallback to Option 2** if Option 1 has issues
   - Implement explicit vcpkg paths for JPEG
   - Test thoroughly on all platforms

3. **Validation**
   - Confirm no `brew unlink` workaround exists in workflows
   - Verify clean builds on all macOS runners
   - Test both Debug and Release configurations

### Phase 3: Cleanup
- Remove all `TEMPORARY WORKAROUND` sections
- Update this documentation as resolved
- Close GitHub issue

## Timeline

- **Immediate**: Workaround deployed ✅
- **Next Sprint**: Implement proper vcpkg solution
- **Target**: Resolve within 2 weeks of merge

## References

- vcpkg documentation: https://vcpkg.readthedocs.io/
- CMake find_package documentation
- Related discussion in development team