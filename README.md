# Vanilla.PDF – The Ultimate C++ PDF Toolkit

[![NuGet](https://img.shields.io/nuget/v/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![Downloads](https://img.shields.io/nuget/dt/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue)](LICENSE.txt)
[![Build](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml)
[![Coverage](https://codecov.io/gh/vanillapdf/vanillapdf/branch/main/graph/badge.svg?token=1UO4W5XGTL)](https://codecov.io/gh/vanillapdf/vanillapdf)
[![Docs](https://img.shields.io/badge/docs-available-brightgreen)](https://vanillapdf.github.io/vanillapdf)

**Vanilla.PDF** is a modern, high-performance, open-source C++17 SDK for creating, editing, signing, and analyzing PDF documents. With no external runtime dependencies and full cross-platform support, it's ideal for embedding into desktop, server, or automation workflows.

## Features

| Category | Description |
|----------|-------------|
| **Digital Signatures** | Add and verify CMS (PKCS#7) digital signatures |
| **PDF Generation** | Create documents with text, images, paths, and pages |
| **Advanced Editing** | Modify content streams, metadata, and page structure |
| **PDF Parsing** | Inspect low-level internals like XRef tables and objects |
| **Encryption** | Work with standard PDF security models |
| **CLI Tools** | Batch-process PDFs directly from the terminal |

## Supported Platforms

| Platform | Compilers | Architectures |
|----------|-----------|---------------|
| Windows | Visual Studio 2022 (MSVC 17.x) | x86, x64 |
| Linux | GCC 8.1+, Clang 10+ | x64, ARM64, ARM |
| macOS | AppleClang 15+ (Xcode 15) | x64, ARM64 |
| Android | NDK toolchain | arm64-v8a, armeabi-v7a, x86, x86_64 |

## Quick Start

```bash
# Clone repository
git clone https://github.com/vanillapdf/vanillapdf.git
cd vanillapdf

# Initialize submodules (required for vcpkg dependencies)
git submodule sync --recursive
git submodule update --init --recursive

# Build (Windows)
cmake --preset windows-x64-msvc-17
cmake --build --preset windows-x64-msvc-17

# Build (Linux/macOS) - use appropriate preset
cmake --preset linux-x64-gcc      # or macos-arm64
cmake --build --preset linux-x64-gcc
```

Run `cmake --list-presets` to see all available configurations including static/dynamic CRT variants and cross-compilation options.

## Installation Options

### vcpkg (Recommended)

The fastest way to get started with pre-compiled binaries:

```bash
vcpkg install vanillapdf
```

### CMake FetchContent

Self-contained approach without external package managers:

```cmake
include(FetchContent)
FetchContent_Declare(vanillapdf
    GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
    GIT_TAG        main)
FetchContent_MakeAvailable(vanillapdf)
target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

A complete working example is available in [`examples/fetchcontent-integration/`](examples/fetchcontent-integration/).

### System Dependencies

For builds using system packages instead of vcpkg:

```bash
# Ubuntu/Debian
sudo apt-get install libssl-dev libjpeg-turbo8-dev zlib1g-dev

# macOS
brew install openssl libjpeg-turbo

# Then configure with external dependency flags
cmake -B build -DVANILLAPDF_EXTERNAL_OPENSSL=ON -DVANILLAPDF_EXTERNAL_JPEG=ON
```

See the [Installation Guide](https://vanillapdf.github.io/vanillapdf/page_install.html) for complete setup options, build configuration, and troubleshooting.

## CLI Tool: `vanillapdf-tools`

The bundled CLI utility enables PDF operations from the command line:

```bash
# Sign a PDF document
./vanillapdf-tools sign -s input.pdf -d signed.pdf -k private_key.p12 -p password

# Verify a signature
./vanillapdf-tools verify -s signed.pdf

# See all available commands
./vanillapdf-tools --help
```

| Option | Description |
|--------|-------------|
| `-s` | Source PDF file |
| `-d` | Destination/output PDF file |
| `-k` | Private key file (PKCS#12 format) |
| `-p` | Password for the key |

## Documentation

| Resource | Description |
|----------|-------------|
| [Official Documentation](https://vanillapdf.github.io/vanillapdf/) | Complete manual and guides |
| [API Reference](https://vanillapdf.github.io/vanillapdf/annotated.html) | Doxygen-generated API docs |
| [Installation Guide](https://vanillapdf.github.io/vanillapdf/page_install.html) | Build options and platform setup |
| [Architecture Overview](https://vanillapdf.github.io/vanillapdf/page_architecture.html) | Internal design and patterns |
| [PDF Tutorial](https://vanillapdf.github.io/vanillapdf/page_pdf_course.html) | Learn PDF format fundamentals |
| [Signature Verification](https://vanillapdf.github.io/vanillapdf/page_signature_verification.html) | Digital signature guide |
| [Code Examples](https://vanillapdf.github.io/vanillapdf/page_examples.html) | Sample code for common tasks |
| [Packaging Guide](https://vanillapdf.github.io/vanillapdf/page_packaging.html) | DEB, Homebrew, NuGet builds |

## Contributing

We welcome pull requests, feature proposals, and bug reports!

- [Contributing Guidelines](CONTRIBUTING.md)
- [Code of Conduct](CODE_OF_CONDUCT.md)
- [Development Setup](CLAUDE.md)

**Important:** All changes require a branch and pull request. The `main` and `release/*` branches are protected.

## License

Vanilla.PDF is licensed under the [Apache 2.0 License](LICENSE.txt).

## Contact & Support

| Channel | Link |
|---------|------|
| Email | info@vanillapdf.com |
| Website | [vanillapdf.com/contact](https://vanillapdf.com/contact/) |
| Issues | [GitHub Issues](https://github.com/vanillapdf/vanillapdf/issues) |
| Sponsor | [GitHub Sponsors](https://github.com/sponsors/vanillapdf) |
