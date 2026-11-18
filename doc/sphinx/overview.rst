Overview
========

**Vanilla.PDF** is an open-source C++17 library with a plain C API for creating,
editing, signing and validating PDF documents, usable across multiple
languages through a C foreign function interface (FFI). It is licensed
under the permissive Apache 2.0 license, suitable for both private and
commercial use.

**Using .NET?** See `vanillapdf.net <https://github.com/vanillapdf/vanillapdf.net>`_,
the managed .NET wrapper with its own NuGet package and samples. This
documentation covers the native C API.

- **Create & modify** — pages, text, images, vector graphics, form fields
- **Sign & verify** — CMS/PKCS#7 signatures with certificate chain validation
- **Encrypt & decrypt** — AES or RC4, with passwords or certificates
- **Stable ABI** — no breaking changes within a major version (see `Versioning and compatibility`_)
- **Thread-safe** — share any object across threads; synchronization is handled internally (see :ref:`thread-safety`)
- **Hardened** — fuzzed and tested under ASan, UBSan and TSan every week

Where to go next
----------------

**Getting started** — install or build the library and create your first PDF:

- :doc:`installation` — All package manager options with copy-paste commands
- :doc:`building` — Clone, configure and build with the CMake presets
- :doc:`quickstart` — Build and run a first program, then create a PDF document

**Developer guide** — understand the internals and the build:

- :doc:`architecture` — Three-layer design, object model, memory model, thread safety
- :doc:`c_api` — Handle system, memory management, error handling
- :doc:`build_options` — Dependencies, feature and development options
- :doc:`testing` — Tests, test data, fuzzing and coverage

**Learning** — understand the PDF format and signatures, and see examples:

- :doc:`pdf_format` — PDF syntax, objects, and document structure
- :doc:`signature_verification` — Trust stores, chain validation, weak-algorithm detection
- :doc:`examples` — Code samples showing the C API in use

Scope and non-goals
--------------------

Vanilla.PDF is a **document structure library**. It operates on the PDF object
graph — creating pages, embedding images, attaching digital signatures,
encrypting content, and parsing the binary format down to individual XRef
entries. It is not a rendering engine.

**What it does:**

- Create, open, modify, and save PDF documents
- Validate PDF file structure (xref, trailers, streams) and report issues
- Add and verify CMS digital signatures with RSA, ECDSA (P-256/P-384/P-521), and Ed25519; verify Ed448 signatures
- Encrypt and decrypt with AES/RC4 (password and certificate-based); compatible with FIPS-enabled OpenSSL
- Fill and read interactive form fields
- Extract embedded images (JPEG, JPEG2000)
- Parse and encode content streams (the operators that draw a page)
- Low-level access to XRef tables, indirect objects, and file trailers

**What it does not do:**

- Rasterize or render PDF pages to images or screen
- Lay out text with font shaping or line breaking
- Validate PAdES compliance levels (BES, T, LTV)
- Perform CRL or OCSP revocation checking (planned: `#157 <https://github.com/vanillapdf/vanillapdf/issues/157>`_)
- Validate RFC 3161 timestamps

Functionality
-------------

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 30 70

   * - Operation
     - Description
   * - **Create documents**
     - Generate PDFs with pages, text, images, and vector paths
   * - **Digital signatures**
     - OpenSSL CMS signatures with RSA, ECDSA (P-256/P-384/P-521), and Ed25519 using PKCS#12 keys or custom callbacks
   * - **Signature verification**
     - Validate signatures with chain validation, weak-algorithm detection, and signing-time checks
   * - **File structure validation**
     - Walk xref tables, trailers, and streams to detect malformed or corrupt files
   * - **Interactive forms**
     - Read and write AcroForm field values (text, choice, checkbox, radio button)
   * - **Merge documents**
     - Combine multiple PDFs into a single file
   * - **Encryption / decryption**
     - AES and RC4 with owner/user passwords; certificate-based decryption; compatible with FIPS-enabled OpenSSL
   * - **Image extraction**
     - Export embedded JPEG and JPEG2000 images from PDF streams
   * - **Content stream processing**
     - Parse and encode PostScript-style page content; apply compression filters
   * - **Low-level parsing**
     - Inspect XRef tables, indirect objects, cross-reference streams, and file trailers

Design philosophy
-----------------

**Why a C API over C++?** The C++ ABI is not stable across compilers or even
across major versions of the same compiler. A native C++ interface would force
every consumer to use the exact same toolchain. By exposing only ANSI C
functions with ``cdecl`` calling conventions, Vanilla.PDF can be linked by any
C or C++ compiler and called from any language with a C FFI — Python, C#,
Rust, Go, Java (via JNI), and others.

**Handle-based design.** All objects are represented as opaque pointers
(:cpp:class:`DocumentHandle* <DocumentHandle>`, :cpp:class:`FileHandle* <FileHandle>`, :cpp:class:`PageObjectHandle* <PageObjectHandle>`). The internal C++
class layout is never visible to callers, which means the library can change its
implementation without breaking binary compatibility.

**Reference-counted ownership.** Handles use intrusive reference counting.
When you receive a handle through an output parameter, you own one reference and
must release it when done. There are no hidden shared-ownership surprises:
every ``_Release`` call is explicit.

