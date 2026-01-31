C API Guide
===========

Vanilla.PDF is written in C++17 but exposes only an **ANSI C interface**,
ensuring ABI stability across compilers and easy interop with other languages.

This page covers the core concepts you need to use the API effectively.

Header and linking
------------------

A single umbrella header provides the entire public API:

.. code-block:: c

   #include <vanillapdf/c_vanillapdf_api.h>

Link with CMake:

.. code-block:: cmake

   find_package(vanillapdf CONFIG REQUIRED)
   target_link_libraries(myapp PRIVATE vanillapdf::vanillapdf)

Handle system
-------------

All objects are represented as **opaque handles** -- typed pointers to internal
C++ structures. You never access fields directly; all interaction goes through
API functions.

Common handle types:

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Handle
     - Represents
   * - ``DocumentHandle``
     - A PDF document (create, open, save, sign)
   * - ``FileHandle``
     - Low-level file access (XRef, trailers, objects)
   * - ``CatalogHandle``
     - The document root (page tree, outlines, names)
   * - ``PageTreeHandle``
     - The page tree node containing all pages
   * - ``PageObjectHandle``
     - A single page (content stream, media box, annotations)
   * - ``PKCS12KeyHandle``
     - A PKCS#12 key for signing
   * - ``SigningKeyHandle``
     - Abstract signing interface (for custom/smart card signing)
   * - ``TrustedCertificateStoreHandle``
     - Certificate store for signature verification

Typical usage pattern:

.. code-block:: c

   DocumentHandle* doc = NULL;
   CatalogHandle* catalog = NULL;
   PageTreeHandle* pages = NULL;

   Document_Create("output.pdf", &doc);
   Document_GetCatalog(doc, &catalog);
   Catalog_GetPages(catalog, &pages);

   /* ... use pages ... */

   PageTree_Release(pages);
   Catalog_Release(catalog);
   Document_Release(doc);

Types
-----

All basic interface types are either opaque handle pointers or standard C value
types (integers, strings).

Since C has no built-in boolean, the library defines ``boolean_type`` with two
states: ``VANILLAPDF_RV_TRUE`` and ``VANILLAPDF_RV_FALSE``.

For the full list, see the :doc:`Types API reference <api/types>`.

Memory management
-----------------

All interface objects are **reference counted**. When you receive a handle
through an output parameter, you own a reference and must release it when done.

.. code-block:: c

   FileHandle file = NULL;
   error_type rc = File_Open("input.pdf", &file);
   if (rc != VANILLAPDF_ERROR_SUCCESS) {
       return rc;
   }

   /* ... work with the file ... */

   File_Release(file);  /* caller must release */

**Rules:**

- Always initialize handles to ``NULL`` before use.
- Release handles in reverse order of acquisition.
- Releasing the same handle twice is **undefined behavior**.

The following macro provides safe cleanup with NULL-guard:

.. code-block:: c

   #define SAFE_RELEASE(function_name, handle) \
   do { \
       if (handle == NULL) { \
           break; \
       } \
       \
       error_type __result__ = (function_name(handle)); \
       if (VANILLAPDF_ERROR_SUCCESS != __result__) { \
           exit(EXIT_FAILURE); \
       } \
       \
       handle = NULL; \
   } while(0)

Usage with ``goto`` cleanup:

.. code-block:: c

   FileHandle file = NULL;
   DocumentHandle document = NULL;

   error_type rc = File_Open(path, &file);
   if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

   rc = Document_Open(path, &document);
   if (rc != VANILLAPDF_ERROR_SUCCESS) { goto cleanup; }

   /* ... work with file and document ... */

   cleanup:
       SAFE_RELEASE(Document_Release, document);
       SAFE_RELEASE(File_Release, file);

.. note::

   The release function should not fail as long as the parameter is valid.
   The error code exists to keep the interface consistent.

Error handling
--------------

Every function returns ``error_type``. On success, the value is
``VANILLAPDF_ERROR_SUCCESS``. Any other value indicates an error whose message
is stored in a thread-local buffer.

Retrieve the last error:

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print last error]
   :end-before: //! [Print last error]
   :dedent:

Common error codes:

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - Code
     - Meaning
   * - ``VANILLAPDF_ERROR_SUCCESS``
     - Operation completed successfully
   * - ``VANILLAPDF_ERROR_PARAMETER_VALUE``
     - NULL or invalid parameter
   * - ``VANILLAPDF_ERROR_GENERAL``
     - Internal error (check error message for details)
   * - ``VANILLAPDF_ERROR_NOT_SUPPORTED``
     - Feature not available in this build

Debugging
---------

Enable diagnostic logging to get internal trace output:

.. code-block:: c

   #include "vanillapdf/c_logging.h"

   Logging_Enable();
   Logging_SetSeverity(LoggingSeverity_Debug);

Log levels from least to most verbose: ``Error``, ``Warning``, ``Info``,
``Debug``.

Common operations
-----------------

**Open an existing PDF:**

.. code-block:: c

   FileHandle* file = NULL;
   File_Open("input.pdf", &file);

   DocumentHandle* doc = NULL;
   Document_Open("input.pdf", &doc);

**Create a new PDF with a page:**

.. code-block:: c

   DocumentHandle* doc = NULL;
   CatalogHandle* cat = NULL;
   PageTreeHandle* pages = NULL;
   PageObjectHandle* page = NULL;

   Document_Create("new.pdf", &doc);
   Document_GetCatalog(doc, &cat);
   Catalog_GetPages(cat, &pages);
   PageObject_CreateFromDocument(doc, &page);
   PageTree_AppendPage(pages, page);
   Document_Save(doc, "new.pdf");

**Sign a document:**

.. code-block:: c

   PKCS12KeyHandle* pkcs12 = NULL;
   SigningKeyHandle* key = NULL;

   PKCS12Key_CreateFromFile("key.p12", "password", &pkcs12);
   PKCS12Key_ToSigningKey(pkcs12, &key);
   Document_Sign(doc, file, key, settings);

**Verify a signature:**

.. code-block:: c

   TrustedCertificateStoreHandle* store = NULL;
   TrustedCertificateStore_Create(&store);
   TrustedCertificateStore_LoadSystemDefaults(store);

   SignatureVerificationResultHandle* result = NULL;
   DigitalSignatureExtensions_Verify(sig, doc, store, settings, &result);

   SignatureVerificationStatusType status;
   SignatureVerificationResult_GetStatus(result, &status);

See :doc:`signature_verification` for the full verification guide and
:doc:`examples` for more code samples.

For the complete API reference, see :doc:`api/documents`, :doc:`api/files`,
:doc:`api/contents`, and :doc:`api/utils`.
