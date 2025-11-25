#ifndef _SIGNATURE_VERIFICATION_SETTINGS_H
#define _SIGNATURE_VERIFICATION_SETTINGS_H

#include "utils/utils_fwd.h"

namespace vanillapdf {

/**
* \class SignatureVerificationSettings
* \brief Configuration settings for signature verification behavior
*
* This class provides a flexible way to configure signature verification options.
* Each setting can be individually controlled via getter/setter methods.
*
* Example usage:
* \code
* auto settings = make_deferred<SignatureVerificationSettings>();
* settings->SetSkipCertificateValidation(false);  // Require valid certificate chain (default)
* settings->SetAllowWeakAlgorithmsFlag(false);    // Reject weak algorithms (default)
*
* auto result = SignatureVerifier::Verify(signed_data, signature, trust_store, settings);
* \endcode
*/
class SignatureVerificationSettings : public IUnknown {
public:
    /* TODO: CRL/OCSP revocation checking (https://github.com/vanillapdf/vanillapdf/issues/157)
    **
    * \brief Check if certificate revocation should be verified (CRL/OCSP)
    * \return true if revocation checking is enabled, false otherwise
    *
    bool GetCheckRevocationFlag(void) const noexcept { return m_check_revocation; }

    **
    * \brief Enable or disable certificate revocation checking
    * \param flag true to enable revocation checking, false to disable
    *
    void SetCheckRevocationFlag(bool flag) noexcept { m_check_revocation = flag; }
    */

    /**
    * \brief Check if certificate chain validation should be skipped
    * \return true if certificate validation is skipped, false otherwise
    *
    * When enabled, the signature will be verified cryptographically but
    * the certificate chain will not be validated against the trust store.
    * This bypasses checks for: expired certificates, untrusted roots,
    * incomplete chains, and other X509 validation errors.
    *
    * \note Effect on verification result when enabled:
    * - SignatureVerificationResult::GetStatus() returns SignatureStatus_Valid
    *   (assuming the cryptographic signature is correct)
    * - SignatureVerificationResult::IsSignatureValid() returns true
    * - SignatureVerificationResult::IsCertificateTrusted() returns **false**
    *
    * The IsCertificateTrusted flag remains false because the certificate chain
    * was not actually verified - we cannot claim trust for something we did not check.
    * This provides transparency: a Valid status with IsCertificateTrusted=false
    * indicates that validation was bypassed rather than successfully completed.
    *
    * \warning This is a security bypass intended for testing/debugging only.
    * In production, certificates should be properly added to the trust store.
    */
    bool GetSkipCertificateValidation(void) const noexcept { return m_skip_certificate_validation; }

    /**
    * \brief Enable or disable certificate chain validation
    * \param flag true to skip validation (insecure), false to require valid chain (default)
    *
    * When set to true, certificate chain validation is bypassed entirely.
    * The cryptographic signature is still verified, but no X509 chain validation
    * is performed. This allows signatures with expired, self-signed, or untrusted
    * certificates to pass verification.
    *
    * \note When validation is skipped, IsCertificateTrusted() will return false
    * in the verification result, even though the overall status may be Valid.
    * This accurately reflects that trust was not established through verification.
    *
    * \warning Setting this to true bypasses important security checks including:
    * certificate expiration, trust chain validation, and root CA verification.
    * Only use for testing or when you explicitly trust the signer through
    * out-of-band means.
    */
    void SetSkipCertificateValidation(bool flag) noexcept { m_skip_certificate_validation = flag; }

    /**
    * \brief Check if signing time validation is enabled
    * \return true if signing time should be validated, false otherwise
    */
    bool GetCheckSigningTimeFlag(void) const noexcept { return m_check_signing_time; }

    /**
    * \brief Validate that certificate was valid at signing time
    * \param flag true to check signing time, false to skip
    */
    void SetCheckSigningTimeFlag(bool flag) noexcept { m_check_signing_time = flag; }

    /**
    * \brief Check if weak cryptographic algorithms are allowed
    * \return true if weak algorithms are allowed, false otherwise
    */
    bool GetAllowWeakAlgorithmsFlag(void) const noexcept { return m_allow_weak_algorithms; }

    /**
    * \brief Allow or reject weak cryptographic algorithms (MD5, SHA-1, RSA < 2048 bits)
    * \param flag true to allow weak algorithms, false to reject them
    */
    void SetAllowWeakAlgorithmsFlag(bool flag) noexcept { m_allow_weak_algorithms = flag; }

private:
    /* TODO: CRL/OCSP revocation checking (https://github.com/vanillapdf/vanillapdf/issues/157)
    bool m_check_revocation = false;            ///< Check CRL/OCSP for certificate revocation
    */
    bool m_skip_certificate_validation = false; ///< Skip X509 certificate chain validation
    bool m_check_signing_time = false;          ///< Validate certificate was valid at signing time
    bool m_allow_weak_algorithms = false;       ///< Allow weak signature algorithms and key sizes
};

} // vanillapdf

#endif /* _SIGNATURE_VERIFICATION_SETTINGS_H */
