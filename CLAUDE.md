# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Using CMake Presets (Recommended)

List available presets:
```bash
cmake --list-presets
```

Common presets include:
- `windows-x64-msvc-17` / `windows-x86-msvc-17` - Windows with Visual Studio 2022
- `windows-x64-msvc-16` / `windows-x86-msvc-16` - Windows with Visual Studio 2019
- `windows-x64-ninja` / `windows-x86-ninja` - Windows with Ninja generator
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

### vcpkg Dependencies

The project uses vcpkg for dependency management. Initialize submodules first:
```bash
git submodule sync --recursive
git submodule update --init --recursive
```

### Running Tests

Enable tests with `-DVANILLAPDF_ENABLE_TESTS=ON` (enabled by default) and run:
```bash
ctest --preset windows-x64-msvc-17
```

Run specific test suites:
```bash
ctest --preset windows-x64-msvc-17 -R "unittest"     # Unit tests only
ctest --preset windows-x64-msvc-17 -R "test"         # Integration tests only
ctest --preset windows-x64-msvc-17 -R "benchmark"    # Benchmarks only
```

### CLI Tools

Build and use the `vanillapdf-tools` CLI utility:
```bash
# Sign a PDF
./vanillapdf-tools sign -s input.pdf -d signed.pdf -k private_key.p12 -p password

# Get help for available commands
./vanillapdf-tools --help
```

## Architecture Overview

### Core Structure

- **src/vanillapdf/** - Main library implementation
  - **syntax/** - Low-level PDF syntax parsing and objects
  - **semantics/** - High-level PDF document semantics
  - **contents/** - Content stream parsing and operations
  - **utils/** - Utility classes and interfaces
  - **implementation/** - C interface implementations

- **src/vanillapdf.tools/** - CLI utility implementation (C code)
- **src/vanillapdf.test/** - Integration tests
- **src/vanillapdf.unittest/** - Unit tests
- **src/vanillapdf.benchmark/** - Performance benchmarks

### Key Design Patterns

1. **C++ Core with C Interface** - Core library is C++17, exposed via C interface in `implementation/`
2. **Object-based PDF Model** - PDF syntax objects (arrays, dictionaries, streams) have corresponding C++ classes
3. **Parser-based Architecture** - Separate parsers for syntax (`syntax/parsers/`) and content streams (`contents/`)
4. **Interface Segregation** - Clean interfaces for encryption, signing, and streams in `utils/`

### Feature Dependencies

Optional features controlled by vcpkg.json and CMake options:
- OpenSSL: PDF encryption/decryption and digital signing
- libjpeg-turbo: JPEG image support
- openjpeg: JPEG 2000 support
- zlib: PDF object compression
- spdlog: Logging framework
- nlohmann-json: Configuration parsing

Use `VANILLAPDF_EXTERNAL_*` CMake options to use system dependencies instead of vcpkg.

## Development Practices

### Code Style

- C++17 standard with extensions disabled
- Uses precompiled headers (`precompiled.h`)
- Visual Studio .natvis files for debugging support
- Follow existing patterns in similar classes

### Testing Strategy

- Unit tests in `src/vanillapdf.unittest/`
- Integration tests in `src/vanillapdf.test/`
- Benchmarks in `src/vanillapdf.benchmark/`
- Sanitizers available with `-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON`

### Contribution Guidelines

- All commits must be signed off with `git commit -s`
- Base PRs on `main` branch
- Match existing coding style
- Test changes with available presets
- See CONTRIBUTING.md for full guidelines

## Common Tasks

### Adding New PDF Object Type
1. Create class in `src/vanillapdf/syntax/objects/`
2. Add parser logic in `src/vanillapdf/syntax/parsers/`
3. Implement C interface in `src/vanillapdf/implementation/syntax/`
4. Add tests in appropriate test directory

### Adding Content Stream Operation
1. Define operation in `src/vanillapdf/contents/content_stream_operations.h`
2. Implement in `src/vanillapdf/contents/content_stream_operations.cpp`
3. Add parser support in `src/vanillapdf/contents/content_stream_parser.h`
4. Create C interface in `src/vanillapdf/implementation/contents/`

### Debugging and Development
- Use sanitizers in Debug builds: `-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON`
- Visual Studio .natvis files available for debugging C++ objects
- Precompiled headers are used (`precompiled.h`) for faster builds