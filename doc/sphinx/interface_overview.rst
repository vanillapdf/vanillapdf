Interface Overview
==================

The library exposes native C-style interface to allow interoperability with most languages.
All handles are basically opaque pointers to internal structures,
which are meant to be modified using the only the API and not the structures themselves.

Types
-----

All of the basic interface types are either opaque pointers to internal structures or simply standard value types such as integers or strings.

Since boolean type is not supported in the C-style interface, we have a custom definition of ``boolean_type`` with only two possible states - ``VANILLAPDF_RV_TRUE`` and ``VANILLAPDF_RV_FALSE``.

For more detailed information please visit the :doc:`Types API reference <api/types>`.

Memory leaks
-------------

All interface objects are reference counted.
Whenever an object is received with an output parameter, it has to be released when no longer needed.

.. code-block:: c

   FileHandle file = NULL;
   error_code file_opened = File_Open(argv[1], &file);
   if (file_opened != VANILLAPDF_ERROR_SUCCESS) {
       return file_opened;
   }

   ...

   error_code file_released = File_Release(file);
   if (file_released != VANILLAPDF_ERROR_SUCCESS) {
       return file_released;
   }

After the file has been opened successfully, the caller is responsible for calling appropriate release function.
Calling the release multiple times is an error and causes **undefined behavior**.
We recommend resetting the handles to their initial state of **NULL**, to prevent any errors.

For example this macro may help in maintaining consistent state of the local variables:

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

With this helper macro, the cleanup becomes easier as long as it is properly initialized with **NULL**.

.. code-block:: c

   FileHandle file = NULL;
   DocumentHandle document = NULL;

   error_code file_opened = File_Open(argv[1], &file);
   if (file_opened != VANILLAPDF_ERROR_SUCCESS) {
       goto err;
   }

   error_code document_opened = Document_Open(argv[1], &document);
   if (document_opened != VANILLAPDF_ERROR_SUCCESS) {
       goto err;
   }

   err:
       SAFE_RELEASE(File_Release, file);
       SAFE_RELEASE(Document_Release, document);

It is also safe to call this release macro several times.
This code style may seem repulsive for some developers, they are free to use any kind of cleanup, as long as it serves the purpose.

.. note::

   The release function should not fail as long as the parameter is valid. The error code is just to keep the interface consistent.

Error handling
--------------

Every function throughout the interface returns ``error_type``. On success, the return value should be ``VANILLAPDF_ERROR_SUCCESS``.
In every other case it means that an error has occurred and it's error message is stored. The next section explains how to gather this message.

Extended error information
^^^^^^^^^^^^^^^^^^^^^^^^^^

There is a code sample how to obtain the last error code with the message printed to console.

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print last error]
   :end-before: //! [Print last error]
   :dedent:

For more detailed information please visit the ``Errors`` API.

Debugging
---------

Sometimes the error code and message is not enough to solve the problem. Library also supports logging, that can be configured via ``Logging``.
Logging have multiple log levels, which control the verbosity of the output.

Following snippet enables the logging feature with the highest possible logging severity.

.. code-block:: c

   #include "vanillapdf/c_logging.h"

   error_type set_logging() {
       RETURN_ERROR_IF_NOT_SUCCESS(Logging_Enable());
       RETURN_ERROR_IF_NOT_SUCCESS(Logging_SetSeverity(LoggingSeverity_Debug));

       return VANILLAPDF_TEST_ERROR_SUCCESS;
   }

For more useful utilities please visit the :doc:`Utilities API reference <api/utils>`.
