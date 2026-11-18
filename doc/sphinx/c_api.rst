C API Guide
===========

Vanilla.PDF is written in C++17 but exposes only an **ANSI C interface**,
ensuring ABI stability across compilers and easy interop with other languages.

This page covers the core concepts you need to use the API effectively:
handles, types, memory management, error handling, and versioning.

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
   * - ``InteractiveFormHandle`` / ``FormFieldHandle``
     - AcroForm tree and individual fields (read/write values)
   * - ``FileStructureValidatorHandle``
     - Walks the PDF structure and reports xref/trailer/stream issues
   * - ``IoStrategyHandle``
     - Pluggable backend for file I/O (in-memory buffer or file stream)
   * - ``ObjectDiagnosticsHandle``
     - Per-type allocation counters for leak detection in tests
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

For the complete handle hierarchy organized by layer (syntax, semantics,
contents, utilities), see :doc:`architecture`.

Types
-----

All basic interface types are either opaque handle pointers or standard C value
types. Key type definitions from ``c_types.h``:

.. list-table::
   :header-rows: 1
   :widths: 30 30 40

   * - Type
     - C type
     - Usage
   * - ``error_type``
     - ``uint32_t``
     - Return value for all API functions
   * - ``boolean_type``
     - ``int8_t``
     - ``VANILLAPDF_RV_TRUE`` / ``VANILLAPDF_RV_FALSE``
   * - ``integer_type``
     - ``int32_t``
     - PDF integer values, version numbers
   * - ``real_type``
     - ``double``
     - PDF real number values
   * - ``size_type``
     - ``uint32_t`` or ``uint64_t``
     - Sizes and counts (platform-dependent)
   * - ``offset_type``
     - ``int64_t``
     - File byte offsets
   * - ``string_type``
     - ``const char*``
     - UTF-8 string data
   * - ``bigint_type``
     - ``int64_t``
     - Large integer values

Since C has no built-in boolean, the library defines ``boolean_type`` with two
constants: ``VANILLAPDF_RV_TRUE`` and ``VANILLAPDF_RV_FALSE``.

For the full list, see the :doc:`Types API reference <api/types>`.

Memory management
-----------------

All interface objects are **reference counted**. When you receive a handle
through an output parameter, you own a reference and must release it when done.

.. code-block:: c

   FileHandle* file = NULL;
   error_type rc = File_Open("input.pdf", &file);
   if (rc != VANILLAPDF_ERROR_SUCCESS) {
       return rc;
   }

   /* ... work with the file ... */

   File_Release(file);  /* caller must release */

**Rules:**

- Always initialize handles to ``NULL`` before use.
- Release handles in reverse order of acquisition.
- Never use a handle after releasing it.
- Releasing the same handle twice is **undefined behavior**.

The following macro provides safe cleanup with a NULL guard:

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

   FileHandle* file = NULL;
   DocumentHandle* document = NULL;

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

Leak detection
^^^^^^^^^^^^^^

For debugging missing ``_Release`` calls, the ObjectDiagnostics API reports
the live allocation count per object type. Snapshot before and after a unit
of work, compare, and any non-zero delta points at a leaked handle. See the
``ObjectDiagnostics_*`` functions in ``c_object_diagnostics.h``.

Text strings
------------

PDF text strings are not bare UTF-8; they use PDFDocEncoding by default and
UTF-16BE when prefixed with a byte-order mark. The ``c_text_string_encoding.h``
helpers (``TextStringEncoding_DetectEncoding``, ``TextStringEncoding_ToUtf8``,
``TextStringEncoding_FromUtf8``) handle detection and conversion so callers can
work with plain UTF-8 at the boundary.

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

Error codes
^^^^^^^^^^^

