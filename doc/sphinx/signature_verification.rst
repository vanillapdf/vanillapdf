Signature Verification Guide
============================

This guide explains how to verify digital signatures in PDF documents using the Vanilla.PDF C API.

For a complete working example, see ``verify.c``.

Overview
--------

PDF digital signatures use CMS (Cryptographic Message Syntax) format to store cryptographic signatures. Verification involves:

1. **Cryptographic verification** - Verify the signature matches the document content
2. **Certificate chain validation** - Validate the signer's certificate against trusted roots
3. **Algorithm strength check** - Detect weak algorithms (MD5, SHA-1, small keys)
4. **Time validation** - Optionally validate certificate at signing time

Supported Signing Algorithms
----------------------------

Vanilla.PDF supports signing and verification with a wide range of key types,
including modern EdDSA curves rarely supported by other PDF libraries:

.. list-table::
   :header-rows: 1
   :widths: 25 25 50

   * - Key Type
     - Digest
     - Notes
   * - RSA (up to 4096-bit)
     - SHA-256, SHA-384, SHA-512, SHA3-256, SHA3-384, SHA3-512
     - Widely supported; 4096-bit recommended
   * - ECDSA (P-256, P-384, P-521)
     - SHA-256, SHA-384, SHA-512
     - Smaller keys, equivalent security to RSA
   * - Ed25519 (EdDSA)
     - SHA-512 (per RFC 8419)
     - Fast, small signatures; modern standard
   * - Ed448 (EdDSA)
     - SHAKE256 (per RFC 8419)
     - Higher security margin than Ed25519
   * - DSA (2048-bit)
     - SHA-256
     - Legacy; prefer ECDSA or EdDSA for new deployments

All algorithms are validated end-to-end via the CI interoperability check
against `pdfsig (poppler-utils) <https://manpages.debian.org/testing/poppler-utils/pdfsig.1.en.html>`_.

Verification Status Codes
--------------------------

The verification result includes a status code indicating the outcome:

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Status
     - Description
   * - ``SignatureStatus_Valid``
     - Signature is cryptographically valid and certificate chain is trusted
   * - ``SignatureStatus_Invalid``
     - Signature verification failed (content modified or wrong key)
   * - ``SignatureStatus_CertificateExpired``
     - Signing certificate has expired
   * - ``SignatureStatus_CertificateNotYetValid``
     - Certificate not yet valid (future start date)
   * - ``SignatureStatus_CertificateUntrusted``
     - Certificate chain does not lead to a trusted root
   * - ``SignatureStatus_WeakAlgorithm``
     - Weak cryptographic algorithm detected
   * - ``SignatureStatus_DocumentModified``
     - Document modified after signing

Weak Algorithm Detection
-------------------------

The verifier detects the following weak algorithms:

**Weak Digest Algorithms:**

- MD2, MD4, MD5, SHA-1, MD5-SHA1

**Weak Signature Algorithms:**

- MD2/MD4/MD5 with RSA
- SHA-0 with RSA/DSA
- SHA-1 with RSA/DSA/ECDSA
- RIPEMD-160 with RSA, MDC2 with RSA

**Weak Key Sizes:**

- RSA keys < 2048 bits
- DSA keys < 2048 bits
- EC keys < 256 bits

Creating a Trust Store
----------------------

Create a trusted certificate store and load certificates:

.. literalinclude:: ../../src/vanillapdf.tools/verify.c
   :language: c
   :start-after: //! [Create trust store]
   :end-before: //! [Create trust store]
   :dedent:

Configuring Verification Settings
----------------------------------

Configure verification behavior with custom settings:

.. literalinclude:: ../../src/vanillapdf.tools/verify.c
   :language: c
   :start-after: //! [Configure verification settings]
   :end-before: //! [Configure verification settings]
   :dedent:

Available settings:

- ``SetSkipCertificateValidation`` - Skip X509 chain validation (testing only)
- ``SetAllowWeakAlgorithmsFlag`` - Allow MD5, SHA-1, small keys
- ``SetCheckSigningTimeFlag`` - Validate cert at signing time instead of current time

Verifying a Signature
---------------------

Call the verification API and inspect the results:

.. literalinclude:: ../../src/vanillapdf.tools/verify.c
   :language: c
   :start-after: //! [Verify signature]
   :end-before: //! [Verify signature]
   :dedent:

Command Line Usage
------------------

The vanillapdf.tools CLI provides signature verification:

.. code-block:: bash

   # Basic verification (uses system trust store)
   vanillapdf.tools verify -f document.pdf

   # Skip certificate validation (for self-signed certificates)
   vanillapdf.tools verify -f document.pdf --skip-certificate-validation

   # Allow weak algorithms (for legacy documents)
   vanillapdf.tools verify -f document.pdf --allow-weak-algorithms

   # Check certificate at signing time
   vanillapdf.tools verify -f document.pdf --check-signing-time

   # Use custom certificate directory
   vanillapdf.tools verify -f document.pdf -c /path/to/trusted/certs

   # Combine options
   vanillapdf.tools verify -f document.pdf \
       --allow-weak-algorithms \
       --check-signing-time

Security Recommendations
------------------------

1. **Always use system trust store** — Let the OS manage trusted root certificates
2. **Avoid** ``--skip-certificate-validation`` — Only use for testing self-signed certificates
3. **Avoid** ``--allow-weak-algorithms`` — Only enable when absolutely necessary for legacy documents
4. **Use** ``--check-signing-time`` — For old documents where certificates may have expired
5. **Keep certificates updated** — Ensure your system's certificate store is current
