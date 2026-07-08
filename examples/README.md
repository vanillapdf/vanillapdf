# Vanilla.PDF Examples

This directory contains practical examples demonstrating Vanilla.PDF integration and usage patterns.

## Available Examples

- **[`fetchcontent-integration/`](fetchcontent-integration/)** - Complete CMake FetchContent integration example with cross-platform testing
- **[`vcpkg-port-integration/`](vcpkg-port-integration/)** - Consuming Vanilla.PDF via its vcpkg port
- **[`conan-integration/`](conan-integration/)** - Consuming Vanilla.PDF via its Conan recipe
- **[`homebrew-integration/`](homebrew-integration/)** - Consuming Vanilla.PDF via its Homebrew formula
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