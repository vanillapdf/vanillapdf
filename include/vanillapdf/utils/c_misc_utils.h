#ifndef _C_MISC_UTILS_H
#define _C_MISC_UTILS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
    * \file c_misc_utils.h
    * \brief Initialization and cleanup helpers for miscellaneous utilities.
    */

    /**
    * \class MiscUtils
    * \ingroup group_utils
    * \brief Static utility class for OpenSSL initialization and cleanup
    * \deprecated Use \ref CryptoUtils instead
    */

    /**
    * \memberof MiscUtils
    * @{ */

    /**
    * \brief Initialize OpenSSL with the default \ref CryptoSettingsHandle
    * \deprecated Use CryptoUtils_Initialize instead
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION MiscUtils_InitializeOpenSSL();

    /**
    * \brief Release what the initialization loaded
    * \deprecated Use CryptoUtils_Cleanup instead
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION MiscUtils_CleanupOpenSSL();

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_MISC_UTILS_H */
