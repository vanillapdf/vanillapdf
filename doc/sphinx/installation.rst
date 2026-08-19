Installation
============

This page covers how to add Vanilla.PDF to your project as a pre-built
dependency. To build from source instead, see :doc:`building`.

vcpkg (Recommended)
-------------------

Install with `vcpkg <https://github.com/microsoft/vcpkg>`_:

.. code-block:: bash

   vcpkg install vanillapdf

Then point CMake at your vcpkg toolchain:

.. code-block:: bash

   cmake -S . -B build \
     -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
   cmake --build build

vcpkg supports feature selection to control optional dependencies:

.. code-block:: bash

   vcpkg install vanillapdf[openssl,zlib,spdlog]

Available features:

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Feature
     - Description
   * - ``openssl``
     - Enable encryption and decryption of secure PDF documents
   * - ``libjpeg-turbo``
     - Decode JPEG images into bitmaps
   * - ``openjpeg``
     - Support JPEG-2000 images through the OpenJPEG codec
   * - ``zlib``
     - Decompress PDF objects compressed with zlib
   * - ``spdlog``
     - High-performance logging for diagnostics and debugging
   * - ``nlohmann-json``
     - Parse application configuration from JSON files
   * - ``tests``
     - Enable unit and integration tests (includes GTest)
   * - ``benchmarks``
     - Enable performance benchmarking tools (includes Google Benchmark)

CMake FetchContent
------------------

Self-contained approach without external package managers:

.. code-block:: cmake
   :substitutions:

   cmake_minimum_required(VERSION 3.20)
   project(MyApp)

   include(FetchContent)
   FetchContent_Declare(
       vanillapdf
       GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
       GIT_TAG        main  # or "v|release|" for a specific release
   )
   FetchContent_MakeAvailable(vanillapdf)

   add_executable(myapp main.c)
   target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)

.. note::

   When using FetchContent, you are responsible for managing Vanilla.PDF's
   dependencies. Install them via system package managers before configuring:

   **Linux:**

   .. code-block:: bash

      sudo apt-get install libssl-dev libjpeg-turbo8-dev zlib1g-dev libopenjp2-7-dev

   **macOS:**

   .. code-block:: bash

      brew install openssl@3 jpeg-turbo zlib openjpeg

A complete working example with cross-platform CI validation is available in
`examples/fetchcontent-integration/
<https://github.com/vanillapdf/vanillapdf/tree/main/examples/fetchcontent-integration>`_.

Run it:

.. code-block:: bash

   cd examples/fetchcontent-integration
   cmake --preset windows-x64-debug  # or linux-x64-debug, macos-arm64-debug
   cmake --build --preset windows-x64-debug
   ctest --preset windows-x64-debug --output-on-failure

Conan
-----

.. note::

   **Coming soon.** The ``vanillapdf`` package is not yet published to Conan
   Center. The instructions below will apply once it is available.

Install via `Conan <https://conan.io/>`_:

.. code-block:: bash
   :substitutions:

   pip install conan
   conan install --requires="vanillapdf/|release|" --build=missing

Or add a ``conanfile.py`` to your project:

.. code-block:: python
   :substitutions:

   from conan import ConanFile
   from conan.tools.cmake import cmake_layout

   class MyApp(ConanFile):
       settings = "os", "compiler", "build_type", "arch"
       generators = "CMakeDeps", "CMakeToolchain"

       def requirements(self):
           self.requires("vanillapdf/|release|")

       def layout(self):
           cmake_layout(self)

A working Conan integration example is available in
`examples/conan-integration/
<https://github.com/vanillapdf/vanillapdf/tree/main/examples/conan-integration>`_.

Homebrew (macOS)
----------------

.. note::

   **Coming soon.** The Homebrew formula is not yet live in Homebrew core. The
   instructions below will apply once it is available.

Install via `Homebrew <https://brew.sh/>`_:

.. code-block:: bash

   brew install vanillapdf

The formula installs the shared library, headers, and CLI tools. Dependencies
(OpenSSL, jpeg-turbo, openjpeg, spdlog, nlohmann-json) are pulled
automatically.

NuGet (.NET interop)
--------------------

For .NET projects, install the NuGet package:

.. code-block:: bash

   dotnet add package vanillapdf.net

The correct native runtime is included automatically based on your target
platform and RID. No manual DLL management is required.

Linking with CMake
------------------

Regardless of installation method, use ``find_package`` in your CMakeLists.txt:

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.20)
   project(MyApp C)

   find_package(vanillapdf CONFIG REQUIRED)

   add_executable(myapp main.c)
   target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)

Verify it works with a minimal ``main.c``:

.. code-block:: c

   #include <stdio.h>
   #include <vanillapdf/c_vanillapdf_api.h>

   int main(void) {
       DocumentHandle* doc = NULL;
       error_type rc = Document_Create("test.pdf", &doc);
       if (rc != VANILLAPDF_ERROR_SUCCESS || doc == NULL) {
           printf("Failed to create document\n");
           return 1;
       }
       Document_Release(doc);
       printf("vanillapdf is working.\n");
       return 0;
   }

Build and run:

.. code-block:: bash

   cmake -S . -B build
   cmake --build build
   ./build/myapp

For building the library from source (cloning, presets, CMake options, tests,
and troubleshooting), see :doc:`building`.
