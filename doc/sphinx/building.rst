Building from source
====================

This page explains how to clone, configure, build, package, and troubleshoot
Vanilla.PDF from source. For pre-built installation methods see
:doc:`installation`.

Build requirements
------------------

**CMake** (minimum version 3.28) —
Download from https://cmake.org/download/

**A C++17 compiler** — see :ref:`supported-platforms` for the compilers
and platforms tested by the CI.

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
     "windows-arm64-msvc-17-static"
     "windows-x86-msvc-17"
     "windows-x64-msvc-17"
     "windows-arm64-msvc-17"
     "windows-x86-msvc-17-static-md"
     "windows-x64-msvc-17-static-md"
     "windows-arm64-msvc-17-static-md"
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

Common presets:

- ``windows-{x86,x64,arm64}-msvc-18`` — Windows with Visual Studio 2026 (dynamic CRT)
- ``windows-{x86,x64,arm64}-msvc-18-static`` — Windows with Visual Studio 2026 (static CRT)
- ``windows-{x86,x64,arm64}-msvc-18-static-md`` — Windows with Visual Studio 2026 (static libs, dynamic CRT)
- ``windows-{x86,x64,arm64}-msvc-17`` — Windows with Visual Studio 2022 (dynamic CRT)
- ``windows-{x86,x64,arm64}-msvc-17-static`` — Windows with Visual Studio 2022 (static CRT)
- ``windows-{x86,x64,arm64}-msvc-17-static-md`` — Windows with Visual Studio 2022 (static libs, dynamic CRT)
- ``linux-x64-gcc`` / ``linux-arm64-gcc`` — Linux with GCC
- ``linux-x64-clang`` / ``linux-arm64-clang`` — Linux with Clang
- ``macos-x64`` / ``macos-arm64`` — macOS builds

Platform-specific builds
------------------------

Building on Windows
^^^^^^^^^^^^^^^^^^^

Choose a preset matching your toolchain:

.. code-block:: bash

   cmake --preset windows-x64-msvc-18
   cmake --build --preset windows-x64-msvc-18
   ctest --preset windows-x64-msvc-18 --build-config Debug

**Windows build notes:**

- Uses Visual Studio 2026 (msvc-18) or Visual Studio 2022 (msvc-17) generator
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

   brew install openssl@4 jpeg-turbo zlib openjpeg
   cmake --preset macos-arm64 -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
     -DVANILLAPDF_EXTERNAL_JPEG=ON -DVANILLAPDF_EXTERNAL_OPENJPEG=ON \
     -DVANILLAPDF_EXTERNAL_ZLIB=ON

Build options and tests
-----------------------

The presets build the library with every feature, the tests and the tools.
:doc:`build_options` describes how to turn features off and how to use system
packages instead of vcpkg, and :doc:`testing` describes how to run the tests.

.. _create-packages:

Create packages
---------------

To distribute your own build, for example a modified library for your own
machines, create installable packages with CPack. Packaging is off by default;
turn it on with ``-DVANILLAPDF_ENABLE_PACKAGING=ON`` and build the ``package``
target:

.. code-block:: bash

   cmake --preset linux-x64-gcc -DCMAKE_BUILD_TYPE=Release \
     -DVANILLAPDF_ENABLE_PACKAGING=ON
   cmake --build --preset linux-x64-gcc --target package

The ``package`` target creates a ZIP archive on Windows, a DMG disk image and a
TGZ archive on macOS, and a TGZ archive on Linux. For Debian and RPM packages,
run CPack with the generator from the build directory:

.. code-block:: bash

   cd build/linux-x64-gcc
   cpack -G DEB    # or: cpack -G RPM

The Debian and RPM packages are split into the runtime package
``libvanillapdf`` and the development package ``libvanillapdf-dev``
(``libvanillapdf-devel`` for RPM).

With the default vcpkg dependencies the packages are self-contained. To create
packages that depend on the libraries of the system instead, as distribution
packages do, configure with the ``VANILLAPDF_EXTERNAL_*`` options described in
:doc:`build_options`.

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
- **macOS**: Install via Homebrew: ``brew install openssl@4 jpeg-turbo zlib openjpeg``
- **Windows**: Use vcpkg presets or ensure Visual Studio 2026 or 2022 is properly installed
