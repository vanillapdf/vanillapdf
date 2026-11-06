Quickstart
==========

Create your first PDF document in a few lines of C code.

Prerequisites
-------------

Install Vanilla.PDF using one of the methods in the
:doc:`installation guide <installation>`. The simplest option:

.. code-block:: bash

   vcpkg install vanillapdf

Create a PDF document
---------------------

The following example creates a PDF document with a single blank page and
writes it to disk. Both files are in `examples/quickstart/
<https://github.com/vanillapdf/vanillapdf/tree/main/examples/quickstart>`_,
and the library's test suite builds and runs the program.

.. literalinclude:: ../../examples/quickstart/main.c
   :language: c

**Key patterns to notice:**

- **Single header** -- ``<vanillapdf/c_vanillapdf_api.h>`` includes the entire C API.
- **Handle-based** -- Every object is an opaque pointer (:cpp:class:`DocumentHandle* <DocumentHandle>`,
  :cpp:class:`CatalogHandle* <CatalogHandle>`, etc.). The internal C++ implementation is never exposed.
- **Error codes** -- Every function returns ``error_type``. Check against
  ``VANILLAPDF_ERROR_SUCCESS``; the ``CHECK`` macro jumps to the cleanup on
  the first failure, where :cpp:func:`Errors_GetLastErrorMessage <Errors::Errors_GetLastErrorMessage>` explains it.
- **Manual cleanup** -- Each handle obtained from the library must be released.
  Initialize to ``NULL`` and release in reverse order.

Build and run
-------------

Create a ``CMakeLists.txt``:

.. literalinclude:: ../../examples/quickstart/CMakeLists.txt
   :language: cmake

Build:

.. code-block:: bash

   cmake -S . -B build
   cmake --build build
   ./build/quickstart    # creates output.pdf

Sign the document
-----------------

Digital signatures are applied through the same C API: load a PKCS#12 key
with :cpp:func:`PKCS12Key_CreateFromFile <PKCS12KeyHandle::PKCS12Key_CreateFromFile>`, configure ``DocumentSignatureSettings``
and call :cpp:func:`Document_Sign <DocumentHandle::Document_Sign>`. Signatures are checked with
:cpp:func:`DigitalSignatureExtensions_Verify`; see :doc:`signature_verification`
for a complete walkthrough of trust stores, settings and result handling.

Query library version
---------------------

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print library info]
   :end-before: //! [Print library info]
   :dedent:

Retrieve error details
----------------------

When a function returns an error code, extract the message:

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print last error]
   :end-before: //! [Print last error]
   :dedent:

Next steps
----------

- :doc:`c_api` -- Handle system, memory management, error handling, debugging
- :doc:`examples` -- Signing, merging, encryption, content stream processing
- :doc:`signature_verification` -- Verify digital signatures with trust stores
- :doc:`pdf_format` -- Understand PDF syntax: objects, XRef tables, trailers
