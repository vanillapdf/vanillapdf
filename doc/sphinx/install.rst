Installation & Build Guide
==========================

This comprehensive guide covers how to obtain, build, and configure Vanilla.PDF
for all supported platforms and use cases.

Installation Methods
--------------------

Install with vcpkg (Recommended)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The fastest way to get started is using
`vcpkg <https://github.com/microsoft/vcpkg>`_:

.. code-block:: bash

   vcpkg install vanillapdf

After installation point CMake at your vcpkg instance:

.. code-block:: bash

   cmake -S . -B build \
     -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
   cmake --build build

Benefits include fastest builds with pre-compiled binaries, feature control via
``vcpkg install vanillapdf[openssl,jpeg,jpeg2000]``, and shared binary cache across projects.

FetchContent Integration (Alternative)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For users who prefer not to introduce external tools into their build chain,
use CMake's FetchContent mechanism:

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.20)
   project(MyApp)

   include(FetchContent)
   FetchContent_Declare(
       vanillapdf
       GIT_REPOSITORY https://github.com/vanillapdf/vanillapdf.git
       GIT_TAG        main  # or "v2.1.0" for specific release
   )
   FetchContent_MakeAvailable(vanillapdf)

   # Link to your target
   target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)

.. note::

   When using FetchContent, you are responsible for managing VanillaPDF's
   dependencies. Install them via system package managers
   (``apt-get install libssl-dev libjpeg-turbo8-dev`` on Linux,
   ``brew install openssl libjpeg-turbo`` on macOS) or other packaging systems like Conan.

A complete working example with cross-platform testing is available in the
FetchContent integration example with automated CI validation.

Build Requirements
------------------

**CMake** (minimum version 3.20)
Download from https://cmake.org/download/

**Supported Compilers:**

- **Windows:** Visual Studio 2022 (MSVC 17.x) or Visual Studio 2026 (MSVC 18.x) — x86, x64, ARM64
- **Linux:** GCC 8.1+ or Clang 10+ (x64, ARM64, ARM)
- **macOS:** AppleClang 15+ (Xcode 15)
- **Android:** NDK toolchain (arm64-v8a, armeabi-v7a, x86, x86_64)

Build from Source
-----------------

Clone and Setup
^^^^^^^^^^^^^^^

Clone the repository and initialize the bundled vcpkg submodule:

.. code-block:: bash

   git clone https://github.com/vanillapdf/vanillapdf.git
   cd vanillapdf
   # fetch submodules for bundled vcpkg
   git submodule sync --recursive
   git submodule update --init --recursive

Available Build Presets
^^^^^^^^^^^^^^^^^^^^^^^

List the available build presets:

.. code-block:: bash

   cmake --list-presets

.. code-block:: text

   Available configure presets:

     "windows-x86-msvc-17-static"
     "windows-x64-msvc-17-static"
     "windows-x86-msvc-17"
     "windows-x64-msvc-17"
     "windows-arm64-msvc-17"
     "windows-arm64-msvc-17-static"
     "windows-arm64-msvc-17-static-md"
     "windows-x86-msvc-17-static-md"
     "windows-x64-msvc-17-static-md"
     "windows-x86-msvc-18-static"
     "windows-x64-msvc-18-static"
     "windows-arm64-msvc-18-static"
     "windows-x86-msvc-18"
     "windows-x64-msvc-18"
     "windows-arm64-msvc-18"
     "windows-x86-msvc-18-static-md"
     "windows-x64-msvc-18-static-md"
     "windows-arm64-msvc-18-static-md"
     "default"
     "linux-x64-gcc"
     "linux-arm64-gcc"
     "linux-x64-clang"
     "linux-arm64-clang"
     "linux-x64-musl"
     "linux-arm64-musl"
     "linux-arm-musl"
     "macos-x64"
     "macos-arm64"
     "android-arm64"
     "android-armv7"
     "android-x86"
     "android-x86_64"

Common presets include:

