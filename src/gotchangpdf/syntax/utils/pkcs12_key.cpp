#include "precompiled.h"
#include "pkcs12_key.h"

#include <openssl/pkcs12.h>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include <openssl/err.h>
//#include <openssl/rc4.h>
//#include <openssl/aes.h>
//#include <openssl/x509.h>
//#include <openssl/md5.h>
//#include <openssl/pkcs7.h>
//#include <openssl/objects.h>

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

		void ProcessSafeBag(PKCS12_SAFEBAG* bag, const Buffer& password);
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

	// Actual implementation
	PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data) : PKCS12KeyImpl(data, Buffer()) {}

	PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data, const Buffer& password)
	{
		OpenSSL_add_all_algorithms();
		OpenSSL_add_all_ciphers();
		OpenSSL_add_all_digests();

		BIO* bio = BIO_new_mem_buf((void*)data.data(), data.size());
		p12 = d2i_PKCS12_bio(bio, nullptr);
		if (nullptr == p12)
			throw GeneralException("Could not parse PKCS#12");

		auto asafes = PKCS12_unpack_authsafes(p12);

		STACK_OF(PKCS12_SAFEBAG) *bags = nullptr;
		for (int i = 0; i < sk_PKCS7_num(asafes); i++) {
			auto p7 = sk_PKCS7_value(asafes, i);
			auto bagnid = OBJ_obj2nid(p7->type);
			if (bagnid == NID_pkcs7_data) {
				bags = PKCS12_unpack_p7data(p7);
			}
			else if (bagnid == NID_pkcs7_encrypted) {
				bool enc = PKCS7_type_is_encrypted(p7);
				bags = PKCS12_unpack_p7encdata(p7, password.data(), password.size());
				unsigned long err = ERR_get_error();
				char *msg = ERR_error_string(err, nullptr);
				int a = 0;
			}

			PKCS12_SAFEBAG *bag_value = sk_PKCS12_SAFEBAG_value(bags, i);
			ProcessSafeBag(bag_value, password);

			sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);
			bags = nullptr;
		}
	}

	void PKCS12Key::PKCS12KeyImpl::ProcessSafeBag(PKCS12_SAFEBAG* bag, const Buffer& password)
	{
		auto nid = M_PKCS12_bag_type(bag);

		if (NID_keyBag == nid) {
			auto p8 = bag->value.keybag;
			key = EVP_PKCS82PKEY(p8);
		}

		if (NID_certBag == nid) {

			//if (PKCS12_SAFEBAG_get0_attr(bag, NID_localKeyID)) {
			//}
			if (M_PKCS12_cert_bag_type(bag) != NID_x509Certificate) return;
			cert = PKCS12_certbag2x509(bag);
		}

		if (NID_pkcs8ShroudedKeyBag == nid) {
			auto p8 = PKCS12_decrypt_skey(bag, password.data(), password.size());
			key = EVP_PKCS82PKEY(p8);
		}

		if (NID_safeContentsBag == nid) {
			// TODO
		}
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
		return output;
	}

	bool PKCS12Key::PKCS12KeyImpl::Equals(const Buffer& issuer, const Buffer& serial) const
	{
		auto issuer_name = cert->cert_info->issuer;
		auto serial_asn = cert->cert_info->serialNumber;

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