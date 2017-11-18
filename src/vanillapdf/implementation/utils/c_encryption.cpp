#include "precompiled.h"
#include "utils/encryption_key_interface.h"
#include "utils/pkcs12_key.h"

#include "vanillapdf/utils/c_encryption.h"
#include "implementation/c_helper.h"

#include <sstream>

using namespace vanillapdf;
using namespace vanillapdf::syntax;

class CustomEncryptionKey : public IEncryptionKey
{
public:
	CustomEncryptionKey(
		EncryptionKeyInitializeFunction initialize,
		EncryptionKeyCleanupFunction cleanup,
		EncryptionKeyDecryptFunction decrypt,
		EncryptionKeyContainsFunction contains
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

	virtual BufferPtr Decrypt(const Buffer& data) override
	{
		auto input_ptr = reinterpret_cast<const BufferHandleTag*>(&data);
		BufferHandle output_ptr = nullptr;

		error_type rv = m_decrypt(input_ptr, &output_ptr);
		if (VANILLAPDF_ERROR_SUCCESS != rv) {
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
		if (VANILLAPDF_ERROR_SUCCESS != rv) {
			std::stringstream ss;
			ss << "Custom key equals operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}

		if (VANILLAPDF_RV_TRUE == result)
			return true;
		else
			return false;
	}

	~CustomEncryptionKey()
	{
		m_cleanup();
	}

private:
	EncryptionKeyInitializeFunction m_init;
	EncryptionKeyCleanupFunction m_cleanup;
	EncryptionKeyDecryptFunction m_decrypt;
	EncryptionKeyContainsFunction m_contains;
};

VANILLAPDF_API error_type CALLING_CONVENTION EncryptionKey_CreateCustom(
	EncryptionKeyInitializeFunction initialize,
	EncryptionKeyCleanupFunction cleanup,
	EncryptionKeyDecryptFunction decrypt,
	EncryptionKeyContainsFunction contains,
	EncryptionKeyHandle* result
	)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(initialize);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(cleanup);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(decrypt);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(contains);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		Deferred<CustomEncryptionKey> key = make_deferred<CustomEncryptionKey>(initialize, cleanup, decrypt, contains);
		auto ptr = static_cast<IEncryptionKey*>(key.AddRefGet());
		*result = reinterpret_cast<EncryptionKeyHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION EncryptionKey_Release(EncryptionKeyHandle handle)
{
	return ObjectRelease<IEncryptionKey, EncryptionKeyHandle>(handle);
}