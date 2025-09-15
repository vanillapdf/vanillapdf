# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

### Using CMake Presets (Recommended)

List available presets:
```bash
cmake --list-presets
```

#### CMake Presets Structure

Presets are organized by platform in separate files:
- `cmake/presets/windows.json` - Visual Studio and Ninja generators
- `cmake/presets/linux.json` - GCC and Clang compilers
- `cmake/presets/macos.json` - AppleClang for x64 and ARM64
- `cmake/presets/android.json` - NDK toolchain for all Android ABIs

Each preset includes configure, build, and test configurations.

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

#### Windows Build Notes

Windows presets automatically configure:
- Static CRT linking (`VANILLAPDF_USE_STATIC_CRT=ON`)
- Platform-specific vcpkg triplets (x86-windows, x64-windows)
- Visual Studio generators (2019/2022) or Ninja

For Ninja builds on Windows, ensure you have:
- Visual Studio Build Tools or full Visual Studio installation
- Ninja build system in PATH

### vcpkg Dependencies

The project uses vcpkg for dependency management. Initialize submodules first:
```bash
git submodule sync --recursive
git submodule update --init --recursive
```

#### vcpkg Features

VanillaPDF uses vcpkg's feature system to control optional dependencies. Available features:

- `openssl` - Enable encryption and decryption of secure PDF documents
- `libjpeg-turbo` - Decode JPEG images into bitmaps
- `openjpeg` - Support JPEG‑2000 images through the OpenJPEG codec
- `zlib` - Decompress PDF objects compressed with zlib
- `spdlog` - High-performance logging for diagnostics and debugging
- `nlohmann-json` - Parse application configuration from JSON files
- `tests` - Enable unit and integration tests (includes gtest)
- `benchmarks` - Enable performance benchmarking tools (includes google benchmark)

Enable features with vcpkg install:
```bash
vcpkg install vanillapdf[openssl,zlib,spdlog]
```

### CMake Configuration Options

Important build configuration options available:

- `-DVANILLAPDF_STANDALONE=ON/OFF` - Enable internal vcpkg setup for standalone builds (default: ON)
- `-DVANILLAPDF_ENABLE_TESTS=ON/OFF` - Perform test scenarios (default: ON)
- `-DVANILLAPDF_ENABLE_BENCHMARK=ON/OFF` - Include benchmarking project (default: ON)
- `-DVANILLAPDF_USE_STATIC_CRT=ON/OFF` - Use static MSVC runtime (/MT) instead of dynamic (/MD) (default: OFF)
- `-DBUILD_SHARED_LIBS=ON/OFF` - Build vanillapdf as a shared library (default: ON)
- `-DVANILLAPDF_ENABLE_COVERAGE=ON` - Enable code coverage instrumentation (for GCC/Clang only)
- `-DVANILLAPDF_FORCE_32_BIT=ON` - Force 32-bit output binary regardless of architecture
- `-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON` - Enable address sanitizer for memory safety testing
- `-DVANILLAPDF_EXTERNAL_*` - Use system dependencies instead of vcpkg (e.g., `-DVANILLAPDF_EXTERNAL_OPENSSL=ON`)

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

#### Code Coverage

Enable code coverage (GCC/Clang only):
```bash
cmake --preset linux-x64-gcc -DVANILLAPDF_ENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build --preset linux-x64-gcc
ctest --preset linux-x64-gcc
gcovr -r . --xml-pretty -o coverage.xml  # Generate coverage report
```

Coverage reports are automatically generated in CI and uploaded to Codecov.

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

### Detailed Architecture

**src/vanillapdf/syntax/** - PDF syntax layer
- `objects/` - PDF object implementations (arrays, dictionaries, streams, etc.)
- `parsers/` - Tokenizer, parser, and reverse parser implementations
- `files/` - File I/O and PDF file structure handling
- `filters/` - PDF filter implementations (compression, encoding)
- `exceptions/` - Syntax-level exception handling
- `utils/` - Syntax utility functions

**src/vanillapdf/contents/** - Content stream processing
- Content stream parser and operations
- Character map parsing and handling
- Content stream instruction definitions

**src/vanillapdf/semantics/** - High-level PDF semantics
- Document structure and page management
- Form handling and interactive elements
- Metadata and document properties

**src/vanillapdf/implementation/** - C interface layer
- C wrappers for C++ functionality
- Maintains ABI compatibility

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

## CI/CD Workflows

The project includes several GitHub Actions workflows:

- `nightly-check.yml` - Full platform matrix testing (Linux, Windows, macOS, Android)
- `coverage.yml` - Code coverage analysis with Codecov integration
- `stack-sanitizer.yml` - Address sanitizer testing for memory safety
- `codeql.yml` - Security analysis with GitHub CodeQL
- `build-nuget.yml` / `build-deb-package.yml` / `build-brew-package.yml` - Package building
- `github-pages.yml` - Documentation deployment
- `update-vcpkg.yml` - Automated monthly vcpkg updates
- `create-vcpkg-pr.yml` - Manual vcpkg update workflow

Builds are tested on:
- Windows: 2022, 2025 (x86/x64, MSVC 17)
- Linux: Ubuntu 22.04/24.04, Rocky 8/9, Fedora 41/42 (x64/ARM64)
- macOS: 13 (x64), 14/15 (ARM64)
- Android: arm64, armv7, x86, x86_64

### Development Tools

#### Visual Studio Debugging Support
- `.natvis` files provide custom visualizations for PDF objects in Visual Studio debugger
- `public.natvis` - Public API object visualizations
- `vanillapdf.natvis` - Internal object visualizations

#### Precompiled Headers
- `precompiled.h/cpp` - Speeds up compilation by pre-compiling common headers
- Automatically included in all source files

#### Resource Files
- `resources.rc` - Windows resource definitions for version information
- `unistd.h` - POSIX compatibility header for Windows builds

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
- For all changes in the vanillapdf repository we need to create a new branch and pull request. This is mandated by repository permissions
- When submitting new change to the repository, make sure we are on the correct branch. Most of the time this is main branch, however in case of hotfixes it could be release branch as well. We are not able to commit directly to main or release branches as they are protected. Please make sure you check this before making a commit and push in git.
- The default branch is main. master exists only for historical purposes and will be eventually removed.

## Troubleshooting

### Common Issues

#### vcpkg Bootstrap Fails
```bash
# Ensure submodules are initialized
git submodule sync --recursive
git submodule update --init --recursive
```

#### macOS JPEG Conflicts
The project includes a workaround for JPEG library conflicts on macOS (see GitHub issue #125):
```bash
brew unlink jpeg jpeg-turbo libjpeg 2>/dev/null || true
```

#### Windows Debug Build Issues
Some x86 debug configurations may have issues. Use Release builds for x86 on Windows if encountering problems.

#### Missing System Dependencies
For external dependency builds, ensure system packages are installed:
```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev libjpeg-turbo8-dev zlib1g-dev

# macOS
brew install openssl libjpeg-turbo
```

#### Build Configuration Issues
If CMake configuration fails, try cleaning the build directory:
```bash
rm -rf build/
cmake --preset your-preset
```

#### Test Failures
If tests fail unexpectedly:
1. Ensure all dependencies are properly installed
2. Check that the correct preset is being used for your platform
3. Run tests with verbose output: `ctest --preset your-preset --verbose`
4. Check for memory issues with sanitizers: `-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON`