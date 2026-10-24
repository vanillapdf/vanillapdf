#ifndef _C_CRYPTO_SETTINGS_H
#define _C_CRYPTO_SETTINGS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_crypto_settings.h
    * \brief This file contains APIs for configuring the cryptographic toolkit
    */

    /**
    * \brief Whether the algorithms only old documents and key files use are enabled
    * \ingroup group_utils
    *
    * Legacy algorithms are RC2 (envelopes of certificate encrypted documents,
    * PKCS#12 key files) and MD2/MD4 (signatures). RC4 and MD5 of the standard
    * security handler are implemented in-house and are always available.
    *
    * This is about whether the algorithms can be executed at all. Whether signature
    * verification accepts weak algorithms is a separate decision, see
    * \ref SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag.
    */
    typedef enum {

        /**
        * \brief Undefined unitialized default value, triggers error when used
        */
        LegacyAlgorithmsPolicy_Undefined = 0,

        /**
        * \brief Never enable the legacy algorithms
        */
        LegacyAlgorithmsPolicy_Disabled,

        /**
        * \brief Enable the legacy algorithms when available, otherwise continue without them
        *
        * This is the default. RC2 envelopes and key files fail with
        * \ref VANILLAPDF_ERROR_NOT_SUPPORTED when the legacy algorithms could not be enabled.
        */
        LegacyAlgorithmsPolicy_Optional,

        /**
        * \brief Fail the initialization when the legacy algorithms cannot be enabled
        *
        * \ref CryptoUtils_Initialize returns \ref VANILLAPDF_ERROR_NOT_SUPPORTED when
        * the library was built without VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER,
        * or when OpenSSL cannot load its legacy provider.
        */
        LegacyAlgorithmsPolicy_Required
    } LegacyAlgorithmsPolicyType;

    /**
    * \class CryptoSettingsHandle
    * \extends IUnknownHandle
    * \ingroup group_utils
    * \brief Configuration of the cryptographic toolkit, applied by \ref CryptoUtils_Initialize
    */

    /**
    * \memberof CryptoSettingsHandle
    * @{
    */

    /**
    * \brief Create new crypto settings with default values
    * \param result Output handle to created settings
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_Create(
        CryptoSettingsHandle** result
    );

    /**
    * \brief Get the legacy algorithms policy
    * \param handle The settings handle
    * \param result Output policy, \ref LegacyAlgorithmsPolicy_Optional by default
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_GetLegacyAlgorithmsPolicy(
        CryptoSettingsHandle* handle,
        LegacyAlgorithmsPolicyType* result
    );

    /**
    * \brief Set the legacy algorithms policy
    * \param handle The settings handle
    * \param value Policy to apply at initialization
    * \return Error code
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_SetLegacyAlgorithmsPolicy(
        CryptoSettingsHandle* handle,
        LegacyAlgorithmsPolicyType value
    );

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_ToUnknown(
        CryptoSettingsHandle* handle,
        IUnknownHandle** result
    );

    /**
    * \brief Convert \ref IUnknownHandle to \ref CryptoSettingsHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_FromUnknown(
        IUnknownHandle* handle,
        CryptoSettingsHandle** result
    );

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_Release(
        CryptoSettingsHandle* handle
    );

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CRYPTO_SETTINGS_H */
