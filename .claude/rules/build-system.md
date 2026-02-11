# Build System

## CRITICAL: Test Performance

- **NEVER run the full test suite (`ctest` without `-R`) in Debug mode** — it takes 40+ minutes on Windows.
- Use Debug only for targeted tests: `ctest --preset <preset> --build-config Debug -R "TestPattern" --output-on-failure`
- Use **Release** (`--build-config Release`) for full test suite runs.

## CMake Presets (Recommended)

List available presets:
```bash
cmake --list-presets
```

### Presets Structure

Presets are organized by platform in separate files:
- `cmake/presets/windows.json` - Visual Studio and Ninja generators
- `cmake/presets/linux.json` - GCC and Clang compilers
- `cmake/presets/macos.json` - AppleClang for x64 and ARM64
- `cmake/presets/android.json` - NDK toolchain for all Android ABIs

Each preset includes configure, build, and test configurations.

Common presets include:
- `windows-x64-msvc-17` / `windows-x86-msvc-17` - Windows with Visual Studio 2022 (dynamic CRT)
- `windows-x64-msvc-17-static` / `windows-x86-msvc-17-static` - Windows with Visual Studio 2022 (static CRT)
- `windows-x64-msvc-17-static-md` / `windows-x86-msvc-17-static-md` - Windows with Visual Studio 2022 (static libs, dynamic CRT)
- `windows-x64-msvc-18` / `windows-x86-msvc-18` - Windows with Visual Studio 2026 (dynamic CRT)
- `windows-x64-msvc-18-static` / `windows-x86-msvc-18-static` - Windows with Visual Studio 2026 (static CRT)
- `windows-x64-msvc-18-static-md` / `windows-x86-msvc-18-static-md` - Windows with Visual Studio 2026 (static libs, dynamic CRT)
- `linux-x64-gcc` / `linux-arm64-gcc` - Linux with GCC
- `linux-x64-clang` / `linux-arm64-clang` - Linux with Clang
- `linux-x64-musl` / `linux-arm64-musl` / `linux-arm-musl` - Linux with musl libc
- `macos-x64` / `macos-arm64` - macOS builds
- `android-*` - Android builds (arm64, armv7, x86, x86_64)

Build with a preset:
```bash
cmake --preset windows-x64-msvc-17
cmake --build --preset windows-x64-msvc-17
```

### Windows Build Notes

Windows presets use Visual Studio generators and automatically configure:
- CRT linking based on preset variant:
  - Standard presets (`windows-x*-msvc-17`, `windows-x*-msvc-18`): Dynamic CRT (default)
  - Static presets (`windows-x*-msvc-17-static`, `windows-x*-msvc-18-static`): Static CRT (`VANILLAPDF_USE_STATIC_CRT=ON`)
  - Static-MD presets (`windows-x*-msvc-17-static-md`, `windows-x*-msvc-18-static-md`): Static libs + dynamic CRT
- Platform-specific vcpkg triplets:
  - `x64-windows` (standard presets, dynamic CRT)
  - `x64-windows-static` (static presets, static CRT)
  - `x64-windows-static-md` (static-md presets, static libs + dynamic CRT)
- Visual Studio 2022 (msvc-17) and Visual Studio 2026 (msvc-18) generators available

## Build Targets

| Target | Type | Language | Depends On | Enabled By |
|--------|------|----------|------------|------------|
| `vanillapdf` | STATIC or SHARED lib | C++17 | vcpkg deps | Always |
| `vanillapdf.tools` | Executable | C | vanillapdf | Always |
| `vanillapdf.unittest` | Executable | C++ | vanillapdf, GTest | `VANILLAPDF_ENABLE_TESTS=ON` |
| `vanillapdf.test` | Executable | C | vanillapdf, Python3 | `VANILLAPDF_ENABLE_TESTS=ON` |
| `vanillapdf.benchmark` | Executable | C++ | vanillapdf, benchmark | `VANILLAPDF_ENABLE_BENCHMARK=ON` |

