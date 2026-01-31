Building from Source
====================

This page explains how to clone, configure, build, test, and troubleshoot
Vanilla.PDF from source. For pre-built installation methods see
:doc:`installation`.

Build requirements
------------------

**CMake** (minimum version 3.20) --
Download from https://cmake.org/download/

**Supported compilers:**

- **Windows:** Visual Studio 2022 (MSVC 17.x) or Visual Studio 2026 (MSVC 18.x)
- **Linux:** GCC 8.1+ or Clang 10+ (x64, ARM64, ARM)
- **macOS:** AppleClang 15+ (Xcode 15)
- **Android:** NDK toolchain (arm64-v8a, armeabi-v7a, x86, x86_64)

Clone and setup
---------------

Clone the repository and initialise the bundled vcpkg submodule:

.. code-block:: bash

   git clone https://github.com/vanillapdf/vanillapdf.git
   cd vanillapdf
   git submodule sync --recursive
   git submodule update --init --recursive

Available build presets
-----------------------

List the available build presets:

.. code-block:: bash

   cmake --list-presets

.. code-block:: text

   Available configure presets:

     "windows-x86-msvc-17-static"
     "windows-x64-msvc-17-static"
     "windows-x86-msvc-17"
     "windows-x64-msvc-17"
     "windows-x86-msvc-17-static-md"
     "windows-x64-msvc-17-static-md"
     "windows-x86-msvc-18-static"
     "windows-x64-msvc-18-static"
     "windows-x86-msvc-18"
     "windows-x64-msvc-18"
     "windows-x86-msvc-18-static-md"
     "windows-x64-msvc-18-static-md"
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

Common presets:

- ``windows-x64-msvc-17`` / ``windows-x86-msvc-17`` -- Windows with Visual Studio 2022 (dynamic CRT)
- ``windows-x64-msvc-17-static`` / ``windows-x86-msvc-17-static`` -- Windows with Visual Studio 2022 (static CRT)
- ``windows-x64-msvc-17-static-md`` / ``windows-x86-msvc-17-static-md`` -- Windows with Visual Studio 2022 (static libs, dynamic CRT)
- ``windows-x64-msvc-18`` / ``windows-x86-msvc-18`` -- Windows with Visual Studio 2026 (dynamic CRT)
- ``windows-x64-msvc-18-static`` / ``windows-x86-msvc-18-static`` -- Windows with Visual Studio 2026 (static CRT)
- ``windows-x64-msvc-18-static-md`` / ``windows-x86-msvc-18-static-md`` -- Windows with Visual Studio 2026 (static libs, dynamic CRT)
- ``linux-x64-gcc`` / ``linux-arm64-gcc`` -- Linux with GCC
- ``linux-x64-clang`` / ``linux-arm64-clang`` -- Linux with Clang
- ``macos-x64`` / ``macos-arm64`` -- macOS builds

Platform-specific builds
------------------------

Building on Windows
^^^^^^^^^^^^^^^^^^^

Choose a preset matching your toolchain:

.. code-block:: bash

   cmake --preset windows-x64-msvc-17
   cmake --build --preset windows-x64-msvc-17
   ctest --preset windows-x64-msvc-17

**Windows build notes:**

- Uses Visual Studio 2022 (msvc-17) or Visual Studio 2026 (msvc-18) generator
- CRT linking depends on preset: ``-static`` variants use static CRT (``VANILLAPDF_USE_STATIC_CRT=ON``)
- Uses optimised vcpkg triplets (e.g. ``x64-windows-static-md`` for static libs + dynamic CRT)

Building on Linux
^^^^^^^^^^^^^^^^^

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
^^^^^^^^^^^^^^^^^

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

Dependency management
---------------------

Vanilla.PDF uses **vcpkg** for automatic dependency management. Dependencies
are handled automatically -- no manual installation required.

**Automatic dependencies:**

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

