Vanilla.PDF documentation
=========================

Vanilla.PDF is an open-source C++17 library with a plain C API for creating,
editing, signing and validating PDF documents, usable across multiple
languages through a C foreign function interface (FFI).

Start here
----------

- :doc:`quickstart` — build and run a program that creates a PDF document
- :doc:`installation` — install the library with vcpkg, CMake FetchContent or NuGet
- :doc:`overview` — what the library does, and what it does not
- :doc:`c_api` — handles, memory management and error handling in the C API
- :doc:`api/documents` — the API reference, starting with documents and pages

The source code, issue tracker and releases are on
`GitHub <https://github.com/vanillapdf/vanillapdf>`_.

.. toctree::
   :maxdepth: 1
   :caption: Getting started

   overview
   quickstart
   installation
   licensing

.. toctree::
   :maxdepth: 1
   :caption: Developer guide

   building
   c_api
   architecture
   signature_verification
   packaging

.. toctree::
   :maxdepth: 1
   :caption: Learning

   pdf_format
   examples

.. toctree::
   :maxdepth: 1
   :caption: API reference

   api/documents
   api/files
   api/contents
   api/utils
   api/types
   api/deprecated
