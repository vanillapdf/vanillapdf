# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 📝 Project Naming Conventions

**CRITICAL**: Use correct naming conventions throughout all work:

- **Official/Marketing Communications**: "Vanilla.PDF" (with dot, proper case)
  - Use in: Documentation, user-facing messages, website content, official guides
- **Code/Technical Contexts**: "vanillapdf" (lowercase, no dot, no quotes)
  - Use in: Code comments, variable names, technical discussions, commit messages
- **❌ INCORRECT**: "VanillaPDF" (avoid this mixed case without dot)

Examples:
```cmake
# Correct in user-facing message:
message(STATUS "Vanilla.PDF dependency management enabled")

# Correct in code comment:
# Configure vanillapdf build options
```

## ⚠️ CRITICAL: Repository Workflow Requirements

**ALWAYS CREATE BRANCH AND PULL REQUEST FOR ALL CHANGES**
- Repository permissions mandate that ALL changes must go through a branch and pull request workflow
- NEVER commit directly to main or release branches - they are protected
- Create a new branch for every change, no matter how small
- Always create a PR for review before merging

## 🤖 Automation and Bot Information

### vanillapdf-bot
The repository uses `vanillapdf-bot` for automated operations:

**Bot Identity:**
- Name: `vanillapdf-bot`
- Email: `info@vanillapdf.com`
- Used for all automated commits and operations

**Bot Duties:**
- **Monthly vcpkg Updates**: Automatically updates vcpkg submodule and baseline (1st of each month)
- **Release Automation**: Creates automated PRs to Microsoft vcpkg repository for new releases
- **Commit Signing**: All automated commits are signed with the bot identity
- **Workflow Consistency**: Ensures consistent authorship across all automated processes

**When to Use Bot Identity:**
- Any automated GitHub Actions workflows
- Scheduled maintenance tasks
- Release automation processes
- vcpkg-related automated updates

**Bot Repositories:**
- `vanillapdf-bot/vcpkg` - Fork used for creating PRs to Microsoft vcpkg

**Claude Code Guidelines for Automation:**
- When creating GitHub Actions workflows that commit or create PRs, ALWAYS use vanillapdf-bot identity
- Use the following git config in workflows:
  ```yaml
  - name: Configure Git
    run: |
      git config --global user.name "vanillapdf-bot"
      git config --global user.email "info@vanillapdf.com"
  ```
- Include Co-Authored-By line in automated commit messages:
  ```
  Co-Authored-By: vanillapdf-bot <info@vanillapdf.com>
  ```
- Never use `github-actions[bot]` or similar generic bot names

## ⚠️ CRITICAL: vcpkg Submodule Management

**NEVER MODIFY external/vcpkg FOLDER**
- The `external/vcpkg` folder is a Git submodule pointing to Microsoft's vcpkg repository
- **NEVER** make direct changes to files in `external/vcpkg/` - these will be lost on submodule updates
- The submodule is automatically updated monthly by vanillapdf-bot

**vcpkg Port Development Workflow:**
- Use `ports/vanillapdf/` folder for port development and testing
- This folder mirrors the structure that will be submitted to Microsoft vcpkg
- When ready for release, the vanillapdf-bot creates PRs to Microsoft vcpkg repository using content from `ports/vanillapdf/`
- Port files in `ports/vanillapdf/`:
  - `vcpkg.json` - Port manifest with features and dependencies
  - `portfile.cmake` - Build instructions and feature configuration
  - `usage` - Installation and usage documentation

**For Claude Code:**
- Always work in `ports/vanillapdf/` when updating vcpkg port files
- Never edit anything in `external/vcpkg/ports/vanillapdf/` - it will be overwritten
- Test port changes by copying from `ports/vanillapdf/` to a local vcpkg installation if needed

## 🚀 Release Process

### Release Branch Strategy
VanillaPDF follows a structured release branch model:

**Branch Structure:**
- `main` - Development branch (default)
- `release/X.Y` - Release branches for major.minor versions (e.g., `release/2.1`)
- Release branches contain patch versions (e.g., `2.1.0`, `2.1.1`, `2.1.2`)

**Version Tagging:**
- All versions are tagged in git with semantic versioning
- Tags follow the format: `vX.Y.Z` (e.g., `v2.1.0`, `v2.1.1`)
- Tags are created on the appropriate release branch

**Release Workflow:**
1. **Major/Minor Releases**: Create new `release/X.Y` branch from `main`
2. **Patch Releases**: Work directly on existing `release/X.Y` branch
3. **Hotfixes**: May branch from release branch if urgent fixes needed
4. **Tagging**: Create git tags for all releases
5. **Automation**: Release process triggers automated workflows including:
   - Package building (NuGet, Deb, Brew)
   - vcpkg port updates via vanillapdf-bot
   - GitHub release creation
   - Documentation updates

