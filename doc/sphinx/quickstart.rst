Quickstart
==========

Build and run a first program with Vanilla.PDF, then extend it to create a PDF
document.

Prerequisites
-------------

Install Vanilla.PDF using one of the methods in the
:doc:`installation guide <installation>`.

Your first program
------------------

The following program prints the version of the library it runs with. It is in
`examples/quickstart/
<https://github.com/vanillapdf/vanillapdf/tree/main/examples/quickstart>`_,
and the library's test suite builds and runs it.

.. literalinclude:: ../../examples/quickstart/main.c
   :language: c

``<vanillapdf/c_vanillapdf_api.h>`` includes the entire C API. Every function
returns an ``error_type``, which is ``VANILLAPDF_ERROR_SUCCESS`` when the call
succeeded.

Build and run
-------------

Create a ``CMakeLists.txt`` next to ``main.c``:

.. literalinclude:: ../../examples/quickstart/CMakeLists.txt
   :language: cmake

Build and run the program:

.. code-block:: bash
   :substitutions:

   cmake -S . -B build
   cmake --build build
   ./build/quickstart    # prints Vanilla.PDF |release|

Create a PDF document
---------------------

The next program creates a PDF document with a single blank page and writes it
to ``output.pdf``. It is in `examples/create/
<https://github.com/vanillapdf/vanillapdf/tree/main/examples/create>`_, and the
test suite runs it as well. Build it with the same ``CMakeLists.txt``, with
``create`` in place of ``quickstart``.

.. literalinclude:: ../../examples/create/main.c
   :language: c

**Key patterns to notice:**

- **Handle-based** — Every object is an opaque pointer (:cpp:class:`DocumentHandle* <DocumentHandle>`,
  :cpp:class:`CatalogHandle* <CatalogHandle>`, etc.). The internal C++ implementation is never exposed.
- **Error codes** — The ``CHECK`` macro jumps to the cleanup on the first
  failure, where :cpp:func:`Errors_GetLastErrorMessage <Errors::Errors_GetLastErrorMessage>` explains it.
- **Manual cleanup** — Each handle obtained from the library must be released.
  Initialize to ``NULL`` and release in reverse order.

Next steps
----------

- :doc:`c_api` — Handle system, memory management, error handling, debugging
- :doc:`examples` — Inspecting, merging, signing, verifying, encrypting and validating documents
- :doc:`signature_verification` — Verify digital signatures with trust stores
- :doc:`pdf_format` — Understand PDF syntax: objects, XRef tables, trailers
