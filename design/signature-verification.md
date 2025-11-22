# Signature Verification Design

**Status**: Core implementation complete, cleanup tasks pending
**Branch**: `feature/signature-verification`
**Date**: 2025-11-22

## Overview

PKCS#7 digital signature verification for PDF documents, including certificate chain validation, weak algorithm detection, and trusted root verification.

The implementation is well-architected and functional with comprehensive test coverage (25 tests passing). There are cleanup items to address before final release.

## Architecture

### Component Layers

```
Application Layer          CLI tools, API consumers
        │
C API Layer                c_signature_verifier.h, c_signing_key.h
        │
Verification Layer         SignatureVerifier (static utility)
        │
Certificate Layer          TrustedCertificateStore, SignatureVerificationResult
        │
Cryptographic Layer        OpenSSL (PKCS7, X509, EVP)
```

### Core Components

| Component | Location | Purpose |
|-----------|----------|---------|
| `SignatureVerifier` | `src/vanillapdf/utils/signature_verifier.h` | Static PKCS#7 verification |
| `SignatureVerificationResult` | `src/vanillapdf/utils/signature_verification_result.h` | Detailed verification results |
| `TrustedCertificateStore` | `src/vanillapdf/utils/trusted_certificate_store.h` | Certificate chain validation |
| `SignatureVerificationSettings` | `src/vanillapdf/utils/signature_verification_settings.h` | Configuration flags |
| `DigitalSignatureExtensions` | `src/vanillapdf/semantics/` | Document-level API |

### Verification Pipeline

```cpp
SignatureVerificationResult* Verify(data, signature, trust_store, settings) {
    // Stage 1: PKCS#7 Parsing
    PKCS7* p7 = d2i_PKCS7(signature);

    // Stage 2: Certificate Extraction
    ExtractCertificateChain(p7, result);

    // Stage 3: Cryptographic Verification
    PKCS7_verify(p7, ...);

    // Stage 4: Chain Validation
    VerifyCertificateChain(p7, store, result, settings);

    // Stage 5: Algorithm Strength Check
    IsWeakAlgorithm(p7, result);

    // Stage 6: Result Assembly
    return result;
}
```

## What's Working Well

- Clean layered architecture (low-level `SignatureVerifier` vs high-level `DigitalSignatureExtensions::Verify`)
- Secure defaults (all "allow" flags default to `false`)
- Comprehensive test coverage (25 tests)
- Proper PKCS#7 signature verification
- Certificate chain validation against trusted stores
- Weak algorithm detection (MD5, SHA-1, MD2, MD4, small RSA/DSA keys)
- Signing time extraction and validation
- CLI verify command with all options
- C API with comprehensive null checks
- Proper resource cleanup with SCOPE_GUARD

## Implementation Status

### Completed Features

- Low-level C++ API (`SignatureVerifier`, `SignatureVerificationResult`, `TrustedCertificateStore`)
- C API with comprehensive null checks and certificate chain access
- Document-level API (`DigitalSignatureExtensions::Verify`)
- CLI verify command with all options
- Comprehensive test suite (25+ tests including end-to-end)
- `SigningKey` direct signing methods
- Certificate chain iteration API
- Weak algorithm detection:
  - Digest algorithms (si->digest_alg): MD2, MD4, MD5, SHA-1, MD5-SHA1
  - Signature algorithms (si->digest_enc_alg):
    - MD2/MD4/MD5 based: md2WithRSA, md4WithRSA, md5WithRSA
    - SHA-0 based: shaWithRSA, dsaWithSHA
    - SHA-1 based: sha1WithRSA, dsaWithSHA1, ecdsaWithSHA1
    - Deprecated: ripemd160WithRSA, mdc2WithRSA
  - Key sizes: RSA<2048, DSA<2048, EC<256
- Signing time extraction and validation

### Configuration Flags

| Flag | Default | Purpose |
|------|---------|---------|
| `SkipCertificateValidationFlag` | `false` | Bypass X509 chain validation (allows self-signed/untrusted certs) |
| `CheckSigningTimeFlag` | `false` | Validate cert at signing time |
| `AllowWeakAlgorithmsFlag` | `false` | Allow MD5, SHA-1, small keys |

## Cleanup Tasks

### Completed ✅

#### 1. CheckRevocationFlag Commented Out
**Status**: ✅ Completed - Commented out with TODO link to GitHub issue #157

The `CheckRevocationFlag` was exposed in the settings API but had NO implementation. It has been commented out with a reference to the future CRL/OCSP implementation issue.

#### 2. Orphaned AddCertificateFromFile Method Removed
**Status**: ✅ Completed

Removed orphaned stub implementation from `trusted_certificate_store.cpp`.

#### 3. IsCertificateValidAtSigningTime Field Removed
**Status**: ✅ Completed

Removed unused field, getter, setter from `SignatureVerificationResult`.

#### 4. EC Key Strength Validation Added
**Status**: ✅ Completed

Added EC key size validation (< 256 bits flagged as weak) in `signature_verifier.cpp`.

#### 5. CLI Test Fixed
**Status**: ✅ Completed

Fixed `Tools.verify.Granizo-signed.with-flag` test to use `--skip-certificate-validation` instead of non-existent `--allow-untrusted-root` flag.

## Test Coverage Gaps (Nice to Have)

1. **Combined flags test**: No tests for multiple flags enabled together
2. **Certificate chain iteration**: No explicit test for `GetCertificateChainAt` with multiple certificates
3. **EC weak key test**: Would need test once EC validation is added

## Testing

```bash
# Run all signature verification tests
ctest --preset windows-x64-msvc-17 --build-config Debug \
  -R "TrustedCertificateStore|SignatureVerifier|SignatureVerificationResult" \
  --output-on-failure
```

## Commits in Branch (Latest First)

```
3c722322 refactor: Simplify certificate validation settings API
9bee0c59 chore: Comment out unimplemented CRL/OCSP revocation checking API
b918aaf6 refactor: Extract CryptoUtils class for centralized OpenSSL utilities
1aee5a39 refactor: Move helper functions to SignatureVerifier private static methods
4daefff7 refactor: Improve error handling and time conversion in signature verification
02284d8c feat: Add CLI verify command for signature verification
3860a597 feat: Add CheckSigningTimeFlag for signing time validation
eda3a3c9 feat: Add SkipCertificateValidationFlag for bypassing chain validation
```

## Next Steps

- [x] Comment out CheckRevocationFlag from all APIs (with GitHub issue link)
- [x] Remove orphaned AddCertificateFromFile method
- [x] Remove unused IsCertificateValidAtSigningTime field
- [x] Add EC key strength validation
- [x] Fix CLI test to use correct flag
- [x] Build and test all changes
- [ ] Commit and push fixes

## Future Work

- [CRL/OCSP Revocation Checking](crl-ocsp-revocation.md) - Certificate revocation validation
- RFC 3161 timestamps
- LTV (Long-Term Validation)
- Multiple signature enumeration
- PAdES compliance levels

---

**Document Version**: 1.0
**Last Updated**: November 2025