- ``windows-{x86,x64,arm64}-msvc-17`` - Windows with Visual Studio 2022 (dynamic CRT)
- ``windows-{x86,x64,arm64}-msvc-17-static`` - Windows with Visual Studio 2022 (static CRT)
- ``windows-{x86,x64,arm64}-msvc-17-static-md`` - Windows with Visual Studio 2022 (static libs, dynamic CRT)
- ``windows-{x86,x64,arm64}-msvc-18`` - Windows with Visual Studio 2026 (dynamic CRT)
- ``windows-{x86,x64,arm64}-msvc-18-static`` - Windows with Visual Studio 2026 (static CRT)
- ``windows-{x86,x64,arm64}-msvc-18-static-md`` - Windows with Visual Studio 2026 (static libs, dynamic CRT)
- ``linux-x64-gcc`` / ``linux-arm64-gcc`` - Linux with GCC
- ``linux-x64-clang`` / ``linux-arm64-clang`` - Linux with Clang
- ``macos-x64`` / ``macos-arm64`` - macOS builds

Platform-Specific Builds
^^^^^^^^^^^^^^^^^^^^^^^^^

Building on Windows
"""""""""""""""""""

Choose a preset matching your toolchain:

.. code-block:: bash

   cmake --preset windows-x64-msvc-17
   cmake --build --preset windows-x64-msvc-17
   ctest --preset windows-x64-msvc-17

**Windows Build Notes:**

- Uses Visual Studio 2022 (msvc-17) or Visual Studio 2026 (msvc-18) generator
- CRT linking depends on preset: ``-static`` variants use static CRT (``VANILLAPDF_USE_STATIC_CRT=ON``)
- Uses optimized vcpkg triplets (e.g., ``x64-windows-static-md`` for static libs + dynamic CRT)

Building on Linux
"""""""""""""""""

.. code-block:: bash

   cmake --preset linux-x64-gcc
   cmake --build --preset linux-x64-gcc
   ctest --preset linux-x64-gcc

**For external dependencies** (optional, faster builds):

.. code-block:: bash

   sudo apt-get install libssl-dev libjpeg-turbo8-dev zlib1g-dev libopenjp2-7-dev
   cmake --preset linux-x64-gcc -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
     -DVANILLAPDF_EXTERNAL_JPEG=ON -DVANILLAPDF_EXTERNAL_OPENJPEG=ON \
     -DVANILLAPDF_EXTERNAL_ZLIB=ON

Building on macOS
"""""""""""""""""

.. code-block:: bash

   cmake --preset macos-arm64
   cmake --build --preset macos-arm64
   ctest --preset macos-arm64

**For external dependencies** (optional, faster builds):

.. code-block:: bash

   brew install openssl@3 jpeg-turbo zlib openjpeg
   cmake --preset macos-arm64 -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
     -DVANILLAPDF_EXTERNAL_JPEG=ON -DVANILLAPDF_EXTERNAL_OPENJPEG=ON \
     -DVANILLAPDF_EXTERNAL_ZLIB=ON

Dependency Management
---------------------

Vanilla.PDF uses **vcpkg** for automatic dependency management. Dependencies are handled
automatically - no manual installation required.

**Automatic Dependencies:**

- OpenSSL (encryption and digital signatures)
- libjpeg-turbo (JPEG image support)
- OpenJPEG (JPEG2000 support)
- zlib (compression)
- spdlog (logging)
- nlohmann-json (configuration)

Using your own vcpkg instance
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You may manage vcpkg yourself and install the required packages manually:

.. code-block:: bash

   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.sh
   ./vcpkg install openssl libjpeg-turbo

Configure Vanilla.PDF with the vcpkg toolchain:

.. code-block:: bash

   cmake -S . -B build/vcpkg \
     -DCMAKE_TOOLCHAIN_FILE=</path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake
   cmake --build build/vcpkg

Using system dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^

