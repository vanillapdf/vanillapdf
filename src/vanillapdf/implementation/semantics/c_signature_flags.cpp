#include "precompiled.h"

#include "semantics/objects/signature_flags.h"

#include "vanillapdf/semantics/c_signature_flags.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_GetSignaturesExist(SignatureFlagsHandle* handle, boolean_type* result) {
    SignatureFlags* flags = reinterpret_cast<SignatureFlags*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(flags);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = flags->GetSignaturesExist() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_SetSignaturesExist(SignatureFlagsHandle* handle, boolean_type value) {
    SignatureFlags* flags = reinterpret_cast<SignatureFlags*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(flags);

    try {
        flags->SetSignaturesExist(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_GetAppendOnly(SignatureFlagsHandle* handle, boolean_type* result) {
    SignatureFlags* flags = reinterpret_cast<SignatureFlags*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(flags);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = flags->GetAppendOnly() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_SetAppendOnly(SignatureFlagsHandle* handle, boolean_type value) {
    SignatureFlags* flags = reinterpret_cast<SignatureFlags*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(flags);

    try {
        flags->SetAppendOnly(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_ToUnknown(SignatureFlagsHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<SignatureFlags, IUnknown, SignatureFlagsHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_FromUnknown(IUnknownHandle* handle, SignatureFlagsHandle** result) {
    return SafeObjectConvert<IUnknown, SignatureFlags, IUnknownHandle, SignatureFlagsHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_Release(SignatureFlagsHandle* handle) {
    return ObjectRelease<SignatureFlags, SignatureFlagsHandle>(handle);
}
