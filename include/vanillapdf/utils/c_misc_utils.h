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
    */

    /**
    * \memberof MiscUtils
    * @{ */

    /**
    * \brief Set the directory where OpenSSL looks for provider modules.
    *
    * On OpenSSL 3.x, providers such as the legacy provider (legacy.dll)
    * are loaded at runtime from a compiled-in directory. If that directory
    * does not match the actual installation (e.g. when using vcpkg on Windows),
    * provider loading will fail. Call this function before
    * MiscUtils_InitializeOpenSSL() to override the search path.
    *
    * This is equivalent to setting the OPENSSL_MODULES environment variable.
    *
    * \param path Directory containing OpenSSL provider modules.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MiscUtils_SetOpenSSLModulesPath(string_type path);

    /** \brief Initialize OpenSSL providers and algorithms. */
    VANILLAPDF_API error_type CALLING_CONVENTION MiscUtils_InitializeOpenSSL();

    /** \brief Cleanup OpenSSL providers and algorithms. */
    VANILLAPDF_API error_type CALLING_CONVENTION MiscUtils_CleanupOpenSSL();

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_MISC_UTILS_H */
