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
    * \brief Get allow untrusted root flag
    * \param handle The settings handle
    * \param result Output flag value
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetAllowUntrustedRootFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Set allow untrusted root flag (self-signed certificates)
    * \param handle The settings handle
    * \param value Flag value to set
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetAllowUntrustedRootFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type value
    );

    /**
    * \brief Get allow expired certificates flag
    * \param handle The settings handle
    * \param result Output flag value
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetAllowExpiredCertsFlag(
        SignatureVerificationSettingsHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Set allow expired certificates flag
    * \param handle The settings handle
    * \param value Flag value to set
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetAllowExpiredCertsFlag(
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
