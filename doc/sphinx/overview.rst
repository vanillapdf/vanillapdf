Overview
========

**Vanilla.PDF** is a modern, open-source C++17 SDK for creating, editing,
signing, and parsing PDF documents. Exposed through an ABI-stable C API, it
compiles natively on Windows, Linux, macOS, and Android with no external
runtime dependencies.

Functionality
-------------

.. list-table::
   :header-rows: 1
   :widths: 28 18 54

   * - Operation
     - Interface
     - Description
   * - **Create documents**
     - C API
     - Generate PDFs with pages, text, images, and vector paths
   * - **Digital signatures**
     - C API, CLI
     - Add CMS (PKCS#7) signatures using PKCS#12 keys or custom callbacks
   * - **Signature verification**
     - C API, CLI
     - Validate signatures with chain validation, weak-algorithm detection, and signing-time checks
   * - **Merge documents**
     - CLI
     - Combine multiple PDFs into a single file
   * - **Encryption / decryption**
     - CLI
     - AES and RC4 with owner/user passwords; certificate-based decryption
   * - **Image extraction**
     - CLI
     - Export embedded JPEG and JPEG2000 images from PDF streams
   * - **Content stream processing**
     - C API, CLI
     - Parse and encode PostScript-style page content; apply compression filters
   * - **Low-level parsing**
     - C API
     - Inspect XRef tables, indirect objects, cross-reference streams, and file trailers

Optional dependencies
---------------------

All dependencies are managed automatically via vcpkg. Each can be disabled or
replaced with a system package:

.. list-table::
   :header-rows: 1
   :widths: 25 55 20

   * - Library
     - Purpose
     - CMake flag
   * - OpenSSL
     - Encryption, decryption, digital signatures
     - ``EXTERNAL_OPENSSL``
   * - libjpeg-turbo
     - JPEG image decoding
     - ``EXTERNAL_JPEG``
   * - OpenJPEG
     - JPEG2000 image support
     - ``EXTERNAL_OPENJPEG``
   * - zlib
     - Flate compression of PDF objects
     - ``EXTERNAL_ZLIB``
   * - spdlog
     - Diagnostic logging
     - ``EXTERNAL_SPDLOG``
   * - nlohmann-json
     - Configuration file parsing
     - ``EXTERNAL_NLOHMANN_JSON``

Supported platforms
-------------------

.. list-table::
   :header-rows: 1
   :widths: 20 40 40

   * - Platform
     - Compilers
     - Architectures
   * - Windows
     - Visual Studio 2022 (MSVC 17.x), Visual Studio 2026 (MSVC 18.x)
     - x86, x64
   * - Linux
     - GCC 8.1+, Clang 10+
     - x64, ARM64, ARM
   * - macOS
     - AppleClang 15+ (Xcode 15)
     - x64, ARM64
   * - Android
     - NDK toolchain
     - arm64-v8a, armeabi-v7a, x86, x86_64

Package availability
--------------------

.. list-table::
   :header-rows: 1
   :widths: 25 40 35

   * - Format
     - Install command
     - Notes
   * - vcpkg
     - ``vcpkg install vanillapdf``
     - Recommended; pre-built binaries
   * - FetchContent
     - ``FetchContent_Declare(vanillapdf ...)``
     - No external tools; manages deps yourself
   * - Conan
     - ``conan install --requires="vanillapdf/2.3.0"``
     - Via Conan Center
   * - Homebrew
     - ``brew install vanillapdf``
     - macOS; uses system dependencies
   * - NuGet
     - ``dotnet add package vanillapdf.net``
     - .NET interop with native runtime
   * - Debian
     - ``dpkg -i vanillapdf_*.deb``
     - Built via :doc:`packaging`
   * - Source
     - ``cmake --preset <platform>``
     - See :doc:`building`

Where to go next
----------------

**Getting started** -- install the library and create your first PDF:

- :doc:`quickstart` -- Create a PDF document in a few lines of C code
- :doc:`installation` -- All package manager options with copy-paste commands

**Developer guide** -- build from source and understand the internals:

- :doc:`building` -- Clone, configure, build, and test
- :doc:`c_api` -- Handle system, memory management, error handling
- :doc:`architecture` -- Internal design, parser, and object ownership
- :doc:`cli_tools` -- sign, verify, merge, extract, encrypt, decrypt

**Learning** -- understand the PDF format and see examples:

- :doc:`pdf_format` -- PDF syntax, objects, and document structure
- :doc:`examples` -- Code samples from the test suite and CLI tools

External resources
------------------

- Adobe PDF specification: `official site <http://www.adobe.com/devnet/pdf/pdf_reference.html>`_ |
  `local copy <_static/PDF32000_2008.pdf>`_
- `GitHub repository <https://github.com/vanillapdf/vanillapdf>`_
- `Website <https://vanillapdf.com>`_
