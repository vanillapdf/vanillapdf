C API guide
===========

Vanilla.PDF is written in C++17 but exposes only an **ANSI C interface**,
ensuring ABI stability across compilers and a binding surface for any language
with a C foreign function interface.

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

All objects are represented as **opaque handles** — typed pointers to internal
C++ structures. You never access fields directly; all interaction goes through
API functions.

Common handle types:

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 35 65

   * - Handle
     - Represents
   * - :cpp:class:`DocumentHandle`
     - A PDF document (create, open, save, sign)
   * - :cpp:class:`FileHandle`
     - Low-level file access (XRef, trailers, objects)
   * - :cpp:class:`CatalogHandle`
     - The document root (page tree, outlines, names)
   * - :cpp:class:`PageTreeHandle`
     - The page tree node containing all pages
   * - :cpp:class:`PageObjectHandle`
     - A single page (content stream, media box, annotations)
   * - :cpp:class:`InteractiveFormHandle` / :cpp:class:`FieldHandle`
     - AcroForm tree and individual fields (read/write values)
   * - :cpp:class:`FileStructureValidationResultHandle`
     - Xref, trailer and stream issues reported by :cpp:func:`FileStructureValidator_Validate <FileStructureValidator::FileStructureValidator_Validate>`
   * - :cpp:class:`PKCS12KeyHandle`
     - A PKCS#12 key for signing
   * - :cpp:class:`SigningKeyHandle`
     - Abstract signing interface (for custom/smart card signing)
   * - :cpp:class:`TrustedCertificateStoreHandle`
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

A missing ``_Release`` call keeps the object, and every object it references,
alive. :cpp:func:`ObjectDiagnostics_GetActiveObjectCount <ObjectDiagnostics::ObjectDiagnostics_GetActiveObjectCount>`
returns the number of live library objects in the process. Read it before and
after a unit of work that releases all of its handles: a higher count
afterwards points at a leaked handle. The count covers the whole process, so
take both readings while no other thread uses the library.

To find where the leaked memory was allocated, use LeakSanitizer
(``-fsanitize=address`` with GCC or Clang on Linux), or on Windows the debug
heap of the C runtime, which a source build supports with
``VANILLAPDF_ENABLE_CRTDBG`` (see :doc:`building`).

Text strings
------------

PDF text strings are not bare UTF-8: they use PDFDocEncoding by default, and
UTF-16BE or (since PDF 2.0) UTF-8 when prefixed with a byte order mark.
Functions that document a UTF-8 result, such as
:cpp:func:`Field_GetQualifiedName <FieldHandle::Field_GetQualifiedName>`,
normalize the text for you. To interpret the raw bytes of a string object,
:cpp:func:`TextStringEncoding_Detect` reports which encoding they use and
:cpp:func:`TextStringEncoding_PDFDocEncodingByteToUnicode` maps a
PDFDocEncoding byte to its Unicode code point.

Error handling
--------------

Every function returns ``error_type``. On success, the value is
``VANILLAPDF_ERROR_SUCCESS``. Any other value indicates an error whose message
is stored in a thread-local buffer.

Retrieve the last error of the calling thread:

.. code-block:: c

   error_type error = VANILLAPDF_ERROR_SUCCESS;
   size_type message_length = 0;

   Errors_GetLastError(&error);

   /* The length includes the terminating NUL character */
   Errors_GetLastErrorMessageLength(&message_length);

   if (message_length > 0) {
       char* message = (char*) malloc(message_length);

       if (message != NULL && Errors_GetLastErrorMessage(message, message_length) == VANILLAPDF_ERROR_SUCCESS) {
           fprintf(stderr, "Error %u: %.*s\n", (unsigned) error, (int) message_length, message);
       }

       free(message);
   }

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
     - File not yet initialized (call :cpp:func:`File_Initialize <FileHandle::File_Initialize>` first)
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

Any handle can be shared between threads without caller-side locking, and the
last error is stored per thread, so a failing call on one thread never
overwrites another thread's error. See :ref:`thread-safety` for the details.

Versioning
----------

Vanilla.PDF follows `Semantic Versioning <https://semver.org/>`_. The C API is
stable within a major version: minor releases add functions without removing or
changing existing ones, patch releases contain only fixes.

Query the version at runtime:

.. code-block:: c

   integer_type major = 0;
   integer_type minor = 0;
   integer_type patch = 0;

   LibraryInfo_GetVersionMajor(&major);
   LibraryInfo_GetVersionMinor(&minor);
   LibraryInfo_GetVersionPatch(&patch);

   printf("Vanilla.PDF %d.%d.%d\n", (int) major, (int) minor, (int) patch);

.. list-table::
   :header-rows: 1
   :widths: 35 65

   * - Function
     - Returns
   * - :cpp:func:`LibraryInfo_GetVersionMajor <LibraryInfo::LibraryInfo_GetVersionMajor>`
     - Major version (backward-incompatible changes)
   * - :cpp:func:`LibraryInfo_GetVersionMinor <LibraryInfo::LibraryInfo_GetVersionMinor>`
     - Minor version (new features, backward-compatible)
   * - :cpp:func:`LibraryInfo_GetVersionPatch <LibraryInfo::LibraryInfo_GetVersionPatch>`
     - Patch version (bug fixes only)
   * - :cpp:func:`LibraryInfo_GetVersionBuild <LibraryInfo::LibraryInfo_GetVersionBuild>`
     - Build metadata (no functional impact)
   * - :cpp:func:`LibraryInfo_GetAuthor <LibraryInfo::LibraryInfo_GetAuthor>`
     - Library author name
   * - :cpp:func:`LibraryInfo_GetBuildDay <LibraryInfo::LibraryInfo_GetBuildDay>`
     - Day of month when library was compiled
   * - :cpp:func:`LibraryInfo_GetBuildMonth <LibraryInfo::LibraryInfo_GetBuildMonth>`
     - Month when library was compiled
   * - :cpp:func:`LibraryInfo_GetBuildYear <LibraryInfo::LibraryInfo_GetBuildYear>`
     - Year when library was compiled

Debugging
---------

Enable diagnostic logging to get internal trace output:

.. code-block:: c

   #include "vanillapdf/utils/c_logging.h"

   Logging_SetSeverity(LoggingSeverity_Debug);

Log levels from least to most verbose: ``Off``, ``Critical``, ``Error``,
``Warning``, ``Info``, ``Debug``, ``Trace``.

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

:cpp:func:`Document_Sign <DocumentHandle::Document_Sign>` creates an invisible signature field of its own on the
first page.

**Sign a visible signature field:**

A visible signature is a signature field whose widget annotation sits on a
page with a rectangle and an appearance. The field and the widget are
separate objects: the field joins the form's field hierarchy and carries the
signature, the widget is linked to it through :cpp:func:`Field_AddWidget <FieldHandle::Field_AddWidget>` and to the
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

``field_name`` is a :cpp:class:`StringObjectHandle` holding the partial field name
(``Signature1`` here). The field sits at the root of the hierarchy, so the
same name is its fully qualified name, which :cpp:func:`Document_SignField <DocumentHandle::Document_SignField>` resolves
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
