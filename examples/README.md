# VanillaPDF Examples

This directory contains practical examples demonstrating VanillaPDF integration and usage patterns.

## Available Examples

- **[`fetchcontent-integration/`](fetchcontent-integration/)** - Complete CMake FetchContent integration example with cross-platform testing
- **[`android-ndk-integration/`](android-ndk-integration/)** - Android NDK cross-compilation example for arm64/arm/x64 ABIs

## Documentation

For detailed documentation of all examples, including integration patterns and API usage, see the **[Examples Documentation](https://vanillapdf.github.io/vanillapdf/page_examples.html)**.

## Quick Start

To run the FetchContent integration example:
```bash
cd examples/fetchcontent-integration
cmake --preset windows-x64-debug  # Choose your platform
cmake --build --preset windows-x64-debug
ctest --preset windows-x64-debug --output-on-failure
```