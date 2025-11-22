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
- Weak algorithm detection (MD5, SHA-1, MD2, MD4, RSA<2048, DSA<2048)
- Signing time extraction and validation

### Configuration Flags

| Flag | Default | Purpose |
|------|---------|---------|
| `SkipCertificateValidationFlag` | `false` | Bypass X509 chain validation |
| `CheckSigningTimeFlag` | `false` | Validate cert at signing time |
| `AllowWeakAlgorithmsFlag` | `false` | Allow MD5, SHA-1, small keys |
| `AllowUntrustedRootFlag` | `false` | Accept self-signed certificates |

## Cleanup Tasks

### Critical Issues (Must Fix)

#### 1. CheckRevocationFlag is Non-Functional

**Location**:
- `src/vanillapdf/utils/signature_verification_settings.h`
- `src/vanillapdf/utils/signature_verifier.cpp:388` (TODO comment)

**Problem**: The `CheckRevocationFlag` is exposed in the settings API but has NO implementation. Users might enable it expecting CRL/OCSP certificate revocation checking to work, but it does nothing.

**Impact**: Misleading API - security feature appears available but isn't functional.

**Recommendation**: Remove the flag from all APIs until CRL/OCSP checking is properly implemented:
- Remove from `SignatureVerificationSettings`
- Remove from C API (`c_signature_verification_settings.h`)
- Remove from CLI tool (`verify.c`)

#### 2. Orphaned AddCertificateFromFile Method

**Location**: `src/vanillapdf/utils/trusted_certificate_store.cpp:173-175`

**Problem**: A stub implementation exists in the .cpp file but the method is NOT declared in the header file (`trusted_certificate_store.h`). This is dead/unreachable code.

**Impact**: Code maintenance issue; confusing for future developers.

**Recommendation**: Remove the orphaned method from the .cpp file.

### Should Fix (Quality)

#### 3. IsCertificateValidAtSigningTime Field Unused

**Location**:
- `src/vanillapdf/utils/signature_verification_result.h:77` (declaration)
- `src/vanillapdf/utils/signature_verification_result.cpp:35` (getter)

**Problem**: The `m_certificate_valid_at_signing` field is declared and has a getter, but it's never set during verification. It always returns `false`.

**Impact**: Useless API surface; potentially confusing for users.

**Recommendation**: Either:
- (a) Implement the logic to properly set this field during verification, OR
- (b) Remove the field and associated methods until needed

#### 4. EC Key Strength Validation Missing

**Location**: `src/vanillapdf/utils/signature_verifier.cpp:70-84` (IsWeakAlgorithm)

**Problem**: The weak algorithm detection checks RSA and DSA key sizes but does NOT validate EC (Elliptic Curve) keys. EC keys smaller than 256 bits should be flagged as weak.

**Impact**: Incomplete weak algorithm detection for modern EC-based signatures.

**Recommendation**: Add EC key size validation:
```cpp
// Check for weak EC key sizes (< 256 bits)
if (key_type == EVP_PKEY_EC && key_bits < 256) {
    spdlog::info("Weak EC key size detected: {} bits", key_bits);
    result->SetMessage(fmt::format("Weak EC key size: {} bits (minimum 256)", key_bits));
    return true;
}
```

## Files to Modify

| File | Change |
|------|--------|
| `src/vanillapdf/utils/signature_verification_settings.h` | Remove CheckRevocationFlag |
| `src/vanillapdf/implementation/utils/c_signature_verification_settings.cpp` | Remove CheckRevocationFlag C API |
| `include/vanillapdf/utils/c_signature_verification_settings.h` | Remove CheckRevocationFlag C API |
| `src/vanillapdf.tools/verify.c` | Remove --check-revocation option |
| `src/vanillapdf/utils/trusted_certificate_store.cpp` | Remove orphaned AddCertificateFromFile |
| `src/vanillapdf/utils/signature_verification_result.h` | Remove IsCertificateValidAtSigningTime |
| `src/vanillapdf/utils/signature_verification_result.cpp` | Remove IsCertificateValidAtSigningTime |
| `include/vanillapdf/utils/c_signature_verifier.h` | Remove IsCertificateValidAtSigningTime C API |
| `src/vanillapdf/implementation/utils/c_signature_verifier.cpp` | Remove IsCertificateValidAtSigningTime C API |
| `src/vanillapdf/utils/signature_verifier.cpp` | Add EC key validation |

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
eda3a3c9 feat: Add AllowUntrustedRootFlag for self-signed certificate handling
```

## Next Steps

- [ ] Remove CheckRevocationFlag from all APIs
- [ ] Remove orphaned AddCertificateFromFile method
- [ ] Remove unused IsCertificateValidAtSigningTime field
- [ ] Add EC key strength validation
- [ ] Build and test all changes
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
