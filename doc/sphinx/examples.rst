Examples
========

Each example is a complete C program in the
`examples/ <https://github.com/vanillapdf/vanillapdf/tree/main/examples>`_
directory of the repository, with a ``CMakeLists.txt`` that builds it against
an installed library, as shown in the :doc:`quickstart`. The library's test
suite builds and runs every one of them, so the code on this page always
compiles and works with the current release.

All examples follow the same pattern: the ``CHECK`` macro stores the result of
each call and jumps to the cleanup on the first failure, where the error
message is printed and every handle obtained so far is released.

Inspect a document
------------------

Open a document, count its pages and read entries of its document information
dictionary. Entries a document does not have are reported as
``VANILLAPDF_ERROR_OBJECT_MISSING``.

.. literalinclude:: ../../examples/inspect/main.c
   :language: c

Merge documents
---------------

Append the pages of several documents to the first one and save the result
under a new name.

.. literalinclude:: ../../examples/merge/main.c
   :language: c

Sign a document
---------------

Signing a document with a key from a PKCS#12 file is shown in the
:ref:`quickstart <quickstart-signing>`; the program is in
`examples/signing/ <https://github.com/vanillapdf/vanillapdf/tree/main/examples/signing>`_.

Verify signatures
-----------------

Find the signed signature fields of a document and verify each signature
against a store of trusted certificates. The program exits with ``2`` when a
signature does not verify or the document has none, so scripts can tell a bad
signature from an error.
See :doc:`signature_verification` for the verification settings and the
meaning of each status.

.. literalinclude:: ../../examples/verification/main.c
   :language: c

Encrypt a document
------------------

Protect a document with AES-256, a user password that opens it and an owner
password that also lifts the access restrictions.

.. literalinclude:: ../../examples/encryption/main.c
   :language: c

Validate the file structure
---------------------------

Check the cross-reference tables, trailers and streams of a file and list the
problems found. The program exits with ``2`` when the file has structural
errors.

.. literalinclude:: ../../examples/validation/main.c
   :language: c

More code in the repository
---------------------------

The repository also contains ``vanillapdf.tools``, an internal test tool that
is not part of the distributed packages. Its sources cover topics the examples
above do not:

- `sign_custom.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/sign_custom.cpp>`_ — signing through callbacks, for keys the library cannot load itself, such as keys in a hardware token
- `write_custom.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/write_custom.cpp>`_ — callbacks invoked at each stage of writing a file, with access to the output stream
- `extract.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/extract.cpp>`_ — extracting images from page resources
- `filter.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/filter.cpp>`_ — encoding and decoding data with the stream filters
- `decrypt.cpp <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/decrypt.cpp>`_ — removing encryption with a password or a certificate

They are written in C++ and release handles with the RAII wrapper in
`handle_guard.h <https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/handle_guard.h>`_.
