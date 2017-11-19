#include "precompiled.h"

#include "utils/encryption_key_interface.h"
#include "utils/signing_key_interface.h"
#include "utils/pkcs12_key.h"

#include "vanillapdf/utils/c_pkcs12_key.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_CreateFromFile(string_type path, string_type password, PKCS12KeyHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(path);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		std::string path_string(path);
		Buffer password_buffer;
		if (nullptr != password) {
			password_buffer = Buffer(password);
		}

		Deferred<PKCS12Key> key = make_deferred<PKCS12Key>(path_string, password_buffer);
		auto ptr = key.AddRefGet();
		*result = reinterpret_cast<PKCS12KeyHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_CreateFromBuffer(BufferHandle* data, string_type password, PKCS12KeyHandle** result) {
	Buffer* buffer_ptr = reinterpret_cast<Buffer*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer_ptr);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		Buffer password_buffer;
		if (nullptr != password) {
			password_buffer = Buffer(password);
		}

		BufferPtr data_buffer(buffer_ptr);
		Deferred<PKCS12Key> key = make_deferred<PKCS12Key>(data_buffer, password_buffer);
		auto ptr = key.AddRefGet();
		*result = reinterpret_cast<PKCS12KeyHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_ToEncryptionKey(PKCS12KeyHandle* handle, EncryptionKeyHandle** result) {
	return SafeObjectConvert<PKCS12Key, IEncryptionKey, PKCS12KeyHandle, EncryptionKeyHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_ToSigningKey(PKCS12KeyHandle* handle, SigningKeyHandle** result) {
	return SafeObjectConvert<PKCS12Key, ISigningKey, PKCS12KeyHandle, SigningKeyHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_Release(PKCS12KeyHandle* handle) {
	return ObjectRelease<PKCS12Key, PKCS12KeyHandle>(handle);
}