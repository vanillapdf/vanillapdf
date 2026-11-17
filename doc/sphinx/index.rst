Vanilla.PDF documentation
=========================

Vanilla.PDF is a cross-platform C++17 library for creating, modifying, signing
and encrypting PDF documents. It is used through an ABI-stable C API, so it can
be called from C, C++ and any language with a C foreign function interface.

Start here
----------

- :doc:`quickstart` — build and run a program that creates a PDF document
- :doc:`installation` — install the library with vcpkg, CMake FetchContent or NuGet
- :doc:`overview` — what the library does, and what it does not
- :doc:`c_api` — handles, memory management and error handling in the C API
- :doc:`api/documents` — the API reference, starting with documents and pages

The source code, issue tracker and releases are on
`GitHub <https://github.com/vanillapdf/vanillapdf>`_. Vanilla.PDF is licensed
under the Apache License 2.0; see :doc:`licensing`.

.. toctree::
   :maxdepth: 2
   :caption: Getting started

   overview
   quickstart
   installation
   licensing

.. toctree::
   :maxdepth: 2
   :caption: Developer guide

   building
   c_api
   architecture
   signature_verification
   packaging

.. toctree::
   :maxdepth: 2
   :caption: Learning

   pdf_format
   examples

.. toctree::
   :maxdepth: 2
   :caption: API reference

   api/documents
   api/files
   api/contents
   api/utils
   api/types
   api/deprecated
