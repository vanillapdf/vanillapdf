#ifndef _SIGNATURE_VERIFIER_H
#define _SIGNATURE_VERIFIER_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"
#include "utils/trusted_certificate_store.h"
#include "utils/signature_verification_result.h"
#include "utils/signature_verification_settings.h"

// Forward declarations for OpenSSL types (only when OpenSSL is available)
#if defined(VANILLAPDF_HAVE_OPENSSL)
struct pkcs7_st;
struct x509_store_st;
typedef struct pkcs7_st PKCS7;
typedef struct x509_store_st X509_STORE;
#endif

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

#if defined(VANILLAPDF_HAVE_OPENSSL)
    /**
     * \brief Check if signature uses weak cryptographic algorithms
     * \return true if weak algorithm detected, false otherwise
     * \throws GeneralException for logic errors (parsing failures, structure errors)
     */
    static bool IsWeakAlgorithm(PKCS7* p7, SignatureVerificationResultPtr& result);

    /**
     * \brief Extract certificate chain from PKCS#7 signature
     * \param p7 PKCS#7 signature structure
     * \param result Verification result to populate with certificate chain
     */
    static void ExtractCertificateChain(PKCS7* p7, SignatureVerificationResultPtr& result);

    /**
     * \brief Extract signing time from PKCS#7 authenticated attributes
     * \param p7 PKCS#7 signature structure
     * \param signing_time Output parameter for extracted time
     * \return true if signing time was found and extracted, false if not present (valid per spec)
     * \throws GeneralException for logic errors (parsing failures, memory allocation, etc.)
     */
    static bool ExtractSigningTime(PKCS7* p7, time_t* signing_time);

    /**
     * \brief Verify certificate chain against trusted store
     * \param p7 PKCS#7 signature structure
     * \param store Trusted certificate store
     * \param result Verification result to populate
     * \param settings Verification settings
     * \return true if chain is valid, false otherwise
     */
    static bool VerifyCertificateChain(PKCS7* p7, X509_STORE* store,
                                       SignatureVerificationResultPtr& result,
                                       SignatureVerificationSettingsPtr settings);
#endif
};

} // vanillapdf

#endif /* _SIGNATURE_VERIFIER_H */
