Quickstart
==========

Create your first PDF document in a few lines of C code.

Prerequisites
-------------

Before starting, install Vanilla.PDF using one of the methods in the
:doc:`installation guide <installation>`.

You will need:

- A C or C++ compiler (see :doc:`building` for supported compilers)
- CMake 3.20 or later
- Vanilla.PDF headers and library

Create a PDF document
---------------------

The following example creates a minimal PDF document with a single blank page.

Step 1 -- Include the header
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: c

   #include <vanillapdf/c_vanillapdf_api.h>

The single umbrella header pulls in every part of the C API.

Step 2 -- Print library info
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print library info]
   :end-before: //! [Print library info]
   :dedent:

Step 3 -- Create a document and add a page
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: c

   DocumentHandle* document = NULL;
   CatalogHandle* catalog = NULL;
   PageTreeHandle* page_tree = NULL;
   PageObjectHandle* page = NULL;

   /* Create a new document bound to an output path */
   error_type rc = Document_Create("output.pdf", &document);
   if (rc != VANILLAPDF_ERROR_SUCCESS) { return rc; }

   /* Retrieve the root catalog */
   rc = Document_GetCatalog(document, &catalog);
   if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

   /* Access the page tree */
   rc = Catalog_GetPages(catalog, &page_tree);
   if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

   /* Create a blank page and append it */
   rc = PageObject_CreateFromDocument(document, &page);
   if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

   rc = PageTree_AppendPage(page_tree, page);
   if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

Step 4 -- Save and clean up
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: c

   /* Write the document to disk */
   rc = Document_Save(document, "output.pdf");

   cleanup:
       if (page)      PageObject_Release(page);
       if (page_tree) PageTree_Release(page_tree);
       if (catalog)   Catalog_Release(catalog);
       if (document)  Document_Release(document);

All handles obtained from the library must be released when no longer needed.
See the :doc:`C API guide <c_api>` for details on memory management.

Complete example
----------------

.. code-block:: c

   #include <stdio.h>
   #include <vanillapdf/c_vanillapdf_api.h>

   int main(void) {
       DocumentHandle* document = NULL;
       CatalogHandle* catalog = NULL;
       PageTreeHandle* page_tree = NULL;
       PageObjectHandle* page = NULL;
       error_type rc;

       rc = Document_Create("output.pdf", &document);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { return 1; }

       rc = Document_GetCatalog(document, &catalog);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

       rc = Catalog_GetPages(catalog, &page_tree);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

       rc = PageObject_CreateFromDocument(document, &page);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

       rc = PageTree_AppendPage(page_tree, page);
       if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

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

Build and run
^^^^^^^^^^^^^

Link against ``vanillapdf::vanillapdf`` in your CMakeLists.txt:

.. code-block:: cmake

   find_package(vanillapdf REQUIRED)
   add_executable(quickstart main.c)
   target_link_libraries(quickstart PRIVATE vanillapdf::vanillapdf)

Then build and run:

.. code-block:: bash

   cmake -S . -B build
   cmake --build build
   ./build/quickstart    # creates output.pdf

Next steps
----------

- :doc:`c_api` -- Memory management, error handling, and debugging
- :doc:`examples` -- More code samples for signing, merging, encryption
- :doc:`signature_verification` -- Verify digital signatures in PDF documents
- :doc:`cli_tools` -- Process PDFs from the command line
