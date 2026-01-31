Overview
========

**Vanilla.PDF** is a modern, high-performance, open-source C++17 SDK for
creating, editing, signing, and analysing PDF documents. With no external
runtime dependencies and full cross-platform support, it is designed for
embedding into desktop, server, or automation workflows.

Key capabilities
----------------

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Feature
     - Description
   * - **Digital Signatures**
     - Add and verify CMS (PKCS#7) digital signatures
   * - **PDF Generation**
     - Create documents with text, images, paths, and pages
   * - **Advanced Editing**
     - Modify content streams, metadata, and page structure
   * - **PDF Parsing**
     - Inspect low-level internals like XRef tables and objects
   * - **Encryption**
     - Work with standard PDF security models
   * - **CLI Tools**
     - Batch-process PDFs directly from the terminal

Supported platforms
-------------------

.. list-table::
   :header-rows: 1
   :widths: 25 40 35

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

Where to go next
----------------

**Getting started** -- install the library and create your first PDF:

- :doc:`quickstart` -- Hands-on tutorial: create a PDF in a few lines of C
- :doc:`installation` -- Install via vcpkg, FetchContent, or Conan

**Developer guide** -- build from source and understand the internals:

- :doc:`building` -- Clone, configure, build, and test
- :doc:`c_api` -- Memory management, error handling, and debugging
- :doc:`architecture` -- Internal design and patterns
- :doc:`cli_tools` -- Command-line PDF processing

**Learning** -- understand the PDF format and see examples:

- :doc:`pdf_format` -- PDF syntax, objects, and document structure
- :doc:`examples` -- Code samples for common tasks

External resources
------------------

- Adobe PDF specification: `official site <http://www.adobe.com/devnet/pdf/pdf_reference.html>`_ |
  `local copy <_static/PDF32000_2008.pdf>`_
- `GitHub repository <https://github.com/vanillapdf/vanillapdf>`_
- `Website <https://vanillapdf.com>`_
