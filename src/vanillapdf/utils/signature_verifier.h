#ifndef _SIGNATURE_VERIFIER_H
#define _SIGNATURE_VERIFIER_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"
#include "utils/trusted_certificate_store.h"
#include "utils/signature_verification_result.h"
#include "utils/signature_verification_options.h"

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
    * \param trusted_store Optional certificate store for chain validation (uses system defaults if null)
    * \param flags Verification behavior flags
    * \return Detailed verification result
    *
    * This function:
    * 1. Extracts the signing certificate from the PKCS#7 signature
    * 2. Verifies the signature cryptographically against signed_data
    * 3. Validates the certificate chain against trusted_store
    * 4. Checks certificate validity periods
    * 5. Returns comprehensive verification result
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
