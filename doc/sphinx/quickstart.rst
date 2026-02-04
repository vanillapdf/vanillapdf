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
writes it to disk.

.. code-block:: c

   #include <stdio.h>
   #include <vanillapdf/c_vanillapdf_api.h>

   int main(void) {
       DocumentHandle* document = NULL;
       CatalogHandle* catalog = NULL;
       PageTreeHandle* page_tree = NULL;
       PageObjectHandle* page = NULL;
       error_type rc;

       /* Create a new document bound to an output path */
       rc = Document_Create("output.pdf", &document);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { return 1; }

       /* Navigate to the page tree via the root catalog */
       rc = Document_GetCatalog(document, &catalog);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

       rc = Catalog_GetPages(catalog, &page_tree);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

       /* Create a blank page and append it */
       rc = PageObject_CreateFromDocument(document, &page);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

       rc = PageTree_AppendPage(page_tree, page);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

       /* Write to disk */
       rc = Document_Save(document, "output.pdf");
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

       printf("Created output.pdf successfully.\n");

   cleanup:
       if (page)      PageObject_Release(page);
       if (page_tree) PageTree_Release(page_tree);
       if (catalog)   Catalog_Release(catalog);
       if (document)  Document_Release(document);

       return (rc == VANILLAPDF_ERROR_SUCCESS) ? 0 : 1;
   }

**Key patterns to notice:**

- **Single header** -- ``<vanillapdf/c_vanillapdf_api.h>`` includes the entire C API.
- **Handle-based** -- Every object is an opaque pointer (``DocumentHandle*``,
  ``CatalogHandle*``, etc.). The internal C++ implementation is never exposed.
- **Error codes** -- Every function returns ``error_type``. Check against
  ``VANILLAPDF_ERROR_SUCCESS``.
- **Manual cleanup** -- Each handle obtained from the library must be released.
  Initialize to ``NULL`` and release in reverse order.

Build and run
-------------

Create a ``CMakeLists.txt``:

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.20)
   project(QuickStart C)

   find_package(vanillapdf CONFIG REQUIRED)

   add_executable(quickstart main.c)
   target_link_libraries(quickstart PRIVATE vanillapdf::vanillapdf)

Build:

.. code-block:: bash

   cmake -S . -B build
   cmake --build build
   ./build/quickstart    # creates output.pdf

Sign the document
-----------------

Use the CLI tool to add a digital signature:

.. code-block:: bash

   vanillapdf-tools sign -s output.pdf -d signed.pdf -k private_key.p12 -p password

Verify it:

.. code-block:: bash

   vanillapdf-tools verify -f signed.pdf

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
- :doc:`cli_tools` -- All CLI commands: sign, verify, merge, extract, encrypt, decrypt
- :doc:`pdf_format` -- Understand PDF syntax: objects, XRef tables, trailers