Alias: `vanillapdf::vanillapdf` for CMake consumption via `find_package(vanillapdf CONFIG)`.

## Source File Organization

Source files use **explicit lists** (NOT `file(GLOB)`). All declared in `src/vanillapdf/CMakeLists.txt`:

```cmake
VANILLAPDF_SYNTAX_SOURCES          # syntax/objects/, parsers/, files/, filters/, exceptions/
VANILLAPDF_SEMANTICS_SOURCES       # semantics/objects/, extensions/, utils/
VANILLAPDF_CONTENTS_SOURCES        # contents/
VANILLAPDF_UTILS_SOURCES           # utils/, streams/
VANILLAPDF_C_IMPLEMENTATION_*      # implementation/ bridge layer (utils/syntax/semantics/contents)
```

Public headers listed in `include/files.cmake`:
```cmake
VANILLAPDF_INCLUDE_HEADERS           # 6 core headers (c_export, c_handles, c_types, etc.)
VANILLAPDF_INCLUDE_UTILS_HEADERS     # 13 headers
VANILLAPDF_INCLUDE_SYNTAX_HEADERS    # 18 headers
VANILLAPDF_INCLUDE_SEMANTICS_HEADERS # 24 headers
VANILLAPDF_INCLUDE_CONTENTS_HEADERS  # 6 headers
```

### Adding New Source Files

| Task | Where to Edit |
|------|---------------|
| Add `.cpp` to library | `src/vanillapdf/CMakeLists.txt` → `VANILLAPDF_*_SOURCES` variable |
| Add public header | `include/files.cmake` → `VANILLAPDF_INCLUDE_*_HEADERS` variable |
| Add C API wrapper | `src/vanillapdf/CMakeLists.txt` → `VANILLAPDF_C_IMPLEMENTATION_*` variable |
| Add unit test file | `src/vanillapdf.unittest/CMakeLists.txt` → `VANILLAPDF_UNITTEST_SOURCES` |
| Add integration test | Place `.pdf` in `test/` directory (auto-discovered via `file(GLOB_RECURSE)`) |

## CMake Configuration Options

### Feature Enable/Disable

```cmake
VANILLAPDF_ENABLE_ENCRYPTION    # ON/OFF (default ON) → VANILLAPDF_HAVE_OPENSSL
VANILLAPDF_ENABLE_JPEG          # ON/OFF (default ON) → VANILLAPDF_HAVE_JPEG
VANILLAPDF_ENABLE_JPEG2000      # ON/OFF (default ON) → VANILLAPDF_HAVE_OPENJPEG
VANILLAPDF_ENABLE_ZLIB          # ON/OFF (default ON) → VANILLAPDF_HAVE_ZLIB
```

Pattern in CMakeLists.txt:
```cmake
if (VANILLAPDF_ENABLE_ENCRYPTION AND OPENSSL_FOUND)
    target_link_libraries(vanillapdf PRIVATE OpenSSL::Crypto)
    target_compile_definitions(vanillapdf PRIVATE -DVANILLAPDF_HAVE_OPENSSL)
endif ()
```

### Build Options

- `VANILLAPDF_INTERNAL_VCPKG=ON/OFF` (default ON) - Use internal vcpkg
- `VANILLAPDF_ENABLE_PACKAGING=ON/OFF` (auto-detected) - CPack packaging
- `VANILLAPDF_ENABLE_TESTS=ON/OFF` (default ON) - Build tests
- `VANILLAPDF_ENABLE_BENCHMARK=ON/OFF` (default ON) - Build benchmarks
- `VANILLAPDF_SKIP_CMAKE_CONFIG_INSTALL=ON/OFF` (default OFF)
- `VANILLAPDF_USE_STATIC_CRT=ON/OFF` (default OFF) - Static MSVC runtime
- `BUILD_SHARED_LIBS=ON/OFF` (default ON) - Shared vs static library
- `VANILLAPDF_ENABLE_COVERAGE=ON` - Code coverage (GCC/Clang)
- `VANILLAPDF_FORCE_32_BIT=ON` - Force 32-bit binary
- `VANILLAPDF_ENABLE_STACK_SANITIZER=ON` - Address sanitizer

