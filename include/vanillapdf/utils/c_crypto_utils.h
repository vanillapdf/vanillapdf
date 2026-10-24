#ifndef _C_CRYPTO_UTILS_H
#define _C_CRYPTO_UTILS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_crypto_utils.h
    * \brief Initialization and cleanup of the cryptographic toolkit
    */

    /**
    * \class CryptoUtils
    * \ingroup group_utils
    * \brief Static utility class for initializing the cryptographic toolkit
    *
    * Initialization is optional: the first cryptographic operation initializes
    * the toolkit with the default settings. Initialize explicitly, before any
    * other call, to apply different settings.
    */

    /**
    * \memberof CryptoUtils
    * @{ */

    /**
    * \brief Initialize the cryptographic toolkit with the given settings
    * \param settings Settings to apply, see \ref CryptoSettingsHandle
    * \return Error code
    *
    * Once the toolkit is initialized, by an earlier call or by the first cryptographic
    * operation, this call succeeds without changing anything, whichever settings it
    * passes. Use \ref CryptoUtils_GetLegacyAlgorithmsAvailable to check the outcome.
    * To apply different settings call \ref CryptoUtils_Cleanup first; do not do that
    * while other threads perform cryptographic operations.
    *
    * Fails with \ref VANILLAPDF_ERROR_NOT_SUPPORTED when the settings require
    * legacy algorithms that cannot be enabled.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoUtils_Initialize(CryptoSettingsHandle* settings);

    /**
    * \brief Check whether the legacy algorithms (RC2, MD2, MD4) can be used
    * \param result Output flag
    * \return Error code
    *
    * Initializes the toolkit with the default settings first, unless already initialized.
    * Legacy algorithms enabled by the process configuration (OPENSSL_CONF) count as well.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoUtils_GetLegacyAlgorithmsAvailable(boolean_type* result);

    /**
    * \brief Release what the initialization loaded
    * \return Error code
    *
    * Optional: the cryptographic toolkit releases everything at process exit. Call it
    * to apply different settings, or before unloading the library. The toolkit can be initialized again
    * afterwards, explicitly or by the next cryptographic operation.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION CryptoUtils_Cleanup();

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CRYPTO_UTILS_H */
