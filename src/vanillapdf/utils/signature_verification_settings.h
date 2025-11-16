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
* settings->SetCheckRevocationFlag(true);
* settings->SetRequireTrustedRootFlag(true);
* settings->SetAllowExpiredCertsFlag(false);
*
* auto result = SignatureVerifier::Verify(signed_data, signature, trust_store, settings);
* \endcode
*/
class SignatureVerificationSettings : public IUnknown {
public:
    /**
    * \brief Check if certificate revocation should be verified (CRL/OCSP)
    * \return true if revocation checking is enabled, false otherwise
    */
    bool GetCheckRevocationFlag(void) const noexcept { return m_check_revocation; }

    /**
    * \brief Enable or disable certificate revocation checking
    * \param flag true to enable revocation checking, false to disable
    */
    void SetCheckRevocationFlag(bool flag) noexcept { m_check_revocation = flag; }

    /**
    * \brief Check if certificate chain must terminate in a trusted root
    * \return true if trusted root is required, false otherwise
    */
    bool GetRequireTrustedRootFlag(void) const noexcept { return m_require_trusted_root; }

    /**
    * \brief Require certificate chain to terminate in a trusted root
    * \param flag true to require trusted root, false to allow self-signed
    */
    void SetRequireTrustedRootFlag(bool flag) noexcept { m_require_trusted_root = flag; }

    /**
    * \brief Check if expired certificates are allowed
    * \return true if expired certificates are allowed, false otherwise
    */
    bool GetAllowExpiredCertsFlag(void) const noexcept { return m_allow_expired_certs; }

    /**
    * \brief Allow or reject expired certificates
    * \param flag true to allow expired certificates, false to reject them
    */
    void SetAllowExpiredCertsFlag(bool flag) noexcept { m_allow_expired_certs = flag; }

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

private:
    bool m_check_revocation = false;       ///< Check CRL/OCSP for certificate revocation
    bool m_require_trusted_root = false;   ///< Require certificate chain to trusted root
    bool m_allow_expired_certs = false;    ///< Allow expired certificates
    bool m_check_signing_time = false;     ///< Validate certificate was valid at signing time
};

} // vanillapdf

#endif /* _SIGNATURE_VERIFICATION_SETTINGS_H */