**No library context to manage.** There is no context object to create or pass
around, and no initialization or shutdown call is required: optional subsystems
such as cryptography and logging configure themselves with defaults on first
use. Error codes and messages are stored per thread, so concurrent calls never
overwrite each other's error state.

Architecture overview
---------------------

The library is organized into three layers:

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 20 80

   * - Layer
     - Responsibility
   * - **Syntax**
     - PDF object types (boolean, integer, string, name, array, dictionary,
       stream), tokenizer, parser, XRef tables, compression filters
       (Flate, DCT, JPX, ASCII85, ASCIIHex, LZW)
   * - **Semantics**
     - High-level document model: documents, catalogs, page trees, pages,
       annotations, interactive forms, digital signatures, outlines,
       named destinations, fonts, and resource dictionaries
   * - **Contents**
     - Content stream parsing and instruction processing for PostScript-style
       page content (text operations, graphics state, inline images)

A thin C interface layer wraps the C++ implementation, mapping each internal
object to an opaque handle. See :doc:`architecture` for the full design
description including the object model, memory model, and extension points.

Versioning and compatibility
----------------------------

Vanilla.PDF follows `Semantic Versioning <https://semver.org/>`_:

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 20 80

   * - Component
     - Meaning
   * - **Major**
     - Backward-incompatible changes to the C API
   * - **Minor**
     - New functionality, backward-compatible with existing callers
   * - **Patch**
     - Bug fixes only, no interface changes
   * - **Build**
     - No functional impact (CI metadata)

The C API is stable within a major version. Code compiled against 2.0 will
continue to link and run correctly against 2.1, 2.2, and so on. Query the
version at runtime:

.. code-block:: c

   integer_type major, minor, patch;
   LibraryInfo_GetVersionMajor(&major);
   LibraryInfo_GetVersionMinor(&minor);
   LibraryInfo_GetVersionPatch(&patch);

Nightly builds use a patch override (999) and a build suffix
(e.g. ``-nightly.main``) to distinguish them from release builds.

Optional dependencies
---------------------

All dependencies are managed automatically via vcpkg. Each can be replaced
with a system package, and the first four can also be turned off with the
:ref:`feature options <feature-options>`:

.. list-table::
   :header-rows: 1
   :class: wrap
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
   * - fmt
     - String formatting
     - ``EXTERNAL_FMT``
   * - nlohmann-json
     - License metadata (optional)
     - ``EXTERNAL_NLOHMANN_JSON``
   * - mimalloc
     - Faster allocator (optional)
     - ``EXTERNAL_MIMALLOC``

nlohmann-json and mimalloc are only used when ``VANILLAPDF_ENABLE_LICENSING`` or
``VANILLAPDF_ENABLE_MIMALLOC`` is on; both are off by default.

.. _supported-platforms:

Supported platforms
-------------------

Every platform below is built and tested by the nightly CI:

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 15 30 30 25

   * - Platform
     - Versions
     - Compilers
     - Architectures
   * - Windows
     - Windows Server 2022, 2025; Windows 11
     - Visual Studio 2026, 2022
     - x86, x64, ARM64
   * - Ubuntu
     - 22.04, 24.04, 26.04
     - GCC 11, 13, 15; Clang 21
     - x64, ARM64
   * - Rocky Linux
     - 8, 9, 10
     - GCC 8, 11, 14
     - x64, ARM64
   * - Fedora
     - 44, 45
     - GCC 16
     - x64, ARM64
   * - macOS
     - 15, 26, 27
     - AppleClang 17+ (Xcode 16.4+)
     - x64, ARM64
   * - Android
     - API level 28, 36
     - NDK r28+
     - x86_64

- The oldest compiler tested is GCC 8.5, on Rocky Linux 8.
- Clang is tested on Ubuntu 26.04 x64 only, by the fuzzing workflow.
- Ubuntu 22.04 is tested on x64 only.
- macOS 27 is tested on ARM64 only, because it does not run on Intel Macs.
- Windows ARM64 is tested on Windows 11.
- The library targets Android API level 28 and later. Presets for arm64-v8a,
  armeabi-v7a and x86 are available, but the CI builds and tests x86_64 only.

Package availability
--------------------

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 25 40 35

   * - Format
     - Install command
     - Notes
   * - :ref:`vcpkg <install-vcpkg>`
     - ``vcpkg install vanillapdf``
     - Recommended
   * - :ref:`FetchContent <install-fetchcontent>`
     - ``FetchContent_Declare``
     - No external tools; caller manages dependencies
   * - :ref:`Debian <create-packages>`
     - ``dpkg -i vanillapdf_*.deb``
     -
   * - :ref:`RPM <create-packages>`
     - ``rpm -i vanillapdf_*.rpm``
     -
   * - :doc:`Source <building>`
     - ``cmake --preset <platform>``
     -

See :doc:`installation` for the full instructions of each method. Conan
Center and Homebrew packages are in progress and not yet available; see
:ref:`upcoming-packages`.

External resources
------------------

- PDF specification: `ISO 32000-1 published by Adobe <https://opensource.adobe.com/dc-acrobat-sdk-docs/pdfstandards/PDF32000_2008.pdf>`_ |
  `local copy <_static/PDF32000_2008.pdf>`_
- `GitHub repository <https://github.com/vanillapdf/vanillapdf>`_
- `Website <https://vanillapdf.com>`_
