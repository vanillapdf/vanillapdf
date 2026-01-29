#include "precompiled.h"

#include "vanillapdf/utils/c_signature_verifier.h"
#include "implementation/c_helper.h"

#include "utils/trusted_certificate_store.h"
#include "utils/signature_verifier.h"
#include "utils/signature_verification_result.h"

using namespace vanillapdf;

// TrustedCertificateStore

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_Create(
    TrustedCertificateStoreHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto store = make_deferred<TrustedCertificateStore>();
        auto ptr = store.AddRefGet();
        *result = reinterpret_cast<TrustedCertificateStoreHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_AddCertificateFromPEM(
    TrustedCertificateStoreHandle* handle,
    const BufferHandle* pem_data) {

    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(handle);
    const Buffer* buffer = reinterpret_cast<const Buffer*>(pem_data);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

    try {
        store->AddCertificateFromPEM(*buffer);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_AddCertificateFromDER(
    TrustedCertificateStoreHandle* handle,
    const BufferHandle* der_data) {

    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(handle);
    const Buffer* buffer = reinterpret_cast<const Buffer*>(der_data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

    try {
        store->AddCertificateFromDER(*buffer);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_LoadFromDirectory(
    TrustedCertificateStoreHandle* handle,
    string_type directory_path) {

    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(directory_path);

    try {
        store->LoadFromDirectory(directory_path);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_LoadSystemDefaults(
    TrustedCertificateStoreHandle* handle) {

    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);

    try {
        store->LoadSystemDefaults();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_ToUnknown(
    TrustedCertificateStoreHandle* handle,
    IUnknownHandle** result) {
    return SafeObjectConvert<TrustedCertificateStore, IUnknown, TrustedCertificateStoreHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_FromUnknown(
    IUnknownHandle* handle,
    TrustedCertificateStoreHandle** result) {
    return SafeObjectConvert<IUnknown, TrustedCertificateStore, IUnknownHandle, TrustedCertificateStoreHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_Release(
    TrustedCertificateStoreHandle* handle) {
    return ObjectRelease<TrustedCertificateStore, TrustedCertificateStoreHandle>(handle);
}

// SignatureVerifier

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerifier_Verify(
    const BufferHandle* signed_data,
    const BufferHandle* signature_contents,
    TrustedCertificateStoreHandle* trusted_store,
    SignatureVerificationSettingsHandle* settings,
    SignatureVerificationResultHandle** result) {

    const Buffer* data_buf = reinterpret_cast<const Buffer*>(signed_data);
    const Buffer* sig_buf = reinterpret_cast<const Buffer*>(signature_contents);
    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(trusted_store);
    SignatureVerificationSettings* verification_settings = reinterpret_cast<SignatureVerificationSettings*>(settings);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(data_buf);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(sig_buf);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
    // Note: settings can be NULL for default settings

    try {

        auto verification_result = SignatureVerifier::Verify(
            *data_buf,
            *sig_buf,
            store,
            verification_settings
        );

        auto ptr = verification_result.AddRefGet();
        *result = reinterpret_cast<SignatureVerificationResultHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

// SignatureVerificationResult

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetStatus(
    SignatureVerificationResultHandle* handle,
    SignatureVerificationStatusType* result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = static_cast<SignatureVerificationStatusType>(vr->GetStatus());
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetMessage(
    SignatureVerificationResultHandle* handle,
    BufferHandle** result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto buffer = vr->GetMessage();
        if (buffer->empty()) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = buffer.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_IsSignatureValid(
    SignatureVerificationResultHandle* handle,
    boolean_type* result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = vr->IsSignatureValid() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_IsDocumentIntact(
    SignatureVerificationResultHandle* handle,
    boolean_type* result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = vr->IsDocumentIntact() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_IsCertificateTrusted(
    SignatureVerificationResultHandle* handle,
    boolean_type* result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = vr->IsCertificateTrusted() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetSignerCertificate(
    SignatureVerificationResultHandle* handle,
    BufferHandle** result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto cert = vr->GetSignerCertificate();
        if (cert->empty()) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = cert.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetCertificateChainCount(
    SignatureVerificationResultHandle* handle,
    size_type* result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = vr->GetCertificateChainCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetCertificateChainAt(
    SignatureVerificationResultHandle* handle,
    size_type index,
    BufferHandle** result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto cert = vr->GetCertificateChainAt(index);
        auto ptr = cert.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetSignerCommonName(
    SignatureVerificationResultHandle* handle,
    BufferHandle** result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto buffer = vr->GetSignerCommonName();
        if (buffer->empty()) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = buffer.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_ToUnknown(
    SignatureVerificationResultHandle* handle,
    IUnknownHandle** result) {
    return SafeObjectConvert<SignatureVerificationResult, IUnknown, SignatureVerificationResultHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_FromUnknown(
    IUnknownHandle* handle,
    SignatureVerificationResultHandle** result) {
    return SafeObjectConvert<IUnknown, SignatureVerificationResult, IUnknownHandle, SignatureVerificationResultHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_Release(
    SignatureVerificationResultHandle* handle) {
    return ObjectRelease<SignatureVerificationResult, SignatureVerificationResultHandle>(handle);
}