If dependencies are managed externally (e.g. by system packages, Conan, or a
parent project's vcpkg), disable internal vcpkg entirely:

.. code-block:: bash

   cmake -B build -DVANILLAPDF_INTERNAL_VCPKG=OFF
   cmake --build build

Alternatively, override specific libraries while keeping internal vcpkg for
the rest:

.. code-block:: bash

   cmake -B build \
     -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
     -DVANILLAPDF_EXTERNAL_JPEG=ON
   cmake --build build

Options exist for OpenSSL, libjpeg-turbo, openjpeg, zlib, spdlog,
nlohmann-json, GTest, and Google Benchmark. When ``VANILLAPDF_EXTERNAL_*``
options are set, the corresponding packages will not be installed by vcpkg and
CMake will search for them on your system instead.

Build configuration
-------------------

Feature enable/disable options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_ENABLE_ENCRYPTION=ON/OFF`` -- PDF encryption/decryption support (default: ON)
- ``-DVANILLAPDF_ENABLE_JPEG=ON/OFF`` -- JPEG image support (default: ON)
- ``-DVANILLAPDF_ENABLE_JPEG2000=ON/OFF`` -- JPEG2000 support (default: ON)

Build configuration options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_INTERNAL_VCPKG=ON/OFF`` -- Enable internal vcpkg dependency management (default: ON)
- ``-DVANILLAPDF_ENABLE_PACKAGING=ON/OFF`` -- Enable packaging features like CPack (auto-detected)
- ``-DVANILLAPDF_ENABLE_TESTS=ON/OFF`` -- Build and run test scenarios (default: OFF)
- ``-DVANILLAPDF_ENABLE_BENCHMARK=ON/OFF`` -- Include benchmarking project (default: OFF)
- ``-DVANILLAPDF_USE_STATIC_CRT=ON/OFF`` -- Use static MSVC runtime (/MT) instead of dynamic (/MD) (default: OFF)
- ``-DBUILD_SHARED_LIBS=ON/OFF`` -- Build vanillapdf as a shared library (default: ON)

Development options
^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_ENABLE_COVERAGE=ON`` -- Enable code coverage instrumentation (GCC/Clang only)
- ``-DVANILLAPDF_FORCE_32_BIT=ON`` -- Force 32-bit output binary regardless of architecture
- ``-DVANILLAPDF_ENABLE_STACK_SANITIZER=ON`` -- Enable address sanitizer for memory safety testing

External dependency options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_EXTERNAL_OPENSSL=ON/OFF`` -- Use system OpenSSL instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_JPEG=ON/OFF`` -- Use system libjpeg instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_OPENJPEG=ON/OFF`` -- Use system OpenJPEG instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_ZLIB=ON/OFF`` -- Use system zlib instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_SPDLOG=ON/OFF`` -- Use system spdlog instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_NLOHMANN_JSON=ON/OFF`` -- Use system nlohmann-json instead of vcpkg (default: OFF)

Running tests
-------------

Tests require ``-DVANILLAPDF_ENABLE_TESTS=ON`` at configure time. After
building, run the test suite:

.. code-block:: bash

   ctest --preset <your-preset> --output-on-failure

**Test categories:**

- Unit tests: Core functionality validation
- Integration tests: End-to-end scenarios
- Benchmarks: Performance testing

Run specific test suites:

.. code-block:: bash

   ctest --preset <your-preset> -R "unittest"     # Unit tests only
   ctest --preset <your-preset> -R "test"         # Integration tests only
   ctest --preset <your-preset> -R "benchmark"    # Benchmarks only

**Windows (Visual Studio) note:** Visual Studio is a multi-config generator,
so you must specify a build configuration:

.. code-block:: bash

   ctest --preset windows-x64-msvc-17 --build-config Debug -R "unittest"
   ctest --preset windows-x64-msvc-17 --build-config Release -R "unittest"

Code coverage
^^^^^^^^^^^^^

Enable code coverage (GCC/Clang only):

.. code-block:: bash

   cmake --preset linux-x64-gcc -DVANILLAPDF_ENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
   cmake --build --preset linux-x64-gcc
   ctest --preset linux-x64-gcc
   gcovr -r . --xml-pretty -o coverage.xml

Troubleshooting
---------------

**vcpkg bootstrap fails:**

.. code-block:: bash

   git submodule sync --recursive
   git submodule update --init --recursive

**Build configuration fails:**

.. code-block:: bash

   rm -rf build/
   cmake --preset <your-preset>

**Platform-specific dependency issues:**

- **Linux**: Install system packages: ``sudo apt-get install libssl-dev libjpeg-turbo8-dev zlib1g-dev libopenjp2-7-dev``
- **macOS**: Install via Homebrew: ``brew install openssl@3 jpeg-turbo zlib openjpeg``
- **Windows**: Use vcpkg presets or ensure Visual Studio 2022 or 2026 is properly installed
