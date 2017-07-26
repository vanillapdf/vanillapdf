#include "precompiled.h"
#include "utils/pkcs12_key.h"

#include <fstream>

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

#include <openssl/pkcs12.h>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/rsa.h>

#endif

namespace gotchangpdf {

class PKCS12Key::PKCS12KeyImpl {
public:
	explicit PKCS12KeyImpl(const Buffer& data);
	explicit PKCS12KeyImpl(const std::string& path);
	PKCS12KeyImpl(const std::string& path, const Buffer& password);
	PKCS12KeyImpl(const Buffer& data, const Buffer& password);

	// IEncryptionKey
	BufferPtr Decrypt(const Buffer& data) const;
	bool ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const;

	// ISigningKey
	BufferPtr Sign(const Buffer& data, MessageDigestAlgorithm algorithm) const;

	// This is only a helper method and may be moved in the future
	static const EVP_MD* GetAlgorithm(MessageDigestAlgorithm algorithm);

	~PKCS12KeyImpl();

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

private:
	PKCS12 *p12 = nullptr;
	EVP_PKEY *key = nullptr;
	EVP_PKEY_CTX *ctx = nullptr;
	X509 *cert = nullptr;
	ENGINE *rsa = nullptr;

#endif

	void Initialize(const Buffer& data, const Buffer& password);
};

#pragma region Forwards
// Forwards to implementation
PKCS12Key::PKCS12Key(const Buffer& data) {
	m_impl = make_unique<PKCS12KeyImpl>(data);
}

PKCS12Key::PKCS12Key(const std::string& path) {
	m_impl = make_unique<PKCS12KeyImpl>(path);
}

PKCS12Key::PKCS12Key(const std::string& path, const Buffer& password) {
	m_impl = make_unique<PKCS12KeyImpl>(path, password);
}

PKCS12Key::PKCS12Key(const Buffer& data, const Buffer& password) {
	m_impl = make_unique<PKCS12KeyImpl>(data, password);
}

BufferPtr PKCS12Key::Decrypt(const Buffer& data) const {
	return m_impl->Decrypt(data);
}

bool PKCS12Key::ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const {
	return m_impl->ContainsPrivateKey(issuer, serial);
}

BufferPtr PKCS12Key::Sign(const Buffer& data, MessageDigestAlgorithm algorithm) const {
	return m_impl->Sign(data, algorithm);
}

#pragma endregion

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

static void InitializeOpenSSL() {
	static bool initialized = false;
	if (initialized) {
		return;
	}

	static std::mutex openssl_lock;
	std::lock_guard<std::mutex> lock(openssl_lock);
	if (initialized) {
		return;
	}

	OpenSSL_add_all_algorithms();
	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_digests();

	initialized = true;
}

#endif

	// Actual implementation
PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const std::string& path) : PKCS12KeyImpl(path, Buffer()) {}
PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const std::string& path, const Buffer& password) {
	std::ifstream file(path, std::ios::in | std::ios::binary);
	SCOPE_GUARD_CAPTURE_REFERENCES(file.close());

	Buffer data;
	Buffer buffer(constant::BUFFER_SIZE);

	while (!file.eof()) {
		file.read(buffer.data(), constant::BUFFER_SIZE);
		auto read = file.gcount();
		if (0 == read) {
			break;
		}

		auto read_converted = ValueConvertUtils::SafeConvert<Buffer::iterator::difference_type>(read);
		data.insert(data.end(), buffer.begin(), buffer.begin() + read_converted);
	}

	Initialize(data, password);
}

PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data) : PKCS12KeyImpl(data, Buffer()) {}

PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data, const Buffer& password) {
	Initialize(data, password);
}

void PKCS12Key::PKCS12KeyImpl::Initialize(const Buffer& data, const Buffer& password) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	InitializeOpenSSL();

	void* buffer_data = const_cast<char *>(data.data());
	int buffer_length = ValueConvertUtils::SafeConvert<int>(data.size());

	BIO* bio = BIO_new_mem_buf(buffer_data, buffer_length);
	p12 = d2i_PKCS12_bio(bio, nullptr);
	if (nullptr == p12) {
		throw GeneralException("Could not parse der structure PKCS#12");
	}

	STACK_OF(X509) *additional_certs = NULL;
	int parsed = PKCS12_parse(p12, password.data(), &key, &cert, &additional_certs);
	if (1 != parsed) {
		throw GeneralException("Could not parse PKCS#12");
	}

	rsa = ENGINE_get_default_RSA();
	ctx = EVP_PKEY_CTX_new(key, rsa);
	int initialized = EVP_PKEY_decrypt_init(ctx);
	if (1 != initialized) {
		throw GeneralException("Could not initialize encryption engine");
	}

#else
	(void) data; (void) password;
	throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

