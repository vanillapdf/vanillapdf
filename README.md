# Vanilla.PDF

[![Build](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml/badge.svg)](https://github.com/vanillapdf/vanillapdf/actions/workflows/nightly-check.yml)
[![Coverage](https://codecov.io/gh/vanillapdf/vanillapdf/branch/main/graph/badge.svg?token=1UO4W5XGTL)](https://codecov.io/gh/vanillapdf/vanillapdf)
[![Docs](https://readthedocs.org/projects/vanillapdf/badge/?version=latest)](https://vanillapdf.readthedocs.io/)
[![NuGet](https://img.shields.io/nuget/v/vanillapdf?color=blue)](https://www.nuget.org/packages/vanillapdf)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue)](LICENSE.txt)

**Vanilla.PDF** is a modern, high-performance, open-source C++17 SDK for creating, editing, signing, and analyzing PDF documents. With no external runtime dependencies and full cross-platform support, it's ideal for embedding into desktop, server, or automation workflows.

## Install

### vcpkg (Recommended)

```bash
vcpkg install vanillapdf
```

### CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(vanillapdf
    GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
    GIT_TAG        main)
FetchContent_MakeAvailable(vanillapdf)
target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

### Conan

```bash
conan install --requires="vanillapdf/2.3.0" --build=missing
```

### Build from Source

```bash
git clone https://github.com/vanillapdf/vanillapdf.git
cd vanillapdf
git submodule sync --recursive && git submodule update --init --recursive
cmake --preset linux-x64-gcc      # or windows-x64-msvc-17, macos-arm64
cmake --build --preset linux-x64-gcc
```

[Full installation guide](https://vanillapdf.readthedocs.io/en/latest/installation.html) | [Building from source](https://vanillapdf.readthedocs.io/en/latest/building.html)

## Features

| Feature | Description |
|---------|-------------|
| **Digital Signatures** | Add and verify CMS (PKCS#7) digital signatures |
| **PDF Generation** | Create documents with text, images, paths, and pages |
| **Advanced Editing** | Modify content streams, metadata, and page structure |
| **PDF Parsing** | Inspect low-level internals like XRef tables and objects |
| **Encryption** | Work with standard PDF security models |
| **CLI Tools** | Batch-process PDFs directly from the terminal |

## Platforms

| Platform | Compilers | Architectures |
|----------|-----------|---------------|
| Windows | Visual Studio 2022, 2026 | x86, x64 |
| Linux | GCC 8.1+, Clang 10+ | x64, ARM64, ARM |
| macOS | AppleClang 15+ (Xcode 15) | x64, ARM64 |
| Android | NDK toolchain | arm64, armv7, x86, x86_64 |

## Documentation

Full documentation is hosted on [Read the Docs](https://vanillapdf.readthedocs.io/).

| Guide | Description |
|-------|-------------|
| [Overview](https://vanillapdf.readthedocs.io/en/latest/overview.html) | Project overview and capabilities |
| [Quickstart](https://vanillapdf.readthedocs.io/en/latest/quickstart.html) | Create your first PDF document |
| [Installation](https://vanillapdf.readthedocs.io/en/latest/installation.html) | vcpkg, FetchContent, Conan setup |
| [C API Guide](https://vanillapdf.readthedocs.io/en/latest/c_api.html) | Memory management, error handling |
| [Examples](https://vanillapdf.readthedocs.io/en/latest/examples.html) | Code samples for common tasks |
| [Architecture](https://vanillapdf.readthedocs.io/en/latest/architecture.html) | Internal design and patterns |
| [CLI Tools](https://vanillapdf.readthedocs.io/en/latest/cli_tools.html) | Command-line PDF processing |
| [PDF Format](https://vanillapdf.readthedocs.io/en/latest/pdf_format.html) | Learn PDF format fundamentals |

## Contributing

We welcome pull requests, feature proposals, and bug reports!

- [Contributing Guidelines](CONTRIBUTING.md)
- [Code of Conduct](CODE_OF_CONDUCT.md)

All changes require a branch and pull request. The `main` and `release/*` branches are protected.

## License

Vanilla.PDF is licensed under the [Apache 2.0 License](LICENSE.txt).

## Contact & Support

| Channel | Link |
|---------|------|
| Email | info@vanillapdf.com |
| Website | [vanillapdf.com](https://vanillapdf.com) |
| Issues | [GitHub Issues](https://github.com/vanillapdf/vanillapdf/issues) |
| Sponsor | [GitHub Sponsors](https://github.com/sponsors/vanillapdf) |
