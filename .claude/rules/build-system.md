# Build System

## CRITICAL: Test Performance

- **NEVER run the full test suite (`ctest` without `-R`) in Debug mode** — it takes 40+ minutes on Windows.
- Use Debug only for targeted tests: `ctest --preset <preset> --build-config Debug -R "TestPattern" --output-on-failure`
- Use **Release** (`--build-config Release`) for full test suite runs.

## CMake Presets

Presets organized by platform in `cmake/presets/{windows,linux,macos,android}.json`. Each includes configure, build, and test configurations. List with `cmake --list-presets`.

Common patterns: `windows-x64-msvc-17`, `windows-x64-msvc-17-static` (static CRT), `windows-x64-msvc-17-static-md` (static libs + dynamic CRT). Same variants for `msvc-18` (VS 2026). Linux: `linux-x64-gcc`, `linux-x64-clang`, `linux-x64-musl`. macOS: `macos-x64`, `macos-arm64`.

## Build Targets

| Target | Type | Language | Enabled By |
|--------|------|----------|------------|
| `vanillapdf` | STATIC/SHARED lib | C++17 | Always |
| `vanillapdf.tools` | Executable | C | Always |
| `vanillapdf.unittest` | Executable | C++ | `VANILLAPDF_ENABLE_TESTS=ON` |
| `vanillapdf.test` | Executable | C | `VANILLAPDF_ENABLE_TESTS=ON` |
| `vanillapdf.benchmark` | Executable | C++ | `VANILLAPDF_ENABLE_BENCHMARK=ON` |

## Adding New Source Files

Source files use **explicit lists** (NOT `file(GLOB)`):

| Task | Where to Edit |
|------|---------------|
| Add `.cpp` to library | `src/vanillapdf/CMakeLists.txt` → `VANILLAPDF_*_SOURCES` variable |
| Add public header | `include/files.cmake` → `VANILLAPDF_INCLUDE_*_HEADERS` variable |
| Add C API wrapper | `src/vanillapdf/CMakeLists.txt` → `VANILLAPDF_C_IMPLEMENTATION_*` variable |
| Add unit test file | `src/vanillapdf.unittest/CMakeLists.txt` → `VANILLAPDF_UNITTEST_SOURCES` |
| Add integration test | Place `.pdf` in `test/` directory (auto-discovered via `file(GLOB_RECURSE)`) |

## Key CMake Options

Features (all default ON): `VANILLAPDF_ENABLE_{ENCRYPTION,JPEG,JPEG2000,ZLIB}` → defines `VANILLAPDF_HAVE_*`

Build options: `VANILLAPDF_INTERNAL_VCPKG` (ON), `VANILLAPDF_ENABLE_TESTS` (ON), `VANILLAPDF_ENABLE_BENCHMARK` (ON), `VANILLAPDF_USE_STATIC_CRT` (OFF), `BUILD_SHARED_LIBS` (ON), `VANILLAPDF_ENABLE_COVERAGE`, `VANILLAPDF_ENABLE_ASAN`, `VANILLAPDF_ENABLE_UBSAN`, `VANILLAPDF_ENABLE_TSAN`

External deps (all OFF): `VANILLAPDF_EXTERNAL_{OPENSSL,JPEG,OPENJPEG,ZLIB,SPDLOG,NLOHMANN_JSON}` — use system instead of vcpkg.

Runtime options: `VANILLAPDF_OPENSSL_MODULES_DIR` — path to OpenSSL provider modules (`legacy.dll`). Auto-set to vcpkg bin dir on Windows shared builds with internal vcpkg. Override with `-DVANILLAPDF_OPENSSL_MODULES_DIR=<path>`.

## vcpkg

vcpkg.json generated from `vcpkg.json.in` at configure time by `cmake/vcpkg_manifest.cmake`. Always included: spdlog, nlohmann-json. Feature-based: encryption→openssl, jpeg→libjpeg-turbo, jpeg2000→openjpeg, zlib→zlib, tests→gtest, benchmarks→benchmark.

## CMake Module Files

| File | Purpose |
|------|---------|
| `cmake/compiler_flags.cmake` | Warning levels and flags |
| `cmake/compiler_checks.cmake` | C++17 feature detection |
| `cmake/coverage.cmake` | Code coverage |
| `cmake/sanitizers.cmake` | ASan, UBSan, TSan sanitizers |
| `cmake/vcpkg_manifest.cmake` | vcpkg.json generation |
| `cmake/vcpkg_init.cmake` | vcpkg initialization |
| `cmake/version.cmake` | Version numbers (CI overrides via `VANILLAPDF_VERSION_*_OVERRIDE`) |
| `cmake/packaging.cmake` | CPack packaging |
| `cmake/vanillapdf_install.cmake` | Install targets |
