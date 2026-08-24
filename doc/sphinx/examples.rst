Examples
========

API Usage Examples
------------------

- `merge.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/merge.cpp>`_ — File merge
- `extract.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/extract.cpp>`_ — Image extraction
- `filter.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/filter.cpp>`_ — Compression filters
- `sign.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/sign.cpp>`_ — Digital signature
- `sign_custom.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/sign_custom.cpp>`_ — Custom digital signature using interface callbacks
- `verify.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/verify.cpp>`_ — Signature verification
- `validate.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/validate.cpp>`_ — File structure validation
- `decrypt.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/decrypt.cpp>`_ — File decryption
- `write_custom.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/write_custom.cpp>`_ — Custom file writer callbacks

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

.. literalinclude:: ../../src/vanillapdf.tools/validate.cpp
   :language: cpp
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

- `main.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/main.cpp>`_ -- Main file for command-line application vanillapdf.tools
- `tools.h <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/tools.h>`_ -- Tools project header file including macro definitions
- `handle_guard.h <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/handle_guard.h>`_ -- RAII wrapper releasing C API handles at scope exit
