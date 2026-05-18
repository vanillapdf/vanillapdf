Overview
========

**Vanilla.PDF** is a modern, high-performance, open-source C++17 SDK for
creating, editing, signing, and analyzing PDF documents. With no external
runtime dependencies and full cross-platform support, it's ideal for embedding
into desktop, server, or automation workflows.

- **Create & modify** PDF documents — pages, text, images, vector graphics
- **Sign & verify** digital signatures (CMS/PKCS#7) with certificate chain validation
- **Encrypt & decrypt** with AES or RC4 using passwords or certificates
- **ABI-stable C API** — opaque handles callable from any language with a C FFI
- **Thread-safe** — no global state, process documents in parallel without locking

Where to go next
----------------

**Getting started** -- install the library and create your first PDF:

- :doc:`quickstart` -- Create a PDF document in a few lines of C code
- :doc:`installation` -- All package manager options with copy-paste commands

**Developer guide** -- build from source and understand the internals:

- :doc:`building` -- Clone, configure, build, and test
- :doc:`c_api` -- Handle system, memory management, error handling
- :doc:`architecture` -- Three-layer design, object model, memory model
- :doc:`signature_verification` -- Trust stores, chain validation, weak-algorithm detection
- :doc:`cli_tools` -- sign, verify, merge, extract, encrypt, decrypt

**Learning** -- understand the PDF format and see examples:

- :doc:`pdf_format` -- PDF syntax, objects, and document structure
- :doc:`examples` -- Code samples from the test suite and CLI tools

Scope and non-goals
--------------------

Vanilla.PDF is a **document structure library**. It operates on the PDF object
graph -- creating pages, embedding images, attaching digital signatures,
encrypting content, and parsing the binary format down to individual XRef
entries. It is not a rendering engine.

**What it does:**

- Create, open, modify, and save PDF documents
- Validate PDF file structure (xref, trailers, streams) and report issues
- Add and verify CMS digital signatures with RSA, ECDSA (P-256/P-384/P-521), Ed25519, and Ed448
- Encrypt and decrypt with AES/RC4 (password and certificate-based); compatible with FIPS-enabled OpenSSL
- Fill and read interactive form fields
- Extract embedded images (JPEG, JPEG2000)
- Parse and encode content streams (PostScript-style page instructions)
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
   :widths: 28 18 54

   * - Operation
     - Interface
     - Description
   * - **Create documents**
     - C API
     - Generate PDFs with pages, text, images, and vector paths
   * - **Digital signatures**
     - C API, CLI
     - OpenSSL CMS signatures with RSA, ECDSA (P-256/P-384/P-521), Ed25519, and Ed448 using PKCS#12 keys or custom callbacks
   * - **Signature verification**
     - C API, CLI
     - Validate signatures with chain validation, weak-algorithm detection, and signing-time checks
   * - **File structure validation**
     - C API, CLI
     - Walk xref tables, trailers, and streams to detect malformed or corrupt files
   * - **Interactive forms**
     - C API
     - Read and write AcroForm field values (text, choice, checkbox, radio button)
   * - **Merge documents**
     - CLI
     - Combine multiple PDFs into a single file
   * - **Encryption / decryption**
     - CLI
     - AES and RC4 with owner/user passwords; certificate-based decryption; compatible with FIPS-enabled OpenSSL
   * - **Image extraction**
     - CLI
     - Export embedded JPEG and JPEG2000 images from PDF streams
   * - **Content stream processing**
     - C API, CLI
     - Parse and encode PostScript-style page content; apply compression filters
   * - **Low-level parsing**
     - C API
     - Inspect XRef tables, indirect objects, cross-reference streams, and file trailers

Design philosophy
-----------------

**Why a C API over C++?** The C++ ABI is not stable across compilers or even
across major versions of the same compiler. A native C++ interface would force
every consumer to use the exact same toolchain. By exposing only ANSI C
functions with ``cdecl`` calling conventions, Vanilla.PDF can be linked by any
C or C++ compiler and called from any language with a C FFI -- Python, C#,
Rust, Go, Java (via JNI), and others.

**Handle-based design.** All objects are represented as opaque pointers
(``DocumentHandle*``, ``FileHandle*``, ``PageObjectHandle*``). The internal C++
class layout is never visible to callers, which means the library can change its
implementation without breaking binary compatibility.

**Reference-counted ownership.** Handles use intrusive reference counting.
When you receive a handle through an output parameter, you own one reference and
must release it when done. There are no hidden shared-ownership surprises:
every ``_Release`` call is explicit.

**No global state.** Error codes and messages are stored in thread-local
buffers, one per thread. The library does not allocate process-wide singletons
or require initialization/shutdown calls.

Architecture overview
---------------------

The library is organized into three layers:

.. list-table::
   :header-rows: 1
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

Thread safety
-------------

Vanilla.PDF is thread-safe. Key internal objects are protected by
``std::recursive_mutex``, reference counting is atomic, and error context is
stored in ``thread_local`` buffers so concurrent threads never interfere with
each other. See :doc:`architecture` for implementation details.

Versioning and compatibility
----------------------------

Vanilla.PDF follows `Semantic Versioning <https://semver.org/>`_:

.. list-table::
   :header-rows: 1
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
     - x86, x64, ARM64
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
     - No external tools; caller manages dependencies
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

External resources
------------------

- Adobe PDF specification: `official site <http://www.adobe.com/devnet/pdf/pdf_reference.html>`_ |
  `local copy <_static/PDF32000_2008.pdf>`_
- `GitHub repository <https://github.com/vanillapdf/vanillapdf>`_
- `Website <https://vanillapdf.com>`_
