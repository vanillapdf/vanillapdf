#include "precompiled.h"
#include "pkcs12_key.h"

#include <openssl/pkcs12.h>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include <openssl/err.h>

namespace gotchangpdf
{
	class PKCS12Key::PKCS12KeyImpl
	{
	public:
		explicit PKCS12KeyImpl(const Buffer& data);
		PKCS12KeyImpl(const Buffer& data, const Buffer& password);
		BufferPtr Decrypt(const Buffer& data) const;
		bool Equals(const Buffer& issuer, const Buffer& serial) const;

		~PKCS12KeyImpl();

	private:
		PKCS12 *p12 = nullptr;
		EVP_PKEY *key = nullptr;
		X509 *cert = nullptr;
	};

	#pragma region Forwards
	// Forwards to implementation
	PKCS12Key::PKCS12Key(const Buffer& data)
	{
		m_impl = std::make_unique<PKCS12KeyImpl>(data);
	}

	PKCS12Key::PKCS12Key(const Buffer& data, const Buffer& password)
	{
		m_impl = std::make_unique<PKCS12KeyImpl>(data, password);
	}

	BufferPtr PKCS12Key::Decrypt(const Buffer& data) const
	{
		return m_impl->Decrypt(data);
	}

	bool PKCS12Key::Equals(const Buffer& issuer, const Buffer& serial) const
	{
		return m_impl->Equals(issuer, serial);
	}
	#pragma endregion

	static std::mutex openssl_lock;
	static void InitializeOpenSSL()
	{
		static bool initialized = false;
		if (initialized) return;

		std::lock_guard<std::mutex> lock(openssl_lock);

		OpenSSL_add_all_algorithms();
		OpenSSL_add_all_ciphers();
		OpenSSL_add_all_digests();

		initialized = true;
	}

	// Actual implementation
	PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data) : PKCS12KeyImpl(data, Buffer()) {}

	PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data, const Buffer& password)
	{
		InitializeOpenSSL();

		BIO* bio = BIO_new_mem_buf((void*)data.data(), data.size());
		p12 = d2i_PKCS12_bio(bio, nullptr);
		if (nullptr == p12)
			throw GeneralException("Could not parse der structure PKCS#12");

		STACK_OF(X509) *additional_certs = NULL;
		int parsed = PKCS12_parse(p12, password.data(), &key, &cert, &additional_certs);
		if (1 != parsed) throw GeneralException("Could not parse PKCS#12");
	}

	BufferPtr PKCS12Key::PKCS12KeyImpl::Decrypt(const Buffer& data) const
	{
		ENGINE *rsa = ENGINE_get_default_RSA();
		EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, rsa);
		EVP_PKEY_decrypt_init(ctx);

		size_t outlen = 0;
		EVP_PKEY_decrypt(ctx, nullptr, &outlen, (unsigned char *)data.data(), data.size());

		BufferPtr output(outlen);
		EVP_PKEY_decrypt(ctx, (unsigned char *)output->data(), &outlen, (unsigned char *)data.data(), data.size());

		if (output->size() != outlen)
			output->resize(outlen);

		return output;
	}

	bool PKCS12Key::PKCS12KeyImpl::Equals(const Buffer& issuer, const Buffer& serial) const
	{
		auto issuer_name = cert->cert_info->issuer;
		ASN1_INTEGER* serial_asn = cert->cert_info->serialNumber;

		Buffer m_issuer(issuer_name->bytes->data, issuer_name->bytes->length);
		Buffer m_serial(serial_asn->data, serial_asn->length);

		return m_issuer.Equals(issuer) && m_serial.Equals(serial);
	}

	PKCS12Key::PKCS12KeyImpl::~PKCS12KeyImpl()
	{
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
	}
}