Installation
============

This page covers how to add Vanilla.PDF to your project as a pre-built
dependency. If you need to build from source, see :doc:`building`.

vcpkg (Recommended)
-------------------

The fastest way to get started is using
`vcpkg <https://github.com/microsoft/vcpkg>`_:

.. code-block:: bash

   vcpkg install vanillapdf

After installation, point CMake at your vcpkg instance:

.. code-block:: bash

   cmake -S . -B build \
     -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
   cmake --build build

vcpkg supports feature selection to control optional dependencies:

.. code-block:: bash

   vcpkg install vanillapdf[openssl,zlib,spdlog]

Available features: ``openssl``, ``libjpeg-turbo``, ``openjpeg``, ``zlib``,
``spdlog``, ``nlohmann-json``, ``tests``, ``benchmarks``.

CMake FetchContent
------------------

For users who prefer not to introduce external package managers,
use CMake's FetchContent mechanism:

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.20)
   project(MyApp)

   include(FetchContent)
   FetchContent_Declare(
       vanillapdf
       GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
       GIT_TAG        main  # or "v2.1.0" for a specific release
   )
   FetchContent_MakeAvailable(vanillapdf)

   # Link to your target
   target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)

.. note::

   When using FetchContent, you are responsible for managing Vanilla.PDF's
   dependencies. Install them via system package managers
   (``apt-get install libssl-dev libjpeg-turbo8-dev`` on Linux,
   ``brew install openssl libjpeg-turbo`` on macOS) or other packaging
   systems like Conan.

FetchContent integration example
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A complete working example with cross-platform CI is available in
`examples/fetchcontent-integration/
<https://github.com/vanillapdf/vanillapdf/tree/main/examples/fetchcontent-integration>`_.

The example demonstrates:

- **Real GitHub integration** -- Fetches Vanilla.PDF from GitHub (not local source)
- **Cross-platform testing** -- Windows (vcpkg), Linux (apt), macOS (Homebrew)
- **CMake test integration** -- Uses ``add_test()`` and ``enable_testing()``
- **Actual PDF creation** -- Creates real PDF files and validates functionality

Run the example:

.. code-block:: bash

   cd examples/fetchcontent-integration
   cmake --preset windows-x64-debug  # or linux-x64-debug, macos-arm64-debug
   cmake --build --preset windows-x64-debug
   ctest --preset windows-x64-debug --output-on-failure

Platform-specific dependency management:

- **Windows**: Uses internal vcpkg for all dependencies
- **Linux**: Uses system packages (``apt-get install``) for faster builds
- **macOS**: Uses Homebrew packages (``brew install``) for faster builds

Conan
-----

Vanilla.PDF is available via `Conan <https://conan.io/>`_:

.. code-block:: bash

   pip install conan
   conan install --requires="vanillapdf/2.3.0" --build=missing

Refer to the ``conan/`` directory in the repository for the Conan recipe.

Linking with CMake
------------------

Regardless of how you installed Vanilla.PDF, use ``find_package`` in your
CMakeLists.txt:

.. code-block:: cmake

   find_package(vanillapdf REQUIRED)
   add_executable(myapp main.c)
   target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)

For a build-from-source workflow (cloning, presets, CMake options, tests, and
troubleshooting), see :doc:`building`.
