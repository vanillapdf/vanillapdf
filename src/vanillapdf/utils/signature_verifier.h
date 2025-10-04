#ifndef _SIGNATURE_VERIFIER_H
#define _SIGNATURE_VERIFIER_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"
#include "utils/trusted_certificate_store.h"
#include "utils/signature_verification_result.h"

namespace vanillapdf {

/**
* \enum VerificationFlags
* \brief Flags controlling signature verification behavior
*/
enum class VerificationFlags {
    None = 0,
    CheckRevocation = 1 << 0,          ///< Check CRL/OCSP for certificate revocation
    RequireTrustedRoot = 1 << 1,       ///< Require certificate chain to a trusted root
    AllowExpiredCerts = 1 << 2,        ///< Allow expired certificates
    CheckSigningTime = 1 << 3          ///< Validate certificate was valid at signing time
};

// Bitwise operators for VerificationFlags
inline VerificationFlags operator|(VerificationFlags a, VerificationFlags b) {
    return static_cast<VerificationFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline VerificationFlags operator&(VerificationFlags a, VerificationFlags b) {
    return static_cast<VerificationFlags>(static_cast<int>(a) & static_cast<int>(b));
}

inline bool operator!(VerificationFlags a) {
    return static_cast<int>(a) == 0;
}

/**
* \class SignatureVerifier
* \brief Low-level utility for verifying PDF digital signatures
*
* This class provides static methods for signature verification without
* requiring parsed PDF document structures. It operates on raw bytes.
*/
class SignatureVerifier {
public:
    /**
    * \brief Verify a digital signature
    * \param signed_data The raw bytes that were signed (extracted from PDF via ByteRange)
    * \param signature_contents The PKCS#7 DER-encoded signature blob
    * \param trusted_store Certificate store for chain validation (required)
    * \param flags Verification behavior flags
    * \return Detailed verification result
    *
    * This function:
    * 1. Extracts the signing certificate from the PKCS#7 signature
    * 2. Verifies the signature cryptographically against signed_data
    * 3. Validates the certificate chain against trusted_store
    * 4. Returns comprehensive verification result
    *
    * \note The trusted_store parameter is required. Use TrustedCertificateStore
    *       to configure trusted certificates (from files, directories, or system defaults).
    */
    static SignatureVerificationResultPtr Verify(
        const Buffer& signed_data,
        const Buffer& signature_contents,
        TrustedCertificateStorePtr trusted_store,
        VerificationFlags flags
    );

private:
    SignatureVerifier() = delete;
    ~SignatureVerifier() = delete;
};

} // vanillapdf

#endif /* _SIGNATURE_VERIFIER_H */
