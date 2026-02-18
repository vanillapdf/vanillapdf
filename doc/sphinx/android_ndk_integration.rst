Android NDK Integration Example
===============================

The Android NDK integration example demonstrates how to cross-compile
Vanilla.PDF for Android ABIs using CMake presets and vcpkg toolchain
chainloading.

Features Demonstrated
---------------------

- **Android toolchain integration** using the NDK with CMake + Ninja
- **RID-compatible ABI mapping** for ``android-arm64``, ``android-arm``, and ``android-x64``
- **vcpkg chainload flow** with Android triplets
- **CI validation** through build-only cross-compilation jobs

Quick Usage
-----------

.. code-block:: bash

   export ANDROID_NDK_ROOT=/path/to/android/ndk
   cd examples/android-ndk-integration
   cmake --preset android-arm64-release
   cmake --build --preset android-arm64-release

Available presets:

- ``android-arm64-release`` (``arm64-v8a``)
- ``android-arm-release`` (``armeabi-v7a``)
- ``android-x64-release`` (``x86_64``)

.. note::

   This example validates cross-compilation and linking on CI.
   The resulting Android binaries are not executed on the host machine.
