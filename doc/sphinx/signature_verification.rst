Signature verification guide
============================

This guide explains how to verify digital signatures in PDF documents using the Vanilla.PDF C API.

Overview
--------

PDF digital signatures use CMS (Cryptographic Message Syntax) format to store cryptographic signatures. Verification involves:

1. **Cryptographic verification** - Verify the signature matches the document content
2. **Certificate chain validation** - Validate the signer's certificate against trusted roots
3. **Algorithm strength check** - Detect weak algorithms (MD5, SHA-1, small keys)
4. **Time validation** - Optionally validate certificate at signing time

Supported signing algorithms
----------------------------

Vanilla.PDF supports signing and verification with a wide range of key types,
including modern EdDSA curves rarely supported by other PDF libraries:

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 25 25 50

   * - Key Type
     - Digest
     - Notes
   * - RSA (up to 4096-bit)
     - SHA-256, SHA-384, SHA-512
     - Widely supported; 4096-bit recommended
   * - ECDSA (P-256, P-384, P-521)
     - SHA-256, SHA-384, SHA-512
     - Smaller keys, equivalent security to RSA
   * - Ed25519 (EdDSA)
     - SHA-512 (required by RFC 8419)
     - Fast, small signatures; modern standard. Signing requires OpenSSL 3.2 or newer,
       and requesting any other digest is rejected as an invalid parameter
   * - Ed448 (EdDSA)
     - SHAKE256
     - Verification only. Signing is rejected as not supported: RFC 8419 requires the
       ``id-shake256-len`` digest algorithm, which OpenSSL does not implement
       (`#320 <https://github.com/vanillapdf/vanillapdf/issues/320>`_)
   * - DSA (2048-bit)
     - SHA-256
     - Legacy; prefer ECDSA or EdDSA for new deployments

Signatures are validated end-to-end by the CI interoperability check against
`pdfsig (poppler-utils) <https://manpages.debian.org/testing/poppler-utils/pdfsig.1.en.html>`_
for RSA, ECDSA and DSA, and against `pyHanko <https://github.com/MatthiasValvekens/pyHanko>`_
for RSA, ECDSA and Ed25519.

Verification status codes
--------------------------

The verification result includes a status code indicating the outcome:

.. list-table::
   :header-rows: 1
   :class: wrap
   :widths: 30 70

   * - Status
     - Description
   * - :cpp:enumerator:`SignatureStatus_Valid <SignatureVerificationStatusType::SignatureStatus_Valid>`
     - Signature is cryptographically valid and certificate chain is trusted
   * - :cpp:enumerator:`SignatureStatus_Invalid <SignatureVerificationStatusType::SignatureStatus_Invalid>`
     - Signature verification failed (content modified or wrong key)
   * - :cpp:enumerator:`SignatureStatus_CertificateExpired <SignatureVerificationStatusType::SignatureStatus_CertificateExpired>`
     - Signing certificate has expired
   * - :cpp:enumerator:`SignatureStatus_CertificateNotYetValid <SignatureVerificationStatusType::SignatureStatus_CertificateNotYetValid>`
     - Certificate not yet valid (future start date)
   * - :cpp:enumerator:`SignatureStatus_CertificateUntrusted <SignatureVerificationStatusType::SignatureStatus_CertificateUntrusted>`
     - Certificate chain does not lead to a trusted root
   * - :cpp:enumerator:`SignatureStatus_WeakAlgorithm <SignatureVerificationStatusType::SignatureStatus_WeakAlgorithm>`
     - Weak cryptographic algorithm detected
   * - :cpp:enumerator:`SignatureStatus_DocumentModified <SignatureVerificationStatusType::SignatureStatus_DocumentModified>`
     - Document modified after signing

Weak algorithm detection
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

Creating a trust store
----------------------

Create a trusted certificate store and load certificates:

.. literalinclude:: ../../src/vanillapdf.tools/verify.cpp
   :language: cpp
   :start-after: //! [Create trust store]
   :end-before: //! [Create trust store]
   :dedent:

Configuring verification settings
----------------------------------

Configure verification behavior with custom settings:

.. literalinclude:: ../../src/vanillapdf.tools/verify.cpp
   :language: cpp
   :start-after: //! [Configure verification settings]
   :end-before: //! [Configure verification settings]
   :dedent:

Available settings:

- :cpp:func:`SetSkipCertificateValidation <SignatureVerificationSettingsHandle::SignatureVerificationSettings_SetSkipCertificateValidation>` - Skip X509 chain validation (testing only)
- :cpp:func:`SetAllowWeakAlgorithmsFlag <SignatureVerificationSettingsHandle::SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag>` - Allow MD5, SHA-1, small keys
- :cpp:func:`SetCheckSigningTimeFlag <SignatureVerificationSettingsHandle::SignatureVerificationSettings_SetCheckSigningTimeFlag>` - Validate cert at signing time instead of current time

Verifying a signature
---------------------

Call the verification API and inspect the results:

.. literalinclude:: ../../src/vanillapdf.tools/verify.cpp
   :language: cpp
   :start-after: //! [Verify signature]
   :end-before: //! [Verify signature]
   :dedent:

Security recommendations
------------------------

1. **Always use system trust store** — Let the OS manage trusted root certificates (:cpp:func:`TrustedCertificateStore_LoadSystemDefaults <TrustedCertificateStoreHandle::TrustedCertificateStore_LoadSystemDefaults>`)
2. **Avoid** :cpp:func:`SetSkipCertificateValidation <SignatureVerificationSettingsHandle::SignatureVerificationSettings_SetSkipCertificateValidation>` — Only use for testing self-signed certificates
3. **Avoid** :cpp:func:`SetAllowWeakAlgorithmsFlag <SignatureVerificationSettingsHandle::SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag>` — Only enable when absolutely necessary for legacy documents
4. **Use** :cpp:func:`SetCheckSigningTimeFlag <SignatureVerificationSettingsHandle::SignatureVerificationSettings_SetCheckSigningTimeFlag>` — For old documents where certificates may have expired
5. **Keep certificates updated** — Ensure your system's certificate store is current

For a complete working example, see `verify.cpp
<https://github.com/vanillapdf/vanillapdf/blob/main/src/vanillapdf.tools/verify.cpp>`_.