All error codes are defined as ``extern const error_type`` in ``c_values.h``:

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - Code
     - Meaning
   * - ``VANILLAPDF_ERROR_SUCCESS``
     - Operation completed successfully
   * - ``VANILLAPDF_ERROR_PARAMETER_VALUE``
     - NULL or invalid parameter passed to function
   * - ``VANILLAPDF_ERROR_NOT_SUPPORTED``
     - Operation not available in this build configuration
   * - ``VANILLAPDF_ERROR_USER_CANCELLED``
     - Operation cancelled by caller (e.g. via callback)
   * - ``VANILLAPDF_ERROR_ZLIB_DATA``
     - Error in compressed (zlib/Flate) data
   * - ``VANILLAPDF_ERROR_INVALID_LICENSE``
     - License file is not valid
   * - ``VANILLAPDF_ERROR_LICENSE_REQUIRED``
     - Licensed feature accessed without valid license
   * - ``VANILLAPDF_ERROR_INSUFFICIENT_SPACE``
     - Buffer too small for requested operation
   * - ``VANILLAPDF_ERROR_GENERAL``
     - Internal error; check error message for details
   * - ``VANILLAPDF_ERROR_CONVERSION``
     - Invalid object type cast (e.g. integer as string)
   * - ``VANILLAPDF_ERROR_FILE_DISPOSED``
     - File handle already disposed
   * - ``VANILLAPDF_ERROR_FILE_NOT_INITIALIZED``
     - File not yet initialized (call ``File_Initialize`` first)
   * - ``VANILLAPDF_ERROR_OBJECT_MISSING``
     - Required dependent object not found in file
   * - ``VANILLAPDF_ERROR_PARSE_EXCEPTION``
     - Low-level parsing error; document may be damaged
   * - ``VANILLAPDF_ERROR_INVALID_PASSWORD``
     - Wrong password or key for encrypted document
   * - ``VANILLAPDF_ERROR_DUPLICATE_KEY``
     - Inserting a key that already exists in a dictionary
   * - ``VANILLAPDF_ERROR_OPTIONAL_ENTRY_MISSING``
     - Requested optional entry is empty or absent
   * - ``VANILLAPDF_ERROR_SEMANTIC_CONTEXT``
     - Object's underlying type differs from expected semantic type

Thread safety
-------------

Vanilla.PDF is thread-safe. Key internal objects are protected by mutexes,
reference counting is atomic, and error context is stored in ``thread_local``
buffers so concurrent threads never interfere with each other.

See :doc:`architecture` for the full list of protected objects and
implementation details.

.. _trivial-functions:

Trivial functions
-----------------

A trivial function is a C API function that only reads or writes a value that
is already in memory. It does no other work. The API reference marks each
trivial function with a "Trivial function" note.

While a trivial function runs, it:

- does not acquire a lock and does not wait for another thread,
- does not allocate or free memory,
- does not raise an internal C++ exception,
- does not write to the log, so it never calls the callback set with
  ``Logging_SetCallbackLogger``,
- does not call any other callback that the application supplied,
- does not read files or streams, and does not parse, decode or encode data,
- does not change the last error of the thread,
- returns after a small, fixed amount of work.

A trivial function can still fail and return an error code, for example
``VANILLAPDF_ERROR_PARAMETER_VALUE`` for a ``NULL`` handle or a conversion
such as ``ButtonField_FromField`` that gets an object of a different type. It
does not store an error message, so ``Errors_GetLastError`` and
``Errors_GetLastErrorMessage`` still return the values of the last function
that stored an error. Use the return code.

What trivial does not mean:

- A trivial function adds no synchronization. If another thread calls a setter
  on the same object at the same time, the result is a data race, as with every
  other function.
- A trivial function does not check that a handle is still valid. A handle that
  was already released causes undefined behavior.
- Not every getter is trivial. Many getters take a lock (for example on
  dictionaries, streams and strings) or load data on demand.

The contract lets a language binding call these functions in a cheaper way.
For example, .NET can call a native function with the
``[SuppressGCTransition]`` attribute. The runtime then does not switch the
thread from managed to native mode, which saves a few nanoseconds per call.
This is only safe when the native function returns quickly, never blocks and
never calls back into managed code. The Vanilla.PDF .NET binding uses this
attribute for the trivial functions.

