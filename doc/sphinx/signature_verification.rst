Signature Verification Guide
============================

This guide explains how to verify digital signatures in PDF documents using the Vanilla.PDF C API.

For a complete working example, see ``verify.c``.

Overview
--------

PDF digital signatures use PKCS#7 (CMS) format to store cryptographic signatures. Verification involves:

1. **Cryptographic verification** - Verify the signature matches the document content
2. **Certificate chain validation** - Validate the signer's certificate against trusted roots
3. **Algorithm strength check** - Detect weak algorithms (MD5, SHA-1, small keys)
4. **Time validation** - Optionally validate certificate at signing time

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

Create a trusted certificate store and load certificates for chain validation.

Configuring Verification Settings
----------------------------------

Configure verification behavior with custom settings.

Available settings:

- ``SetSkipCertificateValidation`` - Skip X509 chain validation (testing only)
- ``SetAllowWeakAlgorithmsFlag`` - Allow MD5, SHA-1, small keys
- ``SetCheckSigningTimeFlag`` - Validate cert at signing time instead of current time

Verifying a Signature
---------------------

Call the verification API and inspect the results.

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

1. **Always use system trust store** - Let the OS manage trusted root certificates
2. **Avoid ``--skip-certificate-validation``** - Only use for testing self-signed certificates
3. **Avoid ``--allow-weak-algorithms``** - Only enable when absolutely necessary for legacy documents
4. **Use ``--check-signing-time``** - For old documents where certificates may have expired
5. **Keep certificates updated** - Ensure your system's certificate store is current