If dependencies are managed externally (e.g., by system packages, Conan, or a parent project's vcpkg),
disable internal vcpkg entirely:

.. code-block:: bash

   cmake -B build -DVANILLAPDF_INTERNAL_VCPKG=OFF
   cmake --build build

Alternatively, if you want to use internal vcpkg but override specific libraries
to come from your package manager, enable the matching ``VANILLAPDF_EXTERNAL_*`` options:

.. code-block:: bash

   cmake -B build \
     -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
     -DVANILLAPDF_EXTERNAL_JPEG=ON
   cmake --build build

Options exist for OpenSSL, libjpeg-turbo, openjpeg, zlib, spdlog,
nlohmann-json, GTest and Google Benchmark. When ``VANILLAPDF_EXTERNAL_*`` options are set, the
corresponding packages will not be installed by vcpkg and CMake will search for
them on your system instead.

Build Configuration
-------------------

Feature Enable/Disable Options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_ENABLE_ENCRYPTION=ON/OFF`` - PDF encryption/decryption support (default: ON)
- ``-DVANILLAPDF_ENABLE_JPEG=ON/OFF`` - JPEG image support (default: ON)
- ``-DVANILLAPDF_ENABLE_JPEG2000=ON/OFF`` - JPEG2000 support (default: ON)

Build Configuration Options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_INTERNAL_VCPKG=ON/OFF`` - Enable internal vcpkg dependency management (default: ON)
- ``-DVANILLAPDF_ENABLE_PACKAGING=ON/OFF`` - Enable packaging features like CPack (auto-detected)
- ``-DVANILLAPDF_ENABLE_TESTS=ON/OFF`` - Build and run test scenarios (default: OFF)
- ``-DVANILLAPDF_ENABLE_BENCHMARK=ON/OFF`` - Include benchmarking project (default: OFF)
- ``-DVANILLAPDF_USE_STATIC_CRT=ON/OFF`` - Use static MSVC runtime (/MT) instead of dynamic (/MD) (default: OFF)
- ``-DBUILD_SHARED_LIBS=ON/OFF`` - Build vanillapdf as a shared library (default: ON)

Development Options
^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_ENABLE_COVERAGE=ON`` - Enable code coverage instrumentation (GCC/Clang only)
- ``-DVANILLAPDF_FORCE_32_BIT=ON`` - Force 32-bit output binary regardless of architecture
- ``-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON`` - Enable address sanitizer for memory safety testing

External Dependency Options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_EXTERNAL_OPENSSL=ON/OFF`` - Use system OpenSSL instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_JPEG=ON/OFF`` - Use system libjpeg instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_OPENJPEG=ON/OFF`` - Use system OpenJPEG instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_ZLIB=ON/OFF`` - Use system zlib instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_SPDLOG=ON/OFF`` - Use system spdlog instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_NLOHMANN_JSON=ON/OFF`` - Use system nlohmann-json instead of vcpkg (default: OFF)

Running Tests
-------------

Tests require ``-DVANILLAPDF_ENABLE_TESTS=ON`` at configure time.
After building, run the test suite:

.. code-block:: bash

   ctest --preset <your-preset> --output-on-failure

**Test Categories:**

- Unit tests: Core functionality validation
- Integration tests: End-to-end scenarios
- Benchmarks: Performance testing

Troubleshooting
---------------

**Common Issues:**

*vcpkg bootstrap fails:*

.. code-block:: bash

   git submodule sync --recursive
   git submodule update --init --recursive

*Build configuration fails:*

.. code-block:: bash

   rm -rf build/
   cmake --preset <your-preset>

*Platform-specific dependency issues:*

- **Linux**: Install system packages: ``sudo apt-get install libssl-dev libjpeg-turbo8-dev zlib1g-dev libopenjp2-7-dev``
- **macOS**: Install via Homebrew: ``brew install openssl@3 jpeg-turbo zlib openjpeg``
- **Windows**: Use vcpkg presets or ensure Visual Studio 2022 or 2026 is properly installed

*OpenSSL legacy provider on Windows:*

**Static linking is the recommended approach on Windows.** Providers are compiled
directly into the library and no additional configuration is needed.

When linking dynamically, OpenSSL 3.x loads provider modules (e.g. ``legacy.dll``)
at runtime from a compiled-in directory. If that directory does not match the
actual installation, provider loading will fail:

.. code-block:: text

   Failed to initialize legacy OSSL provider
   filename(C:\Program Files (x86)\OpenSSL\bin\legacy.dll)

The caller is responsible for configuring the environment so that OpenSSL can
find its provider modules. Set the ``OPENSSL_MODULES`` environment variable to
the directory containing the provider DLLs before running the application:

.. code-block:: bash

   set OPENSSL_MODULES=path/to/bin

The build system handles this automatically for CTest targets.

Building packages
-----------------

Instructions for creating Debian or Homebrew packages are provided in the
packaging guide.
