# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Quick Reference

```bash
# Configure and build (Windows)
cmake --preset windows-x64-msvc-17
cmake --build --preset windows-x64-msvc-17

# Configure and build (Linux/macOS)
cmake --preset linux-x64-gcc      # or macos-arm64
cmake --build --preset linux-x64-gcc

# Run all tests (Windows - must specify build config)
ctest --preset windows-x64-msvc-17 --build-config Debug

# Run specific test
ctest --preset windows-x64-msvc-17 --build-config Debug -R "TestName" --output-on-failure

# Initialize submodules (required before first build)
git submodule sync --recursive && git submodule update --init --recursive
```

## Project Naming Conventions

- **Official/Marketing**: "Vanilla.PDF" (with dot) - documentation, user-facing messages
- **Code/Technical**: "vanillapdf" (lowercase, no dot) - code, commits, technical discussions
- **Avoid**: "VanillaPDF" (mixed case without dot)

## Repository Workflow (CRITICAL)

**Branch Targets:**
- **Default branch**: `main` - ALL pull requests should target this branch
- **Historical reference**: `master` - legacy branch for historical reference ONLY
- When using `gh pr create`, always specify `--base main` or omit the base flag (defaults to main)

**ALL changes MUST go through a branch and pull request:**
- NEVER commit directly to `main` or `release/*` branches (protected)
- Create a new branch for every change: `feature/description` or `fix/description`
- Base branches on `main` (or `release/*` for hotfixes)

## Automation Bot

The repository uses `vanillapdf-bot` (info@vanillapdf.com) for automated operations:
- Monthly vcpkg updates, release automation, vcpkg PRs to Microsoft

When creating GitHub Actions workflows that commit or create PRs:
```yaml
- name: Configure Git
  run: |
    git config --global user.name "vanillapdf-bot"
    git config --global user.email "info@vanillapdf.com"
```

## vcpkg Submodule Management

**NEVER modify `external/vcpkg/`** - it's a Git submodule updated monthly by vanillapdf-bot.

For vcpkg port development, work in `ports/vanillapdf/` (not `external/vcpkg/ports/`).

## Release Process

**Branch Structure:**
- `main` - Development branch (default)
- `release/X.Y` - Release branches for major.minor versions
- Tags: `vX.Y.Z` format on release branches

**Workflow:**
- Major/Minor: Create `release/X.Y` from `main`
- Patch: Work on existing `release/X.Y`
- Hotfixes: Branch from release branch if needed

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

#### Windows Build Notes

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

### Conan Dependencies

The project also supports Conan 2 for dependency management. The Conan recipe is in `conan/`.

#### Building with Conan

```bash
# Install Conan 2
pip install "conan>=2,<3"

# Detect default profile
conan profile detect

# Create the vanillapdf package locally
conan create conan/ --version=2.3.0 --build=missing

# Or build without packaging (for development)
cd conan
conan install . --build=missing
cmake --preset <your-preset>
cmake --build --preset <your-preset>
```

### CMake Configuration Options

Important build configuration options available:

#### Auto-Detection of Build Configuration

VanillaPDF automatically detects the appropriate configuration based on usage context:

**Packaging (`VANILLAPDF_ENABLE_PACKAGING`):**
- `ON`: Main project builds → Enable CPack packaging features
- `OFF`: Dependency usage → Disable packaging to avoid conflicts

**Usage Scenarios:**
```cmake
# Standalone development (default settings)
git clone vanillapdf && cmake --preset windows-x64-msvc-17
# → INTERNAL_VCPKG=ON, ENABLE_PACKAGING=ON

# FetchContent usage (packaging auto-disabled)
FetchContent_Declare(vanillapdf ...)
# → INTERNAL_VCPKG=ON, ENABLE_PACKAGING=OFF

# vcpkg port or external dependency management
vcpkg install vanillapdf
# → INTERNAL_VCPKG=OFF (set by portfile), ENABLE_PACKAGING=OFF

# Manual override to disable internal vcpkg
cmake --preset windows-x64-msvc-17 -DVANILLAPDF_INTERNAL_VCPKG=OFF
# → Use system or externally managed dependencies
```

#### Feature Enable/Disable Options
- `-DVANILLAPDF_ENABLE_ENCRYPTION=ON/OFF` - Enable PDF encryption/decryption support (default: ON)
- `-DVANILLAPDF_ENABLE_JPEG=ON/OFF` - Enable JPEG image support (default: ON)
- `-DVANILLAPDF_ENABLE_JPEG2000=ON/OFF` - Enable JPEG2000 image support (default: ON)

