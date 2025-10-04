#include "precompiled.h"

#include "vanillapdf/utils/c_signature_verification.h"
#include "implementation/c_helper.h"

#include "utils/trusted_certificate_store.h"
#include "utils/signature_verifier.h"
#include "utils/signature_verification_options.h"
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
    const char* pem_data) {

    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(pem_data);

    try {
        store->AddCertificateFromPEM(std::string(pem_data));
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_AddCertificateFromDER(
    TrustedCertificateStoreHandle* handle,
    const BufferHandle* der_data) {

    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(handle);
    Buffer* buffer = reinterpret_cast<Buffer*>(const_cast<BufferHandle*>(der_data));
    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

    try {
        store->AddCertificateFromDER(*buffer);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_AddCertificateFromFile(
    TrustedCertificateStoreHandle* handle,
    const char* file_path) {

    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file_path);

    try {
        store->AddCertificateFromFile(std::string(file_path));
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_LoadFromDirectory(
    TrustedCertificateStoreHandle* handle,
    const char* directory_path) {

    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(directory_path);

    try {
        store->LoadFromDirectory(std::string(directory_path));
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

VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_Release(
    TrustedCertificateStoreHandle* handle) {
    return ObjectRelease<TrustedCertificateStore, TrustedCertificateStoreHandle>(handle);
}

// SignatureVerificationOptions

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationOptions_Create(
    SignatureVerificationOptionsHandle** result) {

    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto options = make_deferred<SignatureVerificationOptions>();
        auto ptr = options.AddRefGet();
        *result = reinterpret_cast<SignatureVerificationOptionsHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationOptions_SetTrustedCertificates(
    SignatureVerificationOptionsHandle* handle,
    TrustedCertificateStoreHandle* store) {

    SignatureVerificationOptions* options = reinterpret_cast<SignatureVerificationOptions*>(handle);
    TrustedCertificateStore* cert_store = reinterpret_cast<TrustedCertificateStore*>(store);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(options);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(cert_store);

    try {
        TrustedCertificateStorePtr store_ptr(cert_store);
        options->SetTrustedCertificates(store_ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationOptions_SetFlags(
    SignatureVerificationOptionsHandle* handle,
    int32_type flags) {

    SignatureVerificationOptions* options = reinterpret_cast<SignatureVerificationOptions*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(options);

    try {
        options->SetFlags(static_cast<VerificationFlags>(flags));
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationOptions_Release(
    SignatureVerificationOptionsHandle* handle) {
    return ObjectRelease<SignatureVerificationOptions, SignatureVerificationOptionsHandle>(handle);
}

// SignatureVerification

VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerification_Verify(
    const BufferHandle* signed_data,
    const BufferHandle* signature_contents,
    TrustedCertificateStoreHandle* trusted_store,
    int32_type flags,
    SignatureVerificationResultHandle** result) {

    Buffer* data_buf = reinterpret_cast<Buffer*>(const_cast<BufferHandle*>(signed_data));
    Buffer* sig_buf = reinterpret_cast<Buffer*>(const_cast<BufferHandle*>(signature_contents));
    TrustedCertificateStore* store = reinterpret_cast<TrustedCertificateStore*>(trusted_store);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(data_buf);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(sig_buf);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        TrustedCertificateStorePtr store_ptr;
        if (store) {
            store_ptr = TrustedCertificateStorePtr(store);
        }

        auto verification_result = SignatureVerifier::Verify(
            *data_buf,
            *sig_buf,
            store_ptr,
            static_cast<VerificationFlags>(flags)
        );

        auto ptr = verification_result.AddRefGet();
        *result = reinterpret_cast<SignatureVerificationResultHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

// SignatureVerificationResult

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_GetStatus(
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

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_GetMessage(
    SignatureVerificationResultHandle* handle,
    BufferHandle** result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        std::string message = vr->GetMessage();
        auto buffer = make_deferred_container<Buffer>(message.begin(), message.end());
        auto ptr = buffer.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_IsSignatureValid(
    SignatureVerificationResultHandle* handle,
    bool_type* result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = vr->IsSignatureValid() ? VANILLAPDF_TRUE : VANILLAPDF_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_IsDocumentIntact(
    SignatureVerificationResultHandle* handle,
    bool_type* result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = vr->IsDocumentIntact() ? VANILLAPDF_TRUE : VANILLAPDF_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_IsCertificateTrusted(
    SignatureVerificationResultHandle* handle,
    bool_type* result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = vr->IsCertificateTrusted() ? VANILLAPDF_TRUE : VANILLAPDF_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_GetSignerCertificate(
    SignatureVerificationResultHandle* handle,
    BufferHandle** result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto cert = vr->GetSignerCertificate();
        if (!cert) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }
        auto ptr = cert.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_GetCertificateChainCount(
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

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_GetCertificateChainAt(
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

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_GetSignerCommonName(
    SignatureVerificationResultHandle* handle,
    BufferHandle** result) {

    SignatureVerificationResult* vr = reinterpret_cast<SignatureVerificationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(vr);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        std::string common_name = vr->GetSignerCommonName();
        auto buffer = make_deferred_container<Buffer>(common_name.begin(), common_name.end());
        auto ptr = buffer.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION VerificationResult_Release(
    SignatureVerificationResultHandle* handle) {
    return ObjectRelease<SignatureVerificationResult, SignatureVerificationResultHandle>(handle);
}