BufferPtr PKCS12Key::PKCS12KeyImpl::Decrypt(const Buffer& data) const {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	size_t outlen = 0;
	EVP_PKEY_decrypt(ctx, nullptr, &outlen, (unsigned char *) data.data(), data.size());

	BufferPtr output = make_deferred<Buffer>(outlen);
	EVP_PKEY_decrypt(ctx, (unsigned char *) output->data(), &outlen, (unsigned char *) data.data(), data.size());

	if (output->size() != outlen)
		output->resize(outlen);

	return output;

#else

	(void) data;
	throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

bool PKCS12Key::PKCS12KeyImpl::ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	// X509_NAME is conflicting with some windows headers
	auto issuer_name = X509_get_issuer_name(cert);
	ASN1_INTEGER* serial_asn = X509_get_serialNumber(cert);

	// Convert issuer to null terminated string
	char* oneline = X509_NAME_oneline(issuer_name, nullptr, 0);
	SCOPE_GUARD([oneline]() {OPENSSL_free(oneline); });

	if (oneline == nullptr) {
		LOG_ERROR_GLOBAL << "Could not print issuer to buffer";
		return false;
	}

	Buffer m_issuer(oneline);
	Buffer m_serial(serial_asn->data, serial_asn->length);
	return m_issuer.Equals(issuer) && m_serial.Equals(serial);

#else

	(void) issuer; (void) serial;
	throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

BufferPtr PKCS12Key::PKCS12KeyImpl::Sign(const Buffer& data, MessageDigestAlgorithm algorithm) const {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	EVP_MD_CTX signing_context;
	EVP_MD_CTX_init(&signing_context);

	auto message_digest = GetAlgorithm(algorithm);

	int initialized = EVP_DigestSignInit(&signing_context, nullptr, message_digest, nullptr, key);
	if (initialized != 1) {
		throw GeneralException("Could not initialize signing context");
	}

	// We want to capture input by value, because it might be out of scope
	// In order to call non-const method we have to tag the lambda mutable
	auto cleanup_lambda = [signing_context]() mutable {
		int cleaned = EVP_MD_CTX_cleanup(&signing_context);

		assert(cleaned == 1 && "Could not clean signing context");

		// This is a special case, because we are executing this code in
		// destructor of scope guard. In case it gets executed, because
		// of an exception, there is a risk of undefined behavior,
		// when another exception is thrown. We just carefully ignore logging.
		if (cleaned != 1 && !std::uncaught_exception()) {
			LOG_ERROR_GLOBAL << "Could not clean signing context";
		}
	};

	SCOPE_GUARD(cleanup_lambda);

	int updated = EVP_DigestSignUpdate(&signing_context, data.data(), data.size());
	if (updated != 1) {
		throw GeneralException("Could not update signing context");
	}

	size_t length = 0;
	int finalized = EVP_DigestSignFinal(&signing_context, nullptr, &length);
	if (finalized != 1) {
		throw GeneralException("Could not get signing digest length");
	}

	BufferPtr result = make_deferred<Buffer>(length);
	int digested = EVP_DigestSignFinal(&signing_context, (unsigned char *) result->data(), &length);
	if (digested != 1) {
		throw GeneralException("Could not read signing digest");
	}

	return result;

#else

	(void) data; (void) algorithm;
	throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

const EVP_MD* PKCS12Key::PKCS12KeyImpl::GetAlgorithm(MessageDigestAlgorithm algorithm) {
	if (algorithm == MessageDigestAlgorithm::None) {
		throw GeneralException("No message digest algorithm was selected");
	}

	if (algorithm == MessageDigestAlgorithm::MDNULL) {
		return EVP_md_null();
	}

	if (algorithm == MessageDigestAlgorithm::MD2) {
	#ifndef OPENSSL_NO_MD2
		return EVP_md2();
	#else
		throw NotSupportedException("OpenSSL was compiled without MD2 message digest support");
	#endif
	}

	if (algorithm == MessageDigestAlgorithm::MD4) {
		return EVP_md4();
	}

	if (algorithm == MessageDigestAlgorithm::MD5) {
		return EVP_md5();
	}

	if (algorithm == MessageDigestAlgorithm::SHA) {
		return EVP_sha();
	}

	if (algorithm == MessageDigestAlgorithm::SHA1) {
		return EVP_sha1();
	}

	if (algorithm == MessageDigestAlgorithm::DSS) {
		return EVP_dss();
	}

	if (algorithm == MessageDigestAlgorithm::DSS1) {
		return EVP_dss1();
	}

	if (algorithm == MessageDigestAlgorithm::ECDSA) {
		return EVP_ecdsa();
	}

	if (algorithm == MessageDigestAlgorithm::SHA224) {
		return EVP_sha224();
	}

	if (algorithm == MessageDigestAlgorithm::SHA256) {
		return EVP_sha256();
	}

	if (algorithm == MessageDigestAlgorithm::SHA384) {
		return EVP_sha384();
	}

	if (algorithm == MessageDigestAlgorithm::SHA512) {
		return EVP_sha512();
	}

	if (algorithm == MessageDigestAlgorithm::MDC2) {
	#ifndef OPENSSL_NO_MDC2
		return EVP_mdc2();
	#else
		throw NotSupportedException("OpenSSL was compiled without MDC2 message digest support");
	#endif
	}

	if (algorithm == MessageDigestAlgorithm::RIPEMD160) {
		return EVP_ripemd160();
	}

	if (algorithm == MessageDigestAlgorithm::WHIRLPOOL) {
		return EVP_whirlpool();
	}

	throw GeneralException("Unknown message digest algorithm");
}

PKCS12Key::PKCS12KeyImpl::~PKCS12KeyImpl() {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	if (nullptr != p12) {
		PKCS12_free(p12);
		p12 = nullptr;
	}

	if (nullptr != key) {
		EVP_PKEY_free(key);
		key = nullptr;
	}

	if (nullptr != cert) {
		X509_free(cert);
		cert = nullptr;
	}

	if (nullptr != ctx) {
		EVP_PKEY_CTX_free(ctx);
		ctx = nullptr;
	}

	if (nullptr != rsa) {
		ENGINE_free(rsa);
		rsa = nullptr;
	}

#endif

}

} // gotchangpdf
