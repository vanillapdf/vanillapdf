# FetchContent CMake Cache Variable Review

## Problem Statement

When vanillapdf is used as a FetchContent dependency, some CMake variables are inadvertently exposed to the parent project's cache, potentially causing configuration pollution and unexpected behavior.

## Current Issues

1. **Variable Inheritance**: Variables defined with `CACHE` become visible in parent project
2. **UI Pollution**: Parent projects see vanillapdf-specific options in cmake-gui/ccmake
3. **Unexpected Behavior**: Parent projects may inherit build flags (e.g., `VANILLAPDF_FORCE_32_BIT`)

## Analysis of Current Variables

### Should Remain as `option()` (User-Facing Features)
These control features that parent projects may legitimately want to configure:

```cmake
option(VANILLAPDF_ENABLE_ENCRYPTION "Enable PDF encryption/decryption support" ON)
option(VANILLAPDF_ENABLE_JPEG "Enable JPEG image support" ON)
option(VANILLAPDF_ENABLE_JPEG2000 "Enable JPEG2000 image support" ON)
option(VANILLAPDF_ENABLE_ZLIB "Enable zlib compression support" ON)
option(VANILLAPDF_EXTERNAL_OPENSSL "Use OpenSSL from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_JPEG "Use libjpeg from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_OPENJPEG "Use openjpeg from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_ZLIB "Use zlib from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_SPDLOG "Use spdlog from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_NLOHMANN_JSON "Use nlohmann-json from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_GTEST "Use GoogleTest from the system or a package manager" OFF)
option(VANILLAPDF_EXTERNAL_BENCHMARK "Use Google Benchmark from the system or a package manager" OFF)
```

### Should Become `CACHE INTERNAL` (Implementation Details)
These are internal implementation details that shouldn't pollute parent projects:

```cmake
set(VANILLAPDF_FORCE_32_BIT OFF CACHE INTERNAL "Force 32-bit output binary regardless of architecture")
set(VANILLAPDF_USE_STATIC_CRT OFF CACHE INTERNAL "Use static MSVC runtime (/MT) instead of dynamic (/MD)")
set(VANILLAPDF_INTERNAL_VCPKG ON CACHE INTERNAL "Enable internal vcpkg dependency management")
set(VANILLAPDF_ENABLE_TESTS OFF CACHE INTERNAL "Perform test scenarios to ensure stable releases")
set(VANILLAPDF_ENABLE_BENCHMARK OFF CACHE INTERNAL "Include benchmarking project to measure performance")
set(VANILLAPDF_ENABLE_PACKAGING OFF CACHE INTERNAL "Enable packaging features (CPack, DEB, Brew, etc.)")
```

## Benefits of `CACHE INTERNAL`

1. **Clean Parent UI**: Variables hidden from cmake-gui/ccmake in parent projects
2. **Still Overrideable**: Parent projects can still override if needed programmatically
3. **Performance**: Values cached for build performance
4. **Appropriate Defaults**: Different defaults for sub-project vs main project usage

## Implementation Strategy

1. **Phase 1**: Review and categorize all vanillapdf options
2. **Phase 2**: Convert implementation details to `CACHE INTERNAL`
3. **Phase 3**: Update documentation with override examples
4. **Phase 4**: Test FetchContent scenarios

## Documentation Needed

### CLAUDE.md Updates
Add section on FetchContent variable overrides:

```cmake
# Override vanillapdf internal settings when using FetchContent
set(VANILLAPDF_ENABLE_TESTS ON CACHE INTERNAL "Enable tests for debugging")
set(VANILLAPDF_USE_STATIC_CRT ON CACHE INTERNAL "Force static CRT")

FetchContent_Declare(vanillapdf ...)
FetchContent_MakeAvailable(vanillapdf)
```

### README.md Updates
Document the distinction between user-facing and internal options.

## Testing Requirements

1. **FetchContent Integration**: Verify clean parent project cache
2. **Override Functionality**: Test parent project can override internal variables
3. **Default Behavior**: Ensure appropriate defaults for sub-project usage

## Files to Modify

- `CMakeLists.txt` - Convert variables to `CACHE INTERNAL`
- `CLAUDE.md` - Add FetchContent override documentation
- `README.md` - Document option categories
- Test FetchContent example with changes

## Priority

**Medium-High** - This affects FetchContent usability but doesn't break functionality. Should be addressed before next release to improve developer experience.