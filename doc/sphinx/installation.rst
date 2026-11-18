Installation
============

This page covers how to add Vanilla.PDF to your project as a pre-built
dependency. To build from source instead, see :doc:`building`.

.. _install-vcpkg:

vcpkg (recommended)
-------------------

Install with `vcpkg <https://github.com/microsoft/vcpkg>`_:

.. code-block:: bash

   vcpkg install vanillapdf

Then point CMake at your vcpkg toolchain:

.. code-block:: bash

   cmake -S . -B build \
     -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
   cmake --build build

The port builds the library with every feature: encryption and signing
(OpenSSL), JPEG images (libjpeg-turbo), JPEG 2000 images (OpenJPEG) and Flate
compression (zlib). It has no vcpkg features to leave them out. To build
without one of these dependencies, build from source with the
:ref:`feature options <feature-options>`.

.. _install-fetchcontent:

CMake FetchContent
------------------

Self-contained approach without external package managers:

.. code-block:: cmake
   :substitutions:

   cmake_minimum_required(VERSION 3.28)
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

      brew install openssl@4 jpeg-turbo zlib openjpeg

A complete working example with cross-platform CI validation is available in
`examples/fetchcontent-integration/
<https://github.com/vanillapdf/vanillapdf/tree/main/examples/fetchcontent-integration>`_.

Run it:

.. code-block:: bash

   cd examples/fetchcontent-integration
   cmake --preset windows-x64-debug  # or linux-x64-debug, macos-arm64-debug
   cmake --build --preset windows-x64-debug
   ctest --preset windows-x64-debug --output-on-failure

Linking with CMake
------------------

Regardless of installation method, use ``find_package`` in your CMakeLists.txt:

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.20)
   project(MyApp C)

   find_package(vanillapdf CONFIG REQUIRED)

   add_executable(myapp main.c)
   target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)

To check the installation, build and run the first program of the
:doc:`quickstart`.

For building the library from source (cloning, presets, CMake options, tests,
and troubleshooting), see :doc:`building`.

.. _upcoming-packages:

In progress
-----------

.. warning::

   The packages below are **not available yet**. Their recipes are maintained
   in the repository, but they cannot be installed until Conan Center and
   Homebrew core accept them. Use one of the methods above in the meantime.

Conan
^^^^^

The ``vanillapdf`` package is not yet published to Conan Center. Once it is,
install via `Conan <https://conan.io/>`_:

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
^^^^^^^^^^^^^^^^

The formula is not yet in Homebrew core. Once it is, install via
`Homebrew <https://brew.sh/>`_:

.. code-block:: bash

   brew install vanillapdf

The formula installs the shared library and headers. Dependencies
(OpenSSL, jpeg-turbo, openjpeg, spdlog, nlohmann-json) are pulled
automatically.
