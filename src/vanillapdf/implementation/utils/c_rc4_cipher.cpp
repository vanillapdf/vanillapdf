#include "precompiled.h"

#include "vanillapdf/utils/c_rc4_cipher.h"
#include "implementation/c_helper.h"

#include "utils/rc4_cipher.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_CreateFromKey(BufferHandle* key, RC4CipherHandle** result) {
    Buffer* key_buffer = reinterpret_cast<Buffer*>(key);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(key_buffer);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto cipher = make_deferred<RC4Cipher>(*key_buffer);
        auto ptr = cipher.AddRefGet();
        *result = reinterpret_cast<RC4CipherHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_Process(RC4CipherHandle* handle, BufferHandle* data, BufferHandle** result) {
    RC4Cipher* cipher = reinterpret_cast<RC4Cipher*>(handle);
    Buffer* data_buffer = reinterpret_cast<Buffer*>(data);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(cipher);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data_buffer);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto processed = cipher->Process(*data_buffer);
        auto ptr = processed.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_ToUnknown(RC4CipherHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<RC4Cipher, IUnknown, RC4CipherHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_FromUnknown(IUnknownHandle* handle, RC4CipherHandle** result) {
    return SafeObjectConvert<IUnknown, RC4Cipher, IUnknownHandle, RC4CipherHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_Release(RC4CipherHandle* handle) {
    return ObjectRelease<RC4Cipher, RC4CipherHandle>(handle);
}
