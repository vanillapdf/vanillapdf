#include "precompiled.h"

#include "vanillapdf/utils/c_signature_verification_settings.h"
#include "implementation/c_helper.h"

#include "utils/signature_verification_settings.h"

using namespace vanillapdf;

// SignatureVerificationSettings

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_Create(
    SignatureVerificationSettingsHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto settings = make_deferred<SignatureVerificationSettings>();
        auto ptr = settings.AddRefGet();
        *result = reinterpret_cast<SignatureVerificationSettingsHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

// TODO: CRL/OCSP revocation checking (https://github.com/vanillapdf/vanillapdf/issues/157)
/*
VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetCheckRevocationFlag(
    SignatureVerificationSettingsHandle* handle,
    boolean_type* result) {

    SignatureVerificationSettings* settings = reinterpret_cast<SignatureVerificationSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = settings->GetCheckRevocationFlag() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetCheckRevocationFlag(
    SignatureVerificationSettingsHandle* handle,
    boolean_type value) {

    SignatureVerificationSettings* settings = reinterpret_cast<SignatureVerificationSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);

    try {
        settings->SetCheckRevocationFlag(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}
*/

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetSkipCertificateValidation(
    SignatureVerificationSettingsHandle* handle,
    boolean_type* result) {

    SignatureVerificationSettings* settings = reinterpret_cast<SignatureVerificationSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = settings->GetSkipCertificateValidation() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetSkipCertificateValidation(
    SignatureVerificationSettingsHandle* handle,
    boolean_type value) {

    SignatureVerificationSettings* settings = reinterpret_cast<SignatureVerificationSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);

    try {
        settings->SetSkipCertificateValidation(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetCheckSigningTimeFlag(
    SignatureVerificationSettingsHandle* handle,
    boolean_type* result) {

    SignatureVerificationSettings* settings = reinterpret_cast<SignatureVerificationSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = settings->GetCheckSigningTimeFlag() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetCheckSigningTimeFlag(
    SignatureVerificationSettingsHandle* handle,
    boolean_type value) {

    SignatureVerificationSettings* settings = reinterpret_cast<SignatureVerificationSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);

    try {
        settings->SetCheckSigningTimeFlag(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_GetAllowWeakAlgorithmsFlag(
    SignatureVerificationSettingsHandle* handle,
    boolean_type* result) {

    SignatureVerificationSettings* settings = reinterpret_cast<SignatureVerificationSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = settings->GetAllowWeakAlgorithmsFlag() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag(
    SignatureVerificationSettingsHandle* handle,
    boolean_type value) {

    SignatureVerificationSettings* settings = reinterpret_cast<SignatureVerificationSettings*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(settings);

    try {
        settings->SetAllowWeakAlgorithmsFlag(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_ToUnknown(
    SignatureVerificationSettingsHandle* handle,
    IUnknownHandle** result) {
    return SafeObjectConvert<SignatureVerificationSettings, IUnknown, SignatureVerificationSettingsHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_FromUnknown(
    IUnknownHandle* handle,
    SignatureVerificationSettingsHandle** result) {
    return SafeObjectConvert<IUnknown, SignatureVerificationSettings, IUnknownHandle, SignatureVerificationSettingsHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationSettings_Release(
    SignatureVerificationSettingsHandle* handle) {
    return ObjectRelease<SignatureVerificationSettings, SignatureVerificationSettingsHandle>(handle);
}
