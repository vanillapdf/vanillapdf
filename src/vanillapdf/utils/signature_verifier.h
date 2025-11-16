#ifndef _SIGNATURE_VERIFIER_H
#define _SIGNATURE_VERIFIER_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"
#include "utils/trusted_certificate_store.h"
#include "utils/signature_verification_result.h"
#include "utils/signature_verification_settings.h"

namespace vanillapdf {

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
    * \param settings Verification behavior settings (optional, uses defaults if empty)
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
    * \note If settings is empty, default settings are used (all flags disabled).
    */
    static SignatureVerificationResultPtr Verify(
        const Buffer& signed_data,
        const Buffer& signature_contents,
        TrustedCertificateStorePtr trusted_store,
        SignatureVerificationSettingsPtr settings
    );

private:
    SignatureVerifier() = delete;
    ~SignatureVerifier() = delete;
};

} // vanillapdf

#endif /* _SIGNATURE_VERIFIER_H */
