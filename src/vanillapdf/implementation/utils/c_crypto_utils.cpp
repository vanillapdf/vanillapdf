#include "precompiled.h"

#include "vanillapdf/utils/c_crypto_utils.h"
#include "implementation/c_helper.h"

#include "utils/crypto_settings.h"
#include "utils/crypto_utils.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION CryptoUtils_Initialize(CryptoSettingsHandle* settings) {
    CryptoSettings* settings_object = reinterpret_cast<CryptoSettings*>(settings);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings_object);

    try {
        CryptoUtils::Initialize(*settings_object);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION CryptoUtils_GetLegacyAlgorithmsAvailable(boolean_type* result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = CryptoUtils::IsLegacyAlgorithmsAvailable() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION CryptoUtils_Cleanup() {
    try {
        CryptoUtils::Cleanup();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}
