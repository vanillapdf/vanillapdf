Examples
========

Integration Examples
--------------------

- :doc:`FetchContent Integration <fetchcontent_integration>` — FetchContent integration with cross-platform testing

API Usage Examples
------------------

- `merge.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/merge.c>`_ — File merge
- `extract.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/extract.c>`_ — Image extraction
- `filter.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/filter.c>`_ — Compression filters
- `sign.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/sign.c>`_ — Digital signature
- `sign_custom.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/sign_custom.c>`_ — Custom digital signature using interface callbacks
- `verify.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/verify.c>`_ — Signature verification
- `validate.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/validate.c>`_ — File structure validation
- `decrypt.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/decrypt.c>`_ — File decryption
- `write_custom.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/write_custom.c>`_ — Custom file writer callbacks

Utilities
---------

Print library info
^^^^^^^^^^^^^^^^^^

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print library info]
   :end-before: //! [Print library info]
   :dedent:

Print last error
^^^^^^^^^^^^^^^^

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print last error]
   :end-before: //! [Print last error]
   :dedent:

Print buffer
^^^^^^^^^^^^

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Print buffer]
   :end-before: //! [Print buffer]
   :dedent:

Test logging
^^^^^^^^^^^^

.. literalinclude:: ../../src/vanillapdf.test/utils.c
   :language: c
   :start-after: //! [Test logging]
   :end-before: //! [Test logging]
   :dedent:

Print date
^^^^^^^^^^

.. literalinclude:: ../../src/vanillapdf.test/documents.c
   :language: c
   :start-after: //! [Print date]
   :end-before: //! [Print date]
   :dedent:

Validate file structure
^^^^^^^^^^^^^^^^^^^^^^^

.. literalinclude:: ../../src/vanillapdf.tools/validate.c
   :language: c
   :start-after: //! [Validate file structure]
   :end-before: //! [Validate file structure]
   :dedent:

Enumerate xref
^^^^^^^^^^^^^^

.. literalinclude:: ../../src/vanillapdf.test/files.c
   :language: c
   :start-after: //! [Enumerate xref]
   :end-before: //! [Enumerate xref]
   :dedent:

Additional Resources
--------------------

- `main.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/main.c>`_ — Main file for command-line application vanillapdf.tools
- `tools.h <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/tools.h>`_ — Tools project header file including macro definitions