#### Build Configuration Options
- `-DVANILLAPDF_INTERNAL_VCPKG=ON/OFF` - Enable internal vcpkg dependency management (default: ON)
  - `ON`: Use internal vcpkg for dependency management (standalone, FetchContent, submodules)
  - `OFF`: Disable internal vcpkg when dependencies are managed externally (vcpkg ports, system packages)
- `-DVANILLAPDF_ENABLE_PACKAGING=ON/OFF` - Enable packaging features like CPack (auto-detected)
  - **Auto-detected default**: `ON` for main project builds, `OFF` for dependency usage
  - `ON`: Enable packaging (DEB, Brew, etc.) for distribution
  - `OFF`: Disable packaging to avoid conflicts in parent projects
- `-DVANILLAPDF_ENABLE_TESTS=ON/OFF` - Perform test scenarios (default: ON)
- `-DVANILLAPDF_ENABLE_BENCHMARK=ON/OFF` - Include benchmarking project (default: ON)
- `-DVANILLAPDF_SKIP_CMAKE_CONFIG_INSTALL=ON/OFF` - Skip installing CMake config files when an external package manager generates its own (default: OFF)
- `-DVANILLAPDF_USE_STATIC_CRT=ON/OFF` - Use static MSVC runtime (/MT) instead of dynamic (/MD) (default: OFF)
- `-DBUILD_SHARED_LIBS=ON/OFF` - Build vanillapdf as a shared library (default: ON)
- `-DVANILLAPDF_ENABLE_COVERAGE=ON` - Enable code coverage instrumentation (for GCC/Clang only)
- `-DVANILLAPDF_FORCE_32_BIT=ON` - Force 32-bit output binary regardless of architecture
- `-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON` - Enable address sanitizer for memory safety testing

#### External Dependency Options
- `-DVANILLAPDF_EXTERNAL_OPENSSL=ON/OFF` - Use system OpenSSL instead of vcpkg (default: OFF)
- `-DVANILLAPDF_EXTERNAL_JPEG=ON/OFF` - Use system libjpeg instead of vcpkg (default: OFF)
- `-DVANILLAPDF_EXTERNAL_OPENJPEG=ON/OFF` - Use system OpenJPEG instead of vcpkg (default: OFF)
- `-DVANILLAPDF_EXTERNAL_ZLIB=ON/OFF` - Use system zlib instead of vcpkg (default: OFF)
- `-DVANILLAPDF_EXTERNAL_SPDLOG=ON/OFF` - Use system spdlog instead of vcpkg (default: OFF)
- `-DVANILLAPDF_EXTERNAL_NLOHMANN_JSON=ON/OFF` - Use system nlohmann-json instead of vcpkg (default: OFF)

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

Run individual test cases with verbose output:
```bash
ctest --preset windows-x64-msvc-17 -R "objects_test" --verbose
ctest --preset windows-x64-msvc-17 -R "signature_verifier_test" --output-on-failure
```

**Windows (Visual Studio) specific**: Visual Studio is a multi-config generator, so you must specify build configuration:
```bash
# Specify configuration with --build-config
ctest --preset windows-x64-msvc-17 --build-config Debug -R "SignatureVerifier" --output-on-failure
ctest --preset windows-x64-msvc-17 --build-config Release -R "unittest"
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

### FetchContent Integration Testing

The project includes a comprehensive FetchContent integration example in `examples/fetchcontent-integration/` that demonstrates real-world usage patterns:

#### Running FetchContent Tests
```bash
cd examples/fetchcontent-integration
cmake --preset windows-x64-debug  # or linux-x64-debug, macos-arm64-debug
cmake --build --preset windows-x64-debug
ctest --preset windows-x64-debug --output-on-failure
```

#### Key Features
- **Real GitHub Integration**: Tests actual FetchContent from GitHub repository (not local source)
- **Cross-platform Testing**: Windows (vcpkg), Linux (apt), macOS (Homebrew) dependency strategies
- **CMake Test Integration**: Uses `add_test()` and `enable_testing()` for proper test execution
- **Automatic CI Testing**: Continuously validated via GitHub Actions workflow

#### Dependency Management Approaches
- **Windows**: Uses internal vcpkg for all dependencies
- **Linux**: Uses system packages (`apt-get install`) for speed
- **macOS**: Uses Homebrew packages (`brew install`) for speed

The example creates actual PDF files and validates the complete integration chain from dependency resolution through PDF creation.

### Conan Integration Testing

The project includes a Conan integration example in `examples/conan-integration/` that validates the Conan package:

#### Running Conan Integration Tests
```bash
# First, create the package in the local Conan cache
conan create conan/ --version=2.3.0 --build=missing

# Then build and test the example consumer project
cd examples/conan-integration
conan install . --build=missing
cmake --preset linux-x64-debug  # or windows-x64-debug, macos-arm64-debug
cmake --build --preset linux-x64-debug
ctest --preset linux-x64-debug --output-on-failure
```

#### Key Features
- **Local Package Testing**: Tests the Conan recipe from `conan/` directory
- **Cross-platform Testing**: Linux, Windows, macOS via CI
- **CMake Test Integration**: Uses `add_test()` and `enable_testing()` for proper test execution
- **Automatic CI Testing**: Continuously validated via GitHub Actions `examples-integration.yml`

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
   - Public API: All `include/vanillapdf/c_*.h` headers provide ABI-stable C interface
   - Internal implementation: C++17 code in `src/vanillapdf/` (not exposed)
   - Bridge layer: `src/vanillapdf/implementation/` wraps C++ objects for C API
   - **Handle-based system**: C API uses opaque pointers (handles) defined in `c_handles.h`
     - Example: `VPDF_DOCUMENT`, `VPDF_FILE`, `VPDF_OBJECT` are all opaque handles
     - Internally cast to C++ objects in implementation layer
     - Ensures ABI stability across compiler versions
2. **Object-based PDF Model** - PDF syntax objects (arrays, dictionaries, streams) have corresponding C++ classes
3. **Parser-based Architecture** - Separate parsers for syntax (`syntax/parsers/`) and content streams (`contents/`)
4. **Interface Segregation** - Clean interfaces for encryption, signing, and streams in `utils/`
5. **Layered Architecture** - Three distinct layers:
   - **Syntax Layer**: Low-level PDF objects and file structure
   - **Semantics Layer**: High-level document concepts (pages, annotations, forms)
   - **Contents Layer**: Content stream parsing and operations

### Signature Verification Architecture

**Location**: `src/vanillapdf/utils/` (low-level utilities, independent of document parsing)

**Core Components**:
- `SignatureVerifier` - Static utility class for PKCS#7 signature verification
  - Operates on raw bytes (signed data + signature blob)
  - No dependency on PDF document structures
  - Uses OpenSSL for cryptographic operations
- `SignatureVerificationResult` - Detailed verification result object
  - Status (valid/invalid/expired/revoked/untrusted/etc.)
  - Certificate chain information
  - Signer common name extraction
- `TrustedCertificateStore` - Certificate store for chain validation
  - Load from PEM/DER format
  - Load from system defaults (Windows CertStore, Linux/macOS OpenSSL paths)
  - Load from directory (e.g., `/etc/ssl/certs`)
- `SigningKey` enhancements - Extended with direct signing methods
  - `Sign_Initialize/Update/Final/Cleanup` for PKCS#7 signature creation
  - Enables testing without full document context

**C API**: `include/vanillapdf/utils/c_signature_verifier.h`, `c_signing_key.h`

**Tests**: `src/vanillapdf.unittest/signature_verifier_test.cpp`
- Run with: `ctest --preset windows-x64-msvc-17 --build-config Debug -R "TrustedCertificateStore|SignatureVerifier|SignatureVerificationResult"`

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
- Do not insert structurally different code into a group of similarly-looking lines without separating it with a blank line and a comment. Uniform blocks (e.g., a series of assignments) should stay visually cohesive.

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
- `update-vcpkg.yml` - Automated monthly vcpkg updates (uses vanillapdf-bot)
- `create-vcpkg-pr.yml` - Manual vcpkg update workflow (uses vanillapdf-bot)
- `create-conan-pr.yml` - Conan Center Index PR workflow (uses vanillapdf-bot)
- `release.yml` - Release automation workflow (uses vanillapdf-bot)
- `backport.yml` - Automatic backporting of merged PRs to release branches

### Backporting PRs to Release Branches

To backport a merged PR to a release branch:

1. Add a label `backport release/X.Y` to the PR (e.g., `backport release/2.2`)
2. When the PR is merged, the backport workflow automatically:
   - Cherry-picks the commit to the target branch
   - Creates a new PR with title `[Backport release/X.Y] <original title>`
   - Adds `backported release/X.Y` label to the new PR
3. If cherry-pick fails (conflicts), manually create a backport PR

Available labels:
- `backport release/2.2` - Request backport to release/2.2 branch
- `backport release/2.1` - Request backport to release/2.1 branch
- `backported release/2.2` - PR was backported to release/2.2
- `backported release/2.1` - PR was backported to release/2.1

For new release branches, create labels following the patterns `backport <branch>` and `backported <branch>`.

**Automated Workflows Using vanillapdf-bot:**
- All vcpkg-related automation
- Release processes and package updates
- Monthly maintenance tasks
- Any workflow that creates commits or PRs automatically

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

## Maintenance Scripts

### Updating vcpkg Dependencies

Use the automated Python script to update vcpkg to the latest version:

```bash
# Check what would be updated (dry run)
python scripts/update_vcpkg.py --dry-run

