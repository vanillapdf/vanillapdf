CLI Tools
=========

Vanilla.PDF ships with ``vanillapdf-tools``, a command-line utility for
common PDF operations.

Building the CLI tool
---------------------

``vanillapdf-tools`` is built automatically as part of the default build:

.. code-block:: bash

   cmake --preset linux-x64-gcc        # or any other preset
   cmake --build --preset linux-x64-gcc

The binary is placed in the build output directory.

Available commands
------------------

Run ``vanillapdf-tools --help`` to see the full list of commands.

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Command
     - Description
   * - ``sign``
     - Digitally sign a PDF document using a PKCS#12 key
   * - ``sign_custom``
     - Sign using custom signature callbacks
   * - ``verify``
     - Verify digital signatures in a PDF
   * - ``merge``
     - Combine multiple PDF documents into one
   * - ``extract``
     - Extract images from a PDF
   * - ``extract_object``
     - Extract a specific object by number
   * - ``filter``
     - Encode or decode content streams with compression filters
   * - ``encrypt``
     - Encrypt a PDF with password protection
   * - ``decrypt``
     - Remove encryption from a PDF
   * - ``write_custom``
     - Write a PDF with custom observer callbacks

sign
----

Digitally sign a PDF document using a PKCS#12 key file:

.. code-block:: bash

   vanillapdf-tools sign -s input.pdf -d signed.pdf -k private_key.p12 -p password

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-s``
     - Source PDF file
   * - ``-d``
     - Destination (signed) PDF file
   * - ``-k``
     - PKCS#12 key file containing certificate and private key
   * - ``-p``
     - Password for the key file

verify
------

Verify digital signatures in a PDF document:

.. code-block:: bash

   vanillapdf-tools verify -f signed.pdf

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-f``
     - Signed PDF file to verify
   * - ``-c``
     - Directory containing trusted certificates (optional, uses system defaults)
   * - ``--skip-certificate-validation``
     - Skip X.509 chain validation (testing only)
   * - ``--allow-weak-algorithms``
     - Allow weak algorithms such as MD5 and SHA-1
   * - ``--check-signing-time``
     - Validate certificate at signing time instead of current time

For details on verification status codes, see :doc:`signature_verification`.

merge
-----

Combine multiple PDF documents into a single file:

.. code-block:: bash

   vanillapdf-tools merge -s base.pdf -d merged.pdf -f extra1.pdf extra2.pdf

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-s``
     - Base PDF file
   * - ``-d``
     - Destination (merged) PDF file
   * - ``-f``
     - One or more PDF files to merge into the base

extract
-------

Extract images from a PDF document:

.. code-block:: bash

   vanillapdf-tools extract -s document.pdf

Extracted images are saved as separate files named
``<object_number>.<generation_number>``.

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-s``
     - Source PDF file

extract_object
--------------

Extract a specific object from a PDF by its object and generation numbers:

.. code-block:: bash

   vanillapdf-tools extract_object -s document.pdf -o 5 -g 0

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-s``
     - Source PDF file
   * - ``-o``
     - Object number
   * - ``-g``
     - Generation number

filter
------

Encode or decode PDF content streams using various compression filters:

.. code-block:: bash

   vanillapdf-tools filter -f flate encode -s input.bin -d compressed.bin
   vanillapdf-tools filter -f flate decode -s compressed.bin -d output.bin

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-f``
     - Filter type and operation: ``<type> <encode|decode>``
   * - ``-s``
     - Source file
   * - ``-d``
     - Destination file

Supported filter types: ``flate``, ``dct``, ``jpx``, ``ascii85``,
``ascii_hex``.

encrypt
-------

Encrypt a PDF document with password protection:

.. code-block:: bash

   vanillapdf-tools encrypt -s input.pdf -d encrypted.pdf -op owner123 -up user123

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-s``
     - Source PDF file
   * - ``-d``
     - Destination (encrypted) PDF file
   * - ``-op``
     - Owner password
   * - ``-up``
     - User password
   * - ``-ka``
     - Encryption algorithm: ``None``, ``RC4``, or ``AES`` (default: ``RC4``)
   * - ``-kl``
     - Key length in bits (default: 40)

decrypt
-------

Remove encryption from a PDF document:

.. code-block:: bash

   vanillapdf-tools decrypt -s encrypted.pdf -d decrypted.pdf -p password

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-s``
     - Encrypted PDF file
   * - ``-d``
     - Destination (decrypted) PDF file
   * - ``-p``
     - User or owner password
   * - ``-k``
     - Path to PKCS#12 file for certificate-based decryption (optional)

write_custom
------------

Write a PDF to a new file with custom observer callbacks:

.. code-block:: bash

   vanillapdf-tools write_custom -s input.pdf -d output.pdf

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Option
     - Description
   * - ``-s``
     - Source PDF file
   * - ``-d``
     - Destination PDF file

This command demonstrates the ``FileWriterObserverHandle`` callback interface.
See `write_custom.c
<https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/write_custom.c>`_
for the implementation.
