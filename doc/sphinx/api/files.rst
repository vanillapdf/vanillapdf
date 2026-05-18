Files
=====

.. doxygengroup:: group_files
   :content-only:
   :members:

.. note::

   **FileStructureValidator initialization requirement**

   ``FileStructureValidator_Validate`` requires the file to have been
   successfully initialized via ``File_Initialize`` before validation can
   run.  In practice this is less restrictive than it sounds: when the
   primary xref parsing fails, the parser falls back to a full-file object
   scan that recovers most malformed PDFs, and only then is the file
   marked as initialized.  Files that still cannot be initialized after
   the fallback — typically those using unsupported features such as
   unknown encryption — will propagate an initialization error instead;
   in those cases the error from ``File_Initialize`` is itself the
   diagnostic signal.

Objects
-------

.. doxygengroup:: group_objects
   :content-only:
   :members:

Cross-Reference Tables and Streams
-----------------------------------

.. doxygengroup:: group_xref
   :content-only:
   :members:

Name Constants
--------------

.. doxygengroup:: group_name_constants
   :content-only:
   :members:
