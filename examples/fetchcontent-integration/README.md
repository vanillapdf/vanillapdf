# FetchContent Integration Example

This example demonstrates real-world integration of VanillaPDF using CMake's FetchContent mechanism.

## Quick Start

```bash
cmake --preset windows-x64-debug  # Choose your platform preset
cmake --build --preset windows-x64-debug
ctest --preset windows-x64-debug --output-on-failure
```

## Available Presets

- `windows-x64-debug` / `windows-x64-release`
- `linux-x64-debug` / `linux-x64-release`
- `macos-arm64-debug` / `macos-arm64-release`

## Documentation

For complete documentation including dependency management, configuration options, and CI validation details, see:

**[Installation Guide — FetchContent](https://vanillapdf.readthedocs.io/en/latest/installation.html#cmake-fetchcontent)**

This example is continuously tested across all platforms via automated CI.