# Build System

## CRITICAL: Test Performance

- **NEVER run the full test suite (`ctest` without `-R`) in Debug mode** — it takes 40+ minutes on Windows.
- Use Debug only for targeted tests: `ctest --preset <preset> --build-config Debug -R "TestPattern" --output-on-failure`
- Use **Release** (`--build-config Release`) for full test suite runs.

## CMake Presets

Presets organized by platform in `cmake/presets/{windows,linux,macos,android}.json`, with shared base configs in `cmake/presets/shared.json`. Each includes configure, build, and test configurations. List with `cmake --list-presets`.

Common patterns: `windows-x64-msvc-17`, `windows-x64-msvc-17-static` (static CRT), `windows-x64-msvc-17-static-md` (static libs + dynamic CRT). Same variants for `msvc-18` (VS 2026). Linux: `linux-x64-gcc`, `linux-x64-clang`, `linux-x64-musl`. macOS: `macos-x64`, `macos-arm64`.

## Build Targets

| Target | Type | Language | Enabled By |
|--------|------|----------|------------|
| `vanillapdf` | STATIC/SHARED lib | C++17 | Always |
| `vanillapdf.tools` | Executable | C++ | `VANILLAPDF_ENABLE_TOOLS=ON` (default OFF; the presets turn it on; needs CLI11) |
| `vanillapdf.unittest` | Executable | C++ | `VANILLAPDF_ENABLE_TESTS=ON` |
| `vanillapdf.test` | Executable | C | `VANILLAPDF_ENABLE_TESTS=ON` |
| `vanillapdf.benchmark` | Executable | C++ | `VANILLAPDF_ENABLE_BENCHMARK=ON` |
| `vanillapdf.fuzzer` | Executable | C++ | `VANILLAPDF_ENABLE_FUZZING=ON` (Clang only) |

## Adding New Source Files

Source files use **explicit lists** (NOT `file(GLOB)`):

| Task | Where to Edit |
|------|---------------|
| Add `.cpp` to library | `src/vanillapdf/CMakeLists.txt` → `VANILLAPDF_*_SOURCES` variable |
| Add public header | `include/files.cmake` → `VANILLAPDF_INCLUDE_*_HEADERS` variable |
| Add C API wrapper | `src/vanillapdf/CMakeLists.txt` → `VANILLAPDF_C_IMPLEMENTATION_*` variable |
| Add unit test file | `src/vanillapdf.unittest/CMakeLists.txt` → `VANILLAPDF_UNITTEST_SOURCES` |
| Add integration test | Add the `.pdf` to the external `vanillapdf-testdata` corpus and its `manifest.json`, then re-cut the release archive and bump the URL/hash in `cmake/testdata.cmake`. Tests are enumerated from the manifest at configure time. |

## Key CMake Options

Features (all default ON): `VANILLAPDF_ENABLE_{ENCRYPTION,JPEG,JPEG2000,ZLIB}` → defines `VANILLAPDF_HAVE_*`

`VANILLAPDF_ENABLE_MIMALLOC` (OFF) → defines `VANILLAPDF_HAVE_MIMALLOC`: `IUnknown` serves its own `operator new`/`delete` from mimalloc, so every reference counted object is allocated there while the containers and every other module stay on the default allocator. Worth roughly 10% on a corpus run. Deliberately *not* a global `operator new` replacement — that is only correct when every C++ dependency is linked into the same module, and it crashes with the dynamic `x64-windows` triplet, where `fmt::vformat` returns a string the CRT allocated. It cannot be combined with `VANILLAPDF_ENABLE_CRTDBG`: the tracking placement form of `pdf_new` no longer resolves once `IUnknown` declares `operator new`, and the debug heap would hand out blocks that the mimalloc `operator delete` cannot release.

`VANILLAPDF_ENABLE_CRTDBG` (OFF) → defines `VANILLAPDF_HAVE_CRTDBG` in the Debug configuration only: `pdf_new` becomes the `new(_NORMAL_BLOCK, __FILE__, __LINE__)` form of the Microsoft debug heap, and `vanillapdf.test` / `vanillapdf.tools` call `_CrtCheckMemory()` before exiting and return `HEAP_CORRUPTION` when it fails. Requires a compiler targeting the Microsoft C runtime (MSVC or clang-cl) and is a configure error together with `VANILLAPDF_ENABLE_MIMALLOC`. Use a dynamic-CRT preset (`/MDd`): with the static CRT the library DLL has its own heap, which the executables cannot check. Covered by the `win.2022-x64-crtdbg` row of `nightly-check.yml`.

Build options: `VANILLAPDF_INTERNAL_VCPKG` (ON), `VANILLAPDF_ENABLE_TESTS` (OFF), `VANILLAPDF_ENABLE_TOOLS` (OFF) — every CMake preset sets both of these ON, `VANILLAPDF_ENABLE_BENCHMARK` (OFF), `VANILLAPDF_USE_STATIC_CRT` (OFF), `BUILD_SHARED_LIBS` (ON), `VANILLAPDF_ENABLE_COVERAGE`, `VANILLAPDF_ENABLE_ASAN`, `VANILLAPDF_ENABLE_UBSAN`, `VANILLAPDF_ENABLE_TSAN`

External deps (all OFF): `VANILLAPDF_EXTERNAL_{OPENSSL,JPEG,OPENJPEG,ZLIB,SPDLOG,NLOHMANN_JSON,MIMALLOC}` — use system instead of vcpkg.

OpenSSL: `VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER` (OFF) — load the OpenSSL 3 legacy provider at initialization for RC2 envelopes/key files and MD2/MD4 signatures. Intended for static OpenSSL (NuGet sets it); with a shared OpenSSL the provider module path must be supplied via `OPENSSL_MODULES`. RC4 and MD5 never need it (in-house).

Limits: `VANILLAPDF_MAX_IMAGE_SIZE` (`0x40000000`, 1 GiB) — largest single decoded image, in bytes, that `DCTDecode` and `JPXDecode` will allocate. Image dimensions come from the document and are independent of how much data backs them, so a few hundred bytes of image stream can otherwise ask for gigabytes. It is a build time constant rather than a runtime property, so callers are protected without having to know it exists; raise it with `-DVANILLAPDF_MAX_IMAGE_SIZE=<bytes>` for documents that legitimately carry larger images.

## vcpkg

vcpkg.json generated from `vcpkg.json.in` at configure time by `cmake/vcpkg_manifest.cmake`. Always included: spdlog, nlohmann-json. Feature-based: encryption→openssl, jpeg→libjpeg-turbo, jpeg2000→openjpeg, zlib→zlib, mimalloc→mimalloc, tests→gtest, benchmarks→benchmark.

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
