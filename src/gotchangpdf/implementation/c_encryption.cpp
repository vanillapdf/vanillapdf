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
		EqualsFunction equals
		) : m_init(initialize),
		m_cleanup(cleanup),
		m_decrypt(decrypt),
		m_equals(equals)
	{
		if (nullptr != m_init) m_init();
	}

	virtual BufferPtr Decrypt(const Buffer& data) const override
	{
		BufferPtr result;
		auto input_ptr = reinterpret_cast<const BufferHandleTag*>(&data);
		auto output_ptr = reinterpret_cast<BufferHandle>(result.get());
		if (nullptr == m_decrypt) {
			throw GeneralException("Decryption function not initialized");
		}

		error_type rv = m_decrypt(input_ptr, &output_ptr);
		if (GOTCHANG_PDF_ERROR_SUCCES != rv) {
			std::stringstream ss;
			ss << "Custom key decrypt operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}

		return result;
	}

	virtual bool ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const override
	{
		auto input_issuer = reinterpret_cast<const BufferHandleTag*>(&issuer);
		auto input_serial = reinterpret_cast<const BufferHandleTag*>(&serial);
		boolean_type result = false;
		if (nullptr == m_equals) {
			throw GeneralException("Equals function not initialized");
		}

		error_type rv = m_equals(input_issuer, input_serial, &result);
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
		if (nullptr != m_cleanup) m_cleanup();
	}

private:
	InitializeFunction m_init;
	CleanupFunction m_cleanup;
	DecryptFunction m_decrypt;
	EqualsFunction m_equals;
};

GOTCHANG_PDF_API error_type CALLING_CONVENTION Pkcs12EncryptionKey_CreateFromFile(string_type path, string_type password, PEncryptionKeyHandle result)
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION Pkcs12EncryptionKey_CreateFromBuffer(BufferHandle data, PEncryptionKeyHandle result)
{
	Buffer* buffer = reinterpret_cast<Buffer*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Deferred<PKCS12Key> key(*buffer);
		auto ptr = static_cast<IEncryptionKey*>(key.AddRefGet());
		*result = reinterpret_cast<EncryptionKeyHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION CustomEncryptionKey_Create(InitializeFunction initialize, CleanupFunction cleanup, DecryptFunction decrypt, EqualsFunction equals, PEncryptionKeyHandle result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Deferred<CustomEncryptionKey> key(initialize, cleanup, decrypt, equals);
		auto ptr = static_cast<IEncryptionKey*>(key.AddRefGet());
		*result = reinterpret_cast<EncryptionKeyHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION EncryptionKey_Release(EncryptionKeyHandle handle)
{
	IEncryptionKey* key = reinterpret_cast<IEncryptionKey*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(key);
	key->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}