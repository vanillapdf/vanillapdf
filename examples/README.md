# Vanilla.PDF Examples

This directory contains practical examples demonstrating Vanilla.PDF integration and usage patterns.

## Available Examples

- **[`quickstart/`](quickstart/)** - The first program of the quickstart guide, which prints the library version; built and run by the library's test suite
- **[`create/`](create/)** - Creates a PDF document with a blank page, as shown at the end of the quickstart guide; built and run by the library's test suite
- **[`inspect/`](inspect/)**, **[`merge/`](merge/)**, **[`signing/`](signing/)**, **[`verification/`](verification/)**, **[`encryption/`](encryption/)**, **[`validation/`](validation/)** - The task examples of the [examples page](https://vanillapdf.readthedocs.io/en/latest/examples.html); built and run by the library's test suite
- **[`fetchcontent-integration/`](fetchcontent-integration/)** - Complete CMake FetchContent integration example with cross-platform testing
- **[`vcpkg-port-integration/`](vcpkg-port-integration/)** - Consuming Vanilla.PDF via its vcpkg port
- **[`vcpkg-msbuild-integration/`](vcpkg-msbuild-integration/)** - Consuming the vcpkg port from a Visual Studio project through the vcpkg MSBuild integration, without CMake
- **[`conan-integration/`](conan-integration/)** - Consuming Vanilla.PDF via its Conan recipe
- **[`homebrew-integration/`](homebrew-integration/)** - Consuming Vanilla.PDF via its Homebrew formula
- **[`system-package-integration/`](system-package-integration/)** - Consuming Vanilla.PDF from an installed DEB or RPM package
- **[`shared/`](shared/)** - Header-only helpers (e.g. `scope_guard.h`) shared across the examples above; not a standalone example

## Documentation

For detailed documentation of all examples, including integration patterns and API usage, see the **[Examples Documentation](https://vanillapdf.readthedocs.io/en/latest/examples.html)**.

## Quick Start

To run the FetchContent integration example:
```bash
cd examples/fetchcontent-integration
cmake --preset windows-x64-debug  # Choose your platform
cmake --build --preset windows-x64-debug
ctest --preset windows-x64-debug --output-on-failure
```