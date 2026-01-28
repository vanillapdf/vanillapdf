FetchContent Integration Example
================================

The FetchContent integration example demonstrates how to consume VanillaPDF using
CMake's FetchContent mechanism in real-world scenarios.

Features Demonstrated
---------------------

- **Real GitHub Integration**: Fetches VanillaPDF from GitHub repository (not local source)
- **Cross-platform Testing**: Windows (vcpkg), Linux (apt), macOS (Homebrew) dependency strategies
- **CMake Test Integration**: Uses ``add_test()`` and ``enable_testing()`` for proper test execution
- **Dependency Management**: Shows both vcpkg and system package approaches
- **Actual PDF Creation**: Creates real PDF files and validates functionality

Quick Usage
-----------

.. code-block:: bash

   cd examples/fetchcontent-integration
   cmake --preset windows-x64-debug  # or linux-x64-debug, macos-arm64-debug
   cmake --build --preset windows-x64-debug
   ctest --preset windows-x64-debug --output-on-failure

Dependency Management
---------------------

The example demonstrates different dependency management strategies:

- **Windows**: Uses internal vcpkg for all dependencies
- **Linux**: Uses system packages (``apt-get install``) for faster builds
- **macOS**: Uses Homebrew packages (``brew install``) for faster builds

Continuous Integration
----------------------

This integration approach is continuously validated via automated CI workflows
across all supported platforms.

.. note::

   When using FetchContent, you are responsible for managing VanillaPDF's dependencies
   through system package managers or other packaging systems.