# Update vcpkg and create PR
python scripts/update_vcpkg.py

# Update without creating PR
python scripts/update_vcpkg.py --no-pr

# Force update even if already on latest
python scripts/update_vcpkg.py --force
```

**Requirements:**
- Python 3.6+
- git command line
- gh CLI (for PR creation)

The script will:
1. Check current vs latest vcpkg versions
2. Create update branch if needed
3. Update vcpkg submodule and vcpkg.json baseline
4. Commit changes with descriptive message
5. Push branch and optionally create PR

### Updating Conan Data

Use the Python script to update `conan/conandata.yml` with a new version's SHA256:

```bash
# Auto-detect latest tag and update
python scripts/update_conandata.py

# Specific version
python scripts/update_conandata.py --version 2.3.0

# Dry run
python scripts/update_conandata.py --dry-run
```

**Requirements:**
- Python 3.6+
- git command line

The script will:
1. Download the release archive from GitHub
2. Calculate the SHA256 hash
3. Add or update the version entry in `conan/conandata.yml`

## Common Tasks

### Adding New PDF Object Type
1. Create C++ class in `src/vanillapdf/syntax/objects/` (e.g., `new_object.h`, `new_object.cpp`)
2. Add parser logic in `src/vanillapdf/syntax/parsers/`
3. Create C API header in `include/vanillapdf/syntax/c_new_object.h`
4. Implement C interface wrapper in `src/vanillapdf/implementation/syntax/c_new_object.cpp`
5. Add to main API header `include/vanillapdf/c_vanillapdf_api.h`
6. Add unit tests in `src/vanillapdf.unittest/objects_test.cpp`

### Adding Content Stream Operation
1. Define operation in `src/vanillapdf/contents/content_stream_operations.h`
2. Implement in `src/vanillapdf/contents/content_stream_operations.cpp`
3. Add parser support in `src/vanillapdf/contents/content_stream_parser.h`
4. Create C API header in `include/vanillapdf/contents/c_content_operation.h` (if needed)
5. Create C interface wrapper in `src/vanillapdf/implementation/contents/`
6. Add integration tests in `src/vanillapdf.test/`

### Adding New Semantic Object
1. Create C++ class in `src/vanillapdf/semantics/objects/` (e.g., `new_semantic.h`, `new_semantic.cpp`)
2. Inherit from `HighLevelObject` base class
3. Create C API header in `include/vanillapdf/semantics/c_new_semantic.h`
4. Implement C interface wrapper in `src/vanillapdf/implementation/semantics/`
5. Add to main API header `include/vanillapdf/c_vanillapdf_api.h`
6. Add tests in appropriate test directory

### Signature Verification

**Components** (in `src/vanillapdf/utils/`):
- `SignatureVerifier` - PKCS#7 signature verification (raw bytes)
- `SignatureVerificationResult` - Verification status and certificate chain
- `TrustedCertificateStore` - Certificate store for chain validation
- `DigitalSignatureExtensions::Verify` - Document-level verification

**Testing**:
```bash
ctest --preset windows-x64-msvc-17 --build-config Debug \
  -R "TrustedCertificateStore|SignatureVerifier|SignatureVerificationResult" \
  --output-on-failure
```

### Debugging and Development
- Use sanitizers in Debug builds: `-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON`
- Visual Studio .natvis files available for debugging C++ objects (`public.natvis`, `vanillapdf.natvis`)
- Precompiled headers are used (`precompiled.h`) for faster builds
- C API headers use handle-based system (opaque pointers) for ABI stability

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
