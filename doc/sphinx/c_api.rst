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
``VANILLAPDF_ENABLE_CRTDBG`` (see :doc:`build_options`).

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

Logging
^^^^^^^

Logging is on by default: the library writes its log to standard output at the
``Info`` level, which includes errors, warnings and informational messages,
such as each file being opened and closed. Set the level with
:cpp:func:`Logging_SetSeverity <Logging::Logging_SetSeverity>`, from the least to the most verbose: ``Off``,
``Critical``, ``Error``, ``Warning``, ``Info``, ``Debug``, ``Trace``.

.. code-block:: c

   Logging_SetSeverity(LoggingSeverity_Off);     /* no log at all */
   Logging_SetSeverity(LoggingSeverity_Debug);   /* more detail */

To send the log elsewhere, write it to size-limited files with
:cpp:func:`Logging_SetRotatingFileLogger <Logging::Logging_SetRotatingFileLogger>`, or pass each entry to your own
logging with :cpp:func:`Logging_SetCallbackLogger <Logging::Logging_SetCallbackLogger>`. The text passed to the
callback is not terminated by a NUL character, so use its length:

.. code-block:: c

   static void CALLING_CONVENTION on_log(void* user_data, LoggingSeverity level,
       string_type payload, size_type length) {
       fprintf(stderr, "vanillapdf: %.*s\n", (int) length, payload);
   }

   static void CALLING_CONVENTION on_flush(void* user_data) {
       fflush(stderr);
   }

   Logging_SetCallbackLogger(on_log, on_flush, NULL);

Each of the two loggers can be set once per process; a second call fails.
:cpp:func:`Logging_SetPattern <Logging::Logging_SetPattern>` changes the format of each entry.
:cpp:func:`Logging_Shutdown <Logging::Logging_Shutdown>` flushes and closes the log, and must be the last
call: any later library call that writes to the log crashes.

Debugger visualizers
^^^^^^^^^^^^^^^^^^^^

The library includes two visualizer files for the Visual Studio debugger:

- ``public.natvis`` — the object handles of the C API, such as
  :cpp:class:`DictionaryObjectHandle` and :cpp:class:`BufferHandle`
- ``vanillapdf.natvis`` — the internal C++ classes, for work on the library
  itself

Built with Visual Studio, the library embeds both files in its program database
(``.pdb``). Only the vcpkg port installs the program database; the ZIP, TGZ and
NuGet packages do not include it. There are no pretty printers for GDB or LLDB,
which show the handles as plain pointers.

Other diagnostics
^^^^^^^^^^^^^^^^^

- `Error handling`_ — the message of the last failed call
- `Leak detection`_ — handles that were never released
- :ref:`Sanitizers <development-options>` — AddressSanitizer,
  UndefinedBehaviorSanitizer and ThreadSanitizer builds of the library

API reference
-------------

Task-based programs are on the :doc:`examples` page. For the complete API
reference, see :doc:`api/documents`, :doc:`api/files`, :doc:`api/contents`,
and :doc:`api/utils`.
