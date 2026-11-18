Vanilla.PDF documentation
=========================

Vanilla.PDF is an open-source C++17 library with a plain C API for creating,
editing, signing and validating PDF documents, usable across multiple
languages through a C foreign function interface (FFI).

**Using .NET?** See `vanillapdf.net <https://github.com/vanillapdf/vanillapdf.net>`_,
the managed .NET wrapper with its own NuGet package and samples. This
documentation covers the native C API.

Start here
----------

- :doc:`installation` — install the library with vcpkg or CMake FetchContent
- :doc:`building` — build the library from source with the CMake presets
- :doc:`quickstart` — build and run a first program, then create a PDF document
- :doc:`overview` — what the library does, and what it does not
- :doc:`c_api` — handles, memory management and error handling in the C API
- :doc:`api/documents` — the API reference, starting with documents and pages

The source code, issue tracker and releases are on
`GitHub <https://github.com/vanillapdf/vanillapdf>`_.

.. toctree::
   :maxdepth: 1
   :caption: Getting started

   overview
   installation
   building
   quickstart
   licensing

.. toctree::
   :maxdepth: 1
   :caption: Developer guide

   architecture
   c_api
   build_options
   testing

.. toctree::
   :maxdepth: 1
   :caption: Learning

   pdf_format
   signature_verification
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
