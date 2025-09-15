# VanillaPDF Examples

This directory contains practical examples demonstrating different ways to integrate and use VanillaPDF.

## Available Examples

### [`fetchcontent-integration/`](fetchcontent-integration/)
**Complete FetchContent integration example**

Demonstrates how to use VanillaPDF via CMake's `FetchContent` mechanism. Shows:
- Zero-configuration setup
- Automatic dependency management via internal vcpkg
- Conflict-free integration with parent projects
- All features enabled by default (encryption, JPEG, JPEG2000)

**Perfect for:** Adding VanillaPDF to existing CMake projects without external setup.

```cmake
include(FetchContent)
FetchContent_Declare(
    vanillapdf
    GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(vanillapdf)
target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)
```

---

## Running Examples

Each example directory contains:
- `CMakeLists.txt` - Build configuration
- `README.md` - Detailed instructions and explanation
- Source files demonstrating usage

To run any example:
```bash
cd examples/<example-name>
cmake -S . -B build
cmake --build build
```