#ifndef _C_SIGNATURE_VERIFICATION_SETTINGS_H
#define _C_SIGNATURE_VERIFICATION_SETTINGS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_signature_verification_settings.h
    * \brief This file contains APIs for configuring signature verification behavior
    */

    /**
    * \class SignatureVerificationSettingsHandle
    * \extends IUnknownHandle
    * \ingroup group_utils
    * \brief Configuration settings for signature verification
    */

    /**
    * \memberof SignatureVerificationSettingsHandle
    * @{
    */

    /**
    * \brief Create new signature verification settings with default values
    * \param result Output handle to created settings
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_Create(
        SignatureVerificationSettingsHandle** result
    );

    /* TODO: CRL/OCSP revocation checking (https://github.com/vanillapdf/vanillapdf/issues/157)

    **
    * \brief Get certificate revocation checking flag
    * \param handle The settings handle
    * \param result Output flag value
    * \return Error code
    *
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetCheckRevocationFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type* result
    );

    **
    * \brief Set certificate revocation checking flag
    * \param handle The settings handle
    * \param value Flag value to set
    * \return Error code
    *
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetCheckRevocationFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type value
    );
    */

    /**
    * \brief Get skip certificate validation flag
    * \param handle The settings handle
    * \param result Output flag value
    * \return Error code
    *
    * When enabled, signature verification will skip X509 certificate chain
    * validation. The cryptographic signature is still verified, but the
    * certificate chain is not validated against the trust store.
    *
    * \note Effect on verification result when enabled:
    * - SignatureVerificationResult_GetStatus() returns SignatureStatus_Valid
    *   (assuming the cryptographic signature is correct)
    * - SignatureVerificationResult_IsSignatureValid() returns VANILLAPDF_RV_TRUE
    * - SignatureVerificationResult_IsCertificateTrusted() returns VANILLAPDF_RV_FALSE
    *
    * The IsCertificateTrusted flag remains false because the certificate chain
    * was not actually verified - we cannot claim trust for something we did not check.
    * This provides transparency: a Valid status with IsCertificateTrusted=false
    * indicates that validation was bypassed rather than successfully completed.
    *
    * \warning This is a security bypass intended for testing/debugging only.
    * In production, certificates should be properly added to the trust store.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetSkipCertificateValidation(
        SignatureVerificationSettingsHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Set skip certificate validation flag
    * \param handle The settings handle
    * \param value Flag value to set (true to skip validation, false for full validation)
    * \return Error code
    *
    * When set to true, certificate chain validation is bypassed entirely.
    * The cryptographic signature is still verified, but no X509 chain validation
    * is performed. This allows signatures with expired, self-signed, or untrusted
    * certificates to pass verification.
    *
    * \note When validation is skipped, SignatureVerificationResult_IsCertificateTrusted()
    * will return VANILLAPDF_RV_FALSE, even though the overall status may be Valid.
    * This accurately reflects that trust was not established through verification.
    *
    * \warning Setting this to true bypasses important security checks including:
    * certificate expiration, trust chain validation, and root CA verification.
    * Only use for testing or when you explicitly trust the signer through
    * out-of-band means.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetSkipCertificateValidation(
        SignatureVerificationSettingsHandle* handle,
        boolean_type value
    );

    /**
    * \brief Get check signing time flag
    * \param handle The settings handle
    * \param result Output flag value
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetCheckSigningTimeFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Set check signing time flag
    * \param handle The settings handle
    * \param value Flag value to set
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetCheckSigningTimeFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type value
    );

    /**
    * \brief Get allow weak algorithms flag
    * \param handle The settings handle
    * \param result Output flag value
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetAllowWeakAlgorithmsFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Set allow weak algorithms flag (MD5, SHA-1, RSA < 2048 bits)
    * \param handle The settings handle
    * \param value Flag value to set
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type value
    );

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_ToUnknown(
        SignatureVerificationSettingsHandle* handle,
        IUnknownHandle** result
    );

    /**
    * \brief Convert \ref IUnknownHandle to \ref SignatureVerificationSettingsHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_FromUnknown(
        IUnknownHandle* handle,
        SignatureVerificationSettingsHandle** result
    );

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_Release(
        SignatureVerificationSettingsHandle* handle
    );

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_SIGNATURE_VERIFICATION_SETTINGS_H */