**For Claude Code:**
- When working on hotfixes, check if you should base your branch on a release branch instead of main
- Always verify the target branch before creating PRs for release-related work
- Release-related commits should follow the same branch and PR workflow

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
- Platform-specific vcpkg triplets:
  - `x64-windows-static-md` (x64, static CRT /MD) - recommended for most use cases
  - `x86-windows-static-md` (x86, static CRT /MD)
  - Avoids CRT mismatch issues in mixed static/dynamic linking scenarios
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
- `update-vcpkg.yml` - Automated monthly vcpkg updates (uses vanillapdf-bot)
- `create-vcpkg-pr.yml` - Manual vcpkg update workflow (uses vanillapdf-bot)
- `release.yml` - Release automation workflow (uses vanillapdf-bot)

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

**🚨 MANDATORY: Branch and PR Workflow**
- **ALWAYS** create a new branch and pull request for ALL changes - this is mandated by repository permissions
- **NEVER** commit directly to main or release branches (they are protected)
- Base new branches on `main` (default branch, `master` is legacy)
- Check current branch before making commits: `git branch --show-current`
- For hotfixes, may need to branch from release branch instead of main

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

## Recent Improvements

### FetchContent Integration Enhancements
- **Fixed vcpkg triplet issues**: Resolved CRT mismatch problems by using `x64-windows-static-md` instead of `x64-windows-static`
- **Enhanced FetchContent example**: Added comprehensive real-world integration testing in `examples/fetchcontent-integration/`
- **Cross-platform CI testing**: Automated testing on Windows (vcpkg), Linux (apt), macOS (Homebrew)
- **CMake test integration**: Proper test execution using `add_test()` and CTest framework
- **Simplified workflow paths**: Eliminated complex path detection logic in favor of CMake-managed execution
- **Workflow organization**: GitHub Actions workflows now use consistent naming and concurrency controls

### Build System Improvements
- **vcpkg triplet standardization**: All Windows builds now use Microsoft's official triplets
- **Debug message cleanup**: Removed temporary debugging output from development
- **Dependency flexibility**: Enhanced support for both system packages and vcpkg dependencies
- **Documentation updates**: Updated README with clearer integration guidance (vcpkg recommended, FetchContent as alternative)

## GitHub Issue Management

### Available Labels

When creating GitHub issues, use these labels for proper categorization:

**Issue Types:**
- `bug` - Something isn't working
- `enhancement` - New feature or request
- `documentation` - Improvements or additions to documentation
- `question` - Further information is requested

**Build System & Dependencies:**
- `cmake` - CMake configuration and build system issues
- `build-system` - General build system improvements
- `fetchcontent` - FetchContent integration issues
- `vcpkg` - vcpkg dependency management
- `dependencies` - Dependency updates

**Development & Quality:**
- `technical-debt` - Code quality and refactoring issues
- `performance` - Performance improvements
- `compatibility` - Platform/compiler compatibility issues
- `ci-cd` - Continuous integration and deployment
- `github_actions` - GitHub Actions workflow updates

**Priority Levels:**
- `priority-high` - High priority issues (critical bugs, blocking issues)
- `priority-medium` - Medium priority issues (important improvements)
- `priority-low` - Low priority issues (nice-to-have features)

**Community:**
- `good first issue` - Good for newcomers
- `help wanted` - Extra attention is needed

**Workflow:**
- `duplicate` - This issue or pull request already exists
- `invalid` - This doesn't seem right
- `wontfix` - This will not be worked on

### Label Usage Guidelines

**For Build System Issues:**
- Use `cmake` + `build-system` for CMake-specific problems
- Add `fetchcontent` for FetchContent integration issues
- Add `vcpkg` for dependency management problems
- Include appropriate priority label

**For Bug Reports:**
- Always use `bug` as primary label
- Add `priority-high` for critical bugs affecting releases
- Add `compatibility` for platform-specific issues
- Add relevant component labels (cmake, ci-cd, etc.)

**For Feature Requests:**
- Use `enhancement` as primary label
- Add relevant component labels
- Include priority level based on impact

**Example Label Combinations:**
- CMake cache variable issue: `enhancement`, `cmake`, `fetchcontent`, `technical-debt`, `priority-medium`
- Critical build failure: `bug`, `build-system`, `priority-high`
- FetchContent documentation: `documentation`, `fetchcontent`, `priority-low`
