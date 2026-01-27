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
- `decrypt.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/decrypt.c>`_ — File decryption
- `write_custom.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/write_custom.c>`_ — Custom file writer callbacks

Utilities
---------

Utility snippets from the integration test suite
(`utils.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.test/utils.c>`_):

- Print library info
- Print last error
- Set license file
- Print buffer
- Test logging

Additional snippets:

- `Print date <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.test/documents.c>`_ — from ``documents.c``
- `Enumerate xref <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.test/files.c>`_ — from ``files.c``

Additional Resources
--------------------

- `main.c <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/main.c>`_ — Main file for command-line application vanillapdf.tools
- `tools.h <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/tools.h>`_ — Tools project header file including macro definitions
