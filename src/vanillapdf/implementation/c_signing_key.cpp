#include "precompiled.h"

#include "utils/signing_key_interface.h"
#include "utils/pkcs12_key.h"

#include "vanillapdf/c_signing_key.h"
#include "c_helper.h"

#include <sstream>

using namespace vanillapdf;
using namespace vanillapdf::syntax;

class CustomSigningKey : public ISigningKey {
public:
	CustomSigningKey(
		SigningKeyInitializeFunction sign_init,
		SigningKeyUpdateFunction sign_update,
		SigningKeyFinalFunction sign_final,
		SigningKeyCleanupFunction sign_cleanup
	) : m_init(sign_init),
		m_update(sign_update),
		m_final(sign_final),
		m_cleanup(sign_cleanup) {
		// These are only assertions, because those parameters shall be handled
		// at the function call level.
		// RETURN_ERROR_PARAM_VALUE_IF_NULL(initialize);
		// If any of the parameters is nullptr, the function will crash
		// on its invocation.

		assert(m_init != nullptr && "Invalid initialization pointer");
		assert(m_update != nullptr && "Invalid update pointer");
		assert(m_final != nullptr && "Invalid final pointer");
		assert(m_cleanup != nullptr && "Invalid cleanup pointer");
	}

	virtual void SignInitialize(MessageDigestAlgorithm algorithm) {

		MessageDigestAlgorithmType algorithm_type = MessageDigestAlgorithmType_None;
		switch (algorithm) {
			case MessageDigestAlgorithm::None:
				algorithm_type = MessageDigestAlgorithmType_None; break;
			case MessageDigestAlgorithm::MDNULL:
				algorithm_type = MessageDigestAlgorithmType_MDNULL; break;
			case MessageDigestAlgorithm::MD2:
				algorithm_type = MessageDigestAlgorithmType_MD2; break;
			case MessageDigestAlgorithm::MD4:
				algorithm_type = MessageDigestAlgorithmType_MD4; break;
			case MessageDigestAlgorithm::MD5:
				algorithm_type = MessageDigestAlgorithmType_MD5; break;
			case MessageDigestAlgorithm::SHA1:
				algorithm_type = MessageDigestAlgorithmType_SHA1; break;
			case MessageDigestAlgorithm::SHA224:
				algorithm_type = MessageDigestAlgorithmType_SHA224; break;
			case MessageDigestAlgorithm::SHA256:
				algorithm_type = MessageDigestAlgorithmType_SHA256; break;
			case MessageDigestAlgorithm::SHA384:
				algorithm_type = MessageDigestAlgorithmType_SHA384; break;
			case MessageDigestAlgorithm::SHA512:
				algorithm_type = MessageDigestAlgorithmType_SHA512; break;
			case MessageDigestAlgorithm::MDC2:
				algorithm_type = MessageDigestAlgorithmType_MDC2; break;
			case MessageDigestAlgorithm::RIPEMD160:
				algorithm_type = MessageDigestAlgorithmType_RIPEMD160; break;
			case MessageDigestAlgorithm::WHIRLPOOL:
				algorithm_type = MessageDigestAlgorithmType_WHIRLPOOL; break;
			default:
				throw GeneralException("Unknown digest algorithm");
		}

		m_init(algorithm_type);
	}

	virtual void SignUpdate(const Buffer& data) {
		auto input_ptr = reinterpret_cast<const BufferHandleTag*>(&data);
		error_type rv = m_update(input_ptr);
		if (VANILLAPDF_ERROR_SUCCESS != rv) {
			std::stringstream ss;
			ss << "Custom key sign update operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}
	}

	virtual void SignUpdate(IInputStreamPtr data, types::stream_size length) {
		(void) data; (void) length;
		throw NotSupportedException("Stream signing not yet supported on interface");
	}

	virtual BufferPtr SignFinal() {
		BufferHandle output_ptr = nullptr;

		error_type rv = m_final(&output_ptr);
		if (VANILLAPDF_ERROR_SUCCESS != rv) {
			std::stringstream ss;
			ss << "Custom key sign final operation returned: " << rv;
			throw UserCancelledException(ss.str());
		}

		if (output_ptr == nullptr) {
			std::stringstream ss;
			ss << "Custom key sign final operation succeeded, but did not fill the signed data pointer";
			throw UserCancelledException(ss.str());
		}

		Buffer *result = reinterpret_cast<Buffer*>(output_ptr);
		return result;
	}

	~CustomSigningKey() {
		m_cleanup();
	}

private:
	SigningKeyInitializeFunction m_init;
	SigningKeyUpdateFunction m_update;
	SigningKeyFinalFunction m_final;
	SigningKeyCleanupFunction m_cleanup;
};

VANILLAPDF_API error_type CALLING_CONVENTION SigningKey_CreateCustom(
	SigningKeyInitializeFunction sign_init,
	SigningKeyUpdateFunction sign_update,
	SigningKeyFinalFunction sign_final,
	SigningKeyCleanupFunction sign_cleanup,
	SigningKeyHandle* result
) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(sign_init);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(sign_update);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(sign_final);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(sign_cleanup);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		Deferred<CustomSigningKey> key = make_deferred<CustomSigningKey>(sign_init, sign_update, sign_final, sign_cleanup);
		auto ptr = static_cast<ISigningKey*>(key.AddRefGet());
		*result = reinterpret_cast<SigningKeyHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SigningKey_Release(SigningKeyHandle handle) {
	return ObjectRelease<ISigningKey, SigningKeyHandle>(handle);
}