### External Dependency Options (all default OFF)

`VANILLAPDF_EXTERNAL_{OPENSSL,JPEG,OPENJPEG,ZLIB,SPDLOG,NLOHMANN_JSON}` - Use system instead of vcpkg.

### Symbol Export (Shared Library)

Defined in CMakeLists.txt when `BUILD_SHARED_LIBS=ON`:
- `VANILLAPDF_EXPORTS` (PRIVATE) - Library exports symbols
- `VANILLAPDF_CONFIGURATION_DLL` (PUBLIC) - Consumers import symbols

## Precompiled Header

File: `src/vanillapdf/precompiled.h` - Includes:
- `c_platform.h` (platform detection)
- Debug memory tracking on MSVC (`_CRTDBG_MAP_ALLOC`, `pdf_new` macro)
- `utils/deferred.h`, `utils/constants.h`, `utils/exceptions.h`, `utils/log.h`, `utils/util.h`
- `syntax/utils/object_utils.h`, `syntax/objects/objects.h`

Setup: `target_precompile_headers(vanillapdf PRIVATE precompiled.h)`

## vcpkg Dependencies

Initialize submodules first:
```bash
git submodule sync --recursive && git submodule update --init --recursive
```

### vcpkg.json Manifest

Generated from `vcpkg.json.in` at configure time by `cmake/vcpkg_manifest.cmake`.

**Always included**: spdlog, nlohmann-json

**Feature-based** (enabled by CMake options):
- `encryption` → openssl
- `jpeg` → libjpeg-turbo
- `jpeg2000` → openjpeg
- `zlib` → zlib
- `tests` → gtest
- `benchmarks` → benchmark

## Conan Dependencies

Recipe in `conan/conanfile.py`. Build:
```bash
pip install "conan>=2,<3"
conan profile detect
conan create conan/ --version=2.3.0 --build=missing
```

## Version Management

Set in `cmake/version.cmake`. CI can override via `VANILLAPDF_VERSION_{MAJOR,MINOR,PATCH}_OVERRIDE`. Build suffix: `-nightly.main` for pre-release.

## CMake Module Files

| File | Purpose |
|------|---------|
| `cmake/compiler_flags.cmake` | Warning levels and flag settings |
| `cmake/compiler_checks.cmake` | C++17 feature detection |
| `cmake/coverage.cmake` | Code coverage configuration |
| `cmake/sanitizers.cmake` | Address sanitizer setup |
| `cmake/vcpkg_manifest.cmake` | vcpkg.json template generation |
| `cmake/vcpkg_init.cmake` | vcpkg initialization and feature management |
| `cmake/version.cmake` | Version number definitions |
| `cmake/packaging.cmake` | CPack and distribution packaging |
| `cmake/vanillapdf_install.cmake` | Installation target definitions |
| `cmake/generate_test_certificates.cmake` | Test certificate generation |

## CLI Tools

```bash
./vanillapdf-tools sign -s input.pdf -d signed.pdf -k private_key.p12 -p password
./vanillapdf-tools verify -f signed.pdf --skip-certificate-validation
./vanillapdf-tools --help
```

## Integration Testing

### FetchContent (`examples/fetchcontent-integration/`)

```bash
cd examples/fetchcontent-integration
cmake --preset windows-x64-debug  # or linux-x64-debug, macos-arm64-debug
cmake --build --preset windows-x64-debug
ctest --preset windows-x64-debug --output-on-failure
```

Platform strategies: Windows (internal vcpkg), Linux (apt packages), macOS (Homebrew).

### Conan (`examples/conan-integration/`)

```bash
conan create conan/ --version=2.3.0 --build=missing
cd examples/conan-integration
conan install . --build=missing
cmake --preset linux-x64-debug && cmake --build --preset linux-x64-debug
ctest --preset linux-x64-debug --output-on-failure
```
