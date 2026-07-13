Packaging
=========

This page details how to generate installable packages for Vanilla.PDF on
common platforms using automated packaging workflows.

For general build instructions, see :doc:`building`.

Automated Package Building
--------------------------

The project includes automated GitHub Actions workflows for package generation:

- **Debian packages**: ``.github/workflows/build-deb-package.yml``
- **RPM packages**: ``.github/workflows/build-rpm-package.yml``
- **Homebrew packages**: ``.github/workflows/build-brew-package.yml``
- **NuGet packages**: ``.github/workflows/build-nuget.yml``

These workflows are continuously tested and use the exact configuration shown below.

Debian Packages
---------------

On Debian/Ubuntu systems, first install the required dependencies:

.. code-block:: bash

   sudo apt-get update
   sudo apt-get install -y libssl-dev libjpeg-turbo8-dev zlib1g-dev libopenjp2-7-dev libopenjp2-tools

Build the Debian package:

.. code-block:: bash

   cmake --preset linux-x64-gcc -DCMAKE_BUILD_TYPE=Release \
     -DVANILLAPDF_ENABLE_PACKAGING=ON \
     -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
     -DVANILLAPDF_EXTERNAL_JPEG=ON \
     -DVANILLAPDF_EXTERNAL_OPENJPEG=ON \
     -DVANILLAPDF_EXTERNAL_ZLIB=ON \
     -DVANILLAPDF_ENABLE_TESTS=ON
   cmake --build --preset linux-x64-gcc --target package

This generates a ``.deb`` file that can be installed with ``dpkg -i``.

Homebrew Packages
-----------------

On macOS, install the required dependencies:

.. code-block:: bash

   brew install openssl@3 jpeg-turbo zlib openjpeg

Build the Homebrew package:

.. code-block:: bash

   cmake --preset macos-arm64 -DCMAKE_BUILD_TYPE=Release \
     -DVANILLAPDF_ENABLE_PACKAGING=ON \
     -DVANILLAPDF_EXTERNAL_OPENSSL=ON \
     -DVANILLAPDF_EXTERNAL_JPEG=ON \
     -DVANILLAPDF_EXTERNAL_OPENJPEG=ON \
     -DVANILLAPDF_EXTERNAL_ZLIB=ON \
     -DVANILLAPDF_ENABLE_TESTS=ON
   cmake --build --preset macos-arm64 --target package

This generates package files suitable for Homebrew distribution.

NuGet Packages
--------------

Windows NuGet packages are built using the automated workflow with vcpkg dependencies:

.. code-block:: bash

   cmake --preset windows-x64-msvc-17-static -DCMAKE_BUILD_TYPE=Release \
     -DVANILLAPDF_ENABLE_PACKAGING=ON \
     -DVANILLAPDF_ENABLE_TESTS=ON
   cmake --build --preset windows-x64-msvc-17-static --config Release --target package

Configuration Notes
-------------------

- ``VANILLAPDF_ENABLE_PACKAGING`` is auto-detected for main project builds
- Using system packages (``EXTERNAL_*=ON``) results in smaller packages and faster builds
- vcpkg dependencies (``EXTERNAL_*=OFF``) create self-contained packages
- **Linux**: System packages are preferred for distribution packages
- **macOS**: Homebrew dependencies are standard for macOS packages
- **Windows**: vcpkg dependencies provide consistent, self-contained packages

For automated package building in CI environments, refer to the GitHub Actions workflows
in ``.github/workflows/`` which demonstrate the complete build process.
