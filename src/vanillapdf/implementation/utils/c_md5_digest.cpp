#include "precompiled.h"

#include "vanillapdf/utils/c_md5_digest.h"
#include "implementation/c_helper.h"

#include "utils/md5_digest.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_Create(MD5DigestHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto digest = make_deferred<MD5Digest>();
        auto ptr = digest.AddRefGet();
        *result = reinterpret_cast<MD5DigestHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_Update(MD5DigestHandle* handle, BufferHandle* data) {
    MD5Digest* digest = reinterpret_cast<MD5Digest*>(handle);
    Buffer* data_buffer = reinterpret_cast<Buffer*>(data);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(digest);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data_buffer);

    try {
        digest->Update(*data_buffer);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_Final(MD5DigestHandle* handle, BufferHandle** result) {
    MD5Digest* digest = reinterpret_cast<MD5Digest*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(digest);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto digest_value = digest->Final();
        auto ptr = digest_value.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_ToUnknown(MD5DigestHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<MD5Digest, IUnknown, MD5DigestHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_FromUnknown(IUnknownHandle* handle, MD5DigestHandle** result) {
    return SafeObjectConvert<IUnknown, MD5Digest, IUnknownHandle, MD5DigestHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_Release(MD5DigestHandle* handle) {
    return ObjectRelease<MD5Digest, MD5DigestHandle>(handle);
}
