#include "precompiled.h"
#include "utils/misc_utils.h"

#include "vanillapdf/utils/c_misc_utils.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION MiscUtils_InitializeOpenSSL() {
    try {
        MiscUtils::InitializeOpenSSL();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MiscUtils_CleanupOpenSSL() {
    try {
        MiscUtils::CleanupOpenSSL();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}
