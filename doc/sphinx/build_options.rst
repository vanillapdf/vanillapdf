Build options
=============

This page describes how the build finds its dependencies and the CMake options
that turn features on and off. For the basic build, see :doc:`building`.

Dependency management
---------------------

Vanilla.PDF uses **vcpkg** for automatic dependency management. Dependencies
are handled automatically — no manual installation required.

**Always installed** (the four feature libraries can be turned off with the
:ref:`feature options <feature-options>`):

- OpenSSL (encryption and digital signatures)
- libjpeg-turbo (JPEG images)
- OpenJPEG (JPEG 2000 images)
- zlib (Flate compression)
- spdlog and fmt (logging)

**Installed only when the option is on:**

- mimalloc — ``VANILLAPDF_ENABLE_MIMALLOC``
- nlohmann-json — ``VANILLAPDF_ENABLE_LICENSING``
- CLI11 — ``VANILLAPDF_ENABLE_TOOLS``
- GoogleTest — ``VANILLAPDF_ENABLE_TESTS``
- Google Benchmark — ``VANILLAPDF_ENABLE_BENCHMARK``

Using your own vcpkg instance
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

By default the build uses the vcpkg submodule in ``external/vcpkg``. To use
your own vcpkg instead, clone and bootstrap it:

.. code-block:: bash

   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.sh

Then point the build at its toolchain file:

.. code-block:: bash

   cmake -S . -B build/vcpkg \
     -DCMAKE_TOOLCHAIN_FILE=</path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake
   cmake --build build/vcpkg

The configure step generates ``vcpkg.json`` from the enabled features, and
vcpkg installs the dependencies it lists into the build directory.

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

Options exist for OpenSSL, libjpeg-turbo, openjpeg, zlib, spdlog, fmt,
nlohmann-json, mimalloc, GTest, and Google Benchmark. When ``VANILLAPDF_EXTERNAL_*``
options are set, the corresponding packages will not be installed by vcpkg and
CMake will search for them on your system instead.

Build configuration
-------------------

.. _feature-options:

Feature enable/disable options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_ENABLE_ENCRYPTION=ON/OFF`` — PDF encryption/decryption support (default: ON)
- ``-DVANILLAPDF_ENABLE_JPEG=ON/OFF`` — JPEG image support (default: ON)
- ``-DVANILLAPDF_ENABLE_JPEG2000=ON/OFF`` — JPEG2000 support (default: ON)
- ``-DVANILLAPDF_ENABLE_ZLIB=ON/OFF`` — zlib (Flate) compression support (default: ON)
- ``-DVANILLAPDF_ENABLE_MIMALLOC=ON/OFF`` — Serve the reference counted objects from mimalloc instead of the default allocator (default: OFF)
- ``-DVANILLAPDF_ENABLE_CRTDBG=ON/OFF`` — Track allocations through the debug heap of the Microsoft C runtime in the Debug configuration; cannot be combined with mimalloc (default: OFF)

Build configuration options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_INTERNAL_VCPKG=ON/OFF`` — Enable internal vcpkg dependency management (default: ON)
- ``-DVANILLAPDF_ENABLE_PACKAGING=ON/OFF`` — Enable packaging features like CPack (default: OFF)
- ``-DVANILLAPDF_ENABLE_TESTS=ON/OFF`` — Build and run test scenarios (default: OFF)
- ``-DVANILLAPDF_ENABLE_BENCHMARK=ON/OFF`` — Include benchmarking project (default: OFF)
- ``-DVANILLAPDF_USE_STATIC_CRT=ON/OFF`` — Use static MSVC runtime (/MT) instead of dynamic (/MD) (default: OFF)
- ``-DBUILD_SHARED_LIBS=ON/OFF`` — Build vanillapdf as a shared library (default: ON)

.. _development-options:

Development options
^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_ENABLE_COVERAGE=ON`` — Enable code coverage instrumentation (GCC/Clang only)
- ``-DVANILLAPDF_FORCE_32_BIT=ON`` — Force 32-bit output binary regardless of architecture
- ``-DVANILLAPDF_ENABLE_ASAN=ON`` — Enable AddressSanitizer (use with Debug builds)
- ``-DVANILLAPDF_ENABLE_UBSAN=ON`` — Enable UndefinedBehaviorSanitizer (use with Debug builds)
- ``-DVANILLAPDF_ENABLE_TSAN=ON`` — Enable ThreadSanitizer (use with Debug builds)
- ``-DVANILLAPDF_ENABLE_FUZZING=ON`` — Build the libFuzzer fuzz targets (Clang only, see :ref:`fuzzing`)

ASan and TSan cannot be enabled simultaneously. UBSan can be combined with either.