The "Trivial function" note is part of the API contract. A function that has
the note keeps it in all later versions with the same major version. Only a
new major version can remove the note, or remove the function. New functions
can get the note in a minor version.

Versioning
----------

Vanilla.PDF follows `Semantic Versioning <https://semver.org/>`_. The C API is
stable within a major version: minor releases add functions without removing or
changing existing ones, patch releases contain only fixes.

Query the version at runtime:

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print library info]
   :end-before: //! [Print library info]
   :dedent:

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Function
     - Returns
   * - ``LibraryInfo_GetVersionMajor``
     - Major version (backward-incompatible changes)
   * - ``LibraryInfo_GetVersionMinor``
     - Minor version (new features, backward-compatible)
   * - ``LibraryInfo_GetVersionPatch``
     - Patch version (bug fixes only)
   * - ``LibraryInfo_GetVersionBuild``
     - Build metadata (no functional impact)
   * - ``LibraryInfo_GetAuthor``
     - Library author name
   * - ``LibraryInfo_GetBuildDay``
     - Day of month when library was compiled
   * - ``LibraryInfo_GetBuildMonth``
     - Month when library was compiled
   * - ``LibraryInfo_GetBuildYear``
     - Year when library was compiled

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

   DocumentSignatureSettingsHandle* settings = NULL;

   PKCS12Key_CreateFromFile("key.p12", "password", &pkcs12);
   PKCS12Key_ToSigningKey(pkcs12, &key);

   DocumentSignatureSettings_Create(&settings);
   DocumentSignatureSettings_SetSigningKey(settings, key);
   DocumentSignatureSettings_SetDigest(settings, MessageDigestAlgorithmType_SHA256);

   Document_Sign(doc, destination, settings);

``Document_Sign`` creates an invisible signature field of its own on the
first page.

**Sign a visible signature field:**

A visible signature is a signature field whose widget annotation sits on a
page with a rectangle and an appearance. The field and the widget are
separate objects: the field joins the form's field hierarchy and carries the
signature, the widget is linked to it through ``Field_AddWidget`` and to the
page through its annotations.

.. code-block:: c

   SignatureFieldHandle* signature_field = NULL;
   FieldHandle* field = NULL;
   SignatureField_CreateFromDocument(doc, field_name, &signature_field);
   SignatureField_ToField(signature_field, &field);

   WidgetAnnotationHandle* widget = NULL;
   AnnotationHandle* annotation = NULL;
   WidgetAnnotation_CreateFromDocument(doc, &widget);
   WidgetAnnotation_ToBaseAnnotation(widget, &annotation);

   RectangleHandle* rect = NULL;
   Rectangle_Create(&rect);
   Rectangle_SetLowerLeftXReal(rect, 100);
   Rectangle_SetLowerLeftYReal(rect, 100);
   Rectangle_SetUpperRightXReal(rect, 300);
   Rectangle_SetUpperRightYReal(rect, 160);
   Annotation_SetRect(annotation, rect);

   /* The appearance is a form XObject the caller draws into */
   FormXObjectHandle* appearance = NULL;
   FormXObject_CreateFromDocument(doc, &appearance);
   Annotation_SetAppearance(annotation, AppearanceType_Normal, appearance);

   /* From PageObject_GetAnnotations, or PageAnnotations_Create and
      PageObject_SetAnnotations for a page without annotations */
   PageAnnotations_Append(page_annotations, annotation);
   Field_AddWidget(field, widget);

   const char* signature_name = "Signature1";
   Document_SignField(doc, destination, signature_name, strlen(signature_name), settings);

``field_name`` is a ``StringObjectHandle`` holding the partial field name
(``Signature1`` here). The field sits at the root of the hierarchy, so the
same name is its fully qualified name, which ``Document_SignField`` resolves
the field by. The name is UTF-8 with its size in bytes given explicitly,
because a partial field name may contain U+0000. A document that already carries a signature is refused with
``VANILLAPDF_ERROR_NOT_SUPPORTED``: the signed file is written whole, which
would invalidate the existing signature.

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
