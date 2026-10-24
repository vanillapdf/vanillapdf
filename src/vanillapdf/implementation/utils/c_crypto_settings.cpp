#include "precompiled.h"

#include "vanillapdf/utils/c_crypto_settings.h"
#include "implementation/c_helper.h"

#include "utils/crypto_settings.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_Create(
    CryptoSettingsHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto settings = make_deferred<CryptoSettings>();
        auto ptr = settings.AddRefGet();
        *result = reinterpret_cast<CryptoSettingsHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_GetLegacyAlgorithmsPolicy(
    CryptoSettingsHandle* handle,
    LegacyAlgorithmsPolicyType* result) {

    CryptoSettings* settings = reinterpret_cast<CryptoSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        switch (settings->GetLegacyAlgorithmsPolicy()) {
            case CryptoSettings::LegacyAlgorithmsPolicy::Undefined:
                *result = LegacyAlgorithmsPolicy_Undefined; break;
            case CryptoSettings::LegacyAlgorithmsPolicy::Disabled:
                *result = LegacyAlgorithmsPolicy_Disabled; break;
            case CryptoSettings::LegacyAlgorithmsPolicy::Optional:
                *result = LegacyAlgorithmsPolicy_Optional; break;
            case CryptoSettings::LegacyAlgorithmsPolicy::Required:
                *result = LegacyAlgorithmsPolicy_Required; break;
            default:
                return VANILLAPDF_ERROR_GENERAL;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_SetLegacyAlgorithmsPolicy(
    CryptoSettingsHandle* handle,
    LegacyAlgorithmsPolicyType value) {

    CryptoSettings* settings = reinterpret_cast<CryptoSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);

    try {
        switch (value) {
            case LegacyAlgorithmsPolicy_Disabled:
                settings->SetLegacyAlgorithmsPolicy(CryptoSettings::LegacyAlgorithmsPolicy::Disabled); break;
            case LegacyAlgorithmsPolicy_Optional:
                settings->SetLegacyAlgorithmsPolicy(CryptoSettings::LegacyAlgorithmsPolicy::Optional); break;
            case LegacyAlgorithmsPolicy_Required:
                settings->SetLegacyAlgorithmsPolicy(CryptoSettings::LegacyAlgorithmsPolicy::Required); break;
            default:
                return VANILLAPDF_ERROR_PARAMETER_VALUE;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_ToUnknown(
    CryptoSettingsHandle* handle,
    IUnknownHandle** result) {
    return SafeObjectConvert<CryptoSettings, IUnknown, CryptoSettingsHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_FromUnknown(
    IUnknownHandle* handle,
    CryptoSettingsHandle** result) {
    return SafeObjectConvert<IUnknown, CryptoSettings, IUnknownHandle, CryptoSettingsHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION CryptoSettings_Release(
    CryptoSettingsHandle* handle) {
    return ObjectRelease<CryptoSettings, CryptoSettingsHandle>(handle);
}