External dependency options
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

- ``-DVANILLAPDF_EXTERNAL_OPENSSL=ON/OFF`` — Use system OpenSSL instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_JPEG=ON/OFF`` — Use system libjpeg instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_OPENJPEG=ON/OFF`` — Use system OpenJPEG instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_ZLIB=ON/OFF`` — Use system zlib instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_SPDLOG=ON/OFF`` — Use system spdlog instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_FMT=ON/OFF`` — Use system fmt instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_NLOHMANN_JSON=ON/OFF`` — Use system nlohmann-json instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_MIMALLOC=ON/OFF`` — Use system mimalloc instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_GTEST=ON/OFF`` — Use system GoogleTest instead of vcpkg (default: OFF)
- ``-DVANILLAPDF_EXTERNAL_BENCHMARK=ON/OFF`` — Use system Google Benchmark instead of vcpkg (default: OFF)

OpenSSL legacy provider
-----------------------

Password protected documents need no OpenSSL configuration. RC4 and MD5, which
the standard security handler uses up to revision 4, are implemented in-house,
so they work with a shared OpenSSL, in FIPS mode and under restrictive system
crypto policies alike.

A few remaining algorithms are provided only by the OpenSSL 3 *legacy*
provider:

- RC2 envelopes of certificate encrypted documents, as written by Apache PDFBox
  and iText 5
- PKCS#12 key files protected with RC2 or RC4, the OpenSSL 1.x export default
- MD2 and MD4 signatures

The library loads the legacy provider by default. Two switches control it:

- **At build time**, ``-DVANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER=OFF`` leaves
  the provider out entirely, when building the library from source.
- **At runtime**, the legacy algorithms policy of :cpp:class:`CryptoSettingsHandle`, applied
  with :cpp:func:`CryptoUtils_Initialize <CryptoUtils::CryptoUtils_Initialize>` before any other call.
  The policies are the values of :cpp:enum:`LegacyAlgorithmsPolicyType`. When the
  provider cannot be loaded under ``Required``, the call fails with
  ``VANILLAPDF_ERROR_NOT_SUPPORTED``.

.. list-table::
   :header-rows: 1
   :class: wrap

   * - Policy
     - Legacy provider
     - When it cannot be loaded
   * - :cpp:enumerator:`Optional <LegacyAlgorithmsPolicyType::LegacyAlgorithmsPolicy_Optional>` (default)
     - Loaded
     - A warning is logged and the library continues without it
   * - :cpp:enumerator:`Required <LegacyAlgorithmsPolicyType::LegacyAlgorithmsPolicy_Required>`
     - Loaded
     - The initialization fails
   * - :cpp:enumerator:`Disabled <LegacyAlgorithmsPolicyType::LegacyAlgorithmsPolicy_Disabled>`
     - Not loaded
     - \-

.. code-block:: c

   CryptoSettingsHandle* settings = NULL;
   CryptoSettings_Create(&settings);
   CryptoSettings_SetLegacyAlgorithmsPolicy(settings, LegacyAlgorithmsPolicy_Required);
   CryptoUtils_Initialize(settings);
   CryptoSettings_Release(settings);

Without the legacy provider RC2 envelopes and key files fail with
``VANILLAPDF_ERROR_NOT_SUPPORTED``, while MD2 and MD4 signatures do not verify.
:cpp:func:`CryptoUtils_GetLegacyAlgorithmsAvailable <CryptoUtils::CryptoUtils_GetLegacyAlgorithmsAvailable>` reports whether it is loaded. A legacy key file can be re-exported with AES:

.. code-block:: bash

   openssl pkcs12 -legacy -in old.pfx -nodes | openssl pkcs12 -export -out new.pfx

A static OpenSSL, as in the NuGet packages, compiles the provider into libcrypto,
so it always loads. A shared OpenSSL loads it as a separate module
(``legacy.dll``, ``legacy.so``) from a directory fixed when OpenSSL was built.
Linux distributions and Homebrew ship it there. The vcpkg OpenSSL does not have
a usable directory compiled in, so applications using a dynamic vcpkg triplet
ship the module from the triplet's ``bin`` directory and point the
``OPENSSL_MODULES`` environment variable at it. A Windows installation of the
library built against a dynamic vcpkg triplet (``cmake --install``) bundles
OpenSSL together with the module in its ``bin`` directory, which is then the
directory to point ``OPENSSL_MODULES`` at.

The library loads its providers into OpenSSL's default library context, which
the application shares when it links the same OpenSSL. Legacy algorithms the
application enables itself, in code or through an OpenSSL configuration file
(``OPENSSL_CONF``), are available to the library under every policy.
