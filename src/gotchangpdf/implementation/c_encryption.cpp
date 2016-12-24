#include "precompiled.h"
#include "encryption.h"
#include "pkcs12_key.h"

#include "gotchangpdf/c_encryption.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

class CustomEncryptionKey : public IEncryptionKey
{
public:
	CustomEncryptionKey(
		InitializeFunction initialize,
		CleanupFunction cleanup,
		DecryptFunction decrypt,
		ContainsFunction contains
		) : m_init(initialize),
		m_cleanup(cleanup),
		m_decrypt(decrypt),
		m_contains(contains)
	{
		// These are only assertions, because those parameters shall be handled
		// at the function call level.
		// RETURN_ERROR_PARAM_VALUE_IF_NULL(initialize);
		// If any of the parameters is nullptr, the function will crash
		// on its invocation.

		assert(initialize != nullptr && "Invalid initialization pointer");
		assert(cleanup != nullptr && "Invalid cleanup pointer");
		assert(decrypt != nullptr && "Invalid decrypt pointer");
		assert(contains != nullptr && "Invalid contains pointer");

		m_init();
	}

	virtual BufferPtr Decrypt(const Buffer& data) const override
	{
		auto input_ptr = reinterpret_cast<const BufferHandleTag*>(&data);
		BufferHandle output_ptr = nullptr;

		error_type rv = m_decrypt(input_ptr, &output_ptr);
		if (GOTCHANG_PDF_ERROR_SUCCES != rv) {
			std::stringstream ss;
			ss << "Custom key decrypt operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}

		if (output_ptr == nullptr) {
			std::stringstream ss;
			ss << "Custom key decrypt operation succeeded, but did not fill the decrypted data pointer";
			throw UserCancelledException(ss.str());
		}

		Buffer *result = reinterpret_cast<Buffer*>(output_ptr);
		return result;
	}

	virtual bool ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const override
	{
		auto input_issuer = reinterpret_cast<const BufferHandleTag*>(&issuer);
		auto input_serial = reinterpret_cast<const BufferHandleTag*>(&serial);
		boolean_type result = false;

		error_type rv = m_contains(input_issuer, input_serial, &result);
		if (GOTCHANG_PDF_ERROR_SUCCES != rv) {
			std::stringstream ss;
			ss << "Custom key equals operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}

		if (GOTCHANG_PDF_TRUE == result)
			return true;
		else
			return false;
	}

	~CustomEncryptionKey()
	{
		m_cleanup();
	}

private:
	InitializeFunction m_init;
	CleanupFunction m_cleanup;
	DecryptFunction m_decrypt;
	ContainsFunction m_contains;
};

GOTCHANG_PDF_API error_type CALLING_CONVENTION EncryptionKey_CreateFromPkcs12File(string_type path, string_type password, PEncryptionKeyHandle result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(path);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		std::string path_string(path);
		Buffer password_buffer;
		if (nullptr != password) {
			password_buffer = Buffer(password);
		}

		Deferred<PKCS12Key> key = Deferred<PKCS12Key>(path_string, password_buffer);

		auto ptr = static_cast<IEncryptionKey*>(key.AddRefGet());
		*result = reinterpret_cast<EncryptionKeyHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION EncryptionKey_CreateFromPkcs12Buffer(BufferHandle data, string_type password, PEncryptionKeyHandle result)
{
	Buffer* buffer_ptr = reinterpret_cast<Buffer*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer_ptr);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Buffer password_buffer;
		if (nullptr != password) {
			password_buffer = Buffer(password);
		}

		BufferPtr data_buffer(buffer_ptr);
		Deferred<PKCS12Key> key(data_buffer, password_buffer);
		auto ptr = static_cast<IEncryptionKey*>(key.AddRefGet());
		*result = reinterpret_cast<EncryptionKeyHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION EncryptionKey_CreateCustom(
	InitializeFunction initialize,
	CleanupFunction cleanup,
	DecryptFunction decrypt,
	ContainsFunction contains,
	PEncryptionKeyHandle result
	)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(initialize);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(cleanup);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(decrypt);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(contains);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Deferred<CustomEncryptionKey> key(initialize, cleanup, decrypt, contains);
		auto ptr = static_cast<IEncryptionKey*>(key.AddRefGet());
		*result = reinterpret_cast<EncryptionKeyHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION EncryptionKey_Release(EncryptionKeyHandle handle)
{
	return ObjectRelease<IEncryptionKey, EncryptionKeyHandle>(handle);
}