#include "precompiled.h"

#include "utils/misc_utils.h"
#include "utils/pkcs12_key.h"
#include "utils/license_info.h"
#include "utils/streams/input_stream.h"

#include <fstream>

#if defined(VANILLAPDF_HAVE_OPENSSL)

#include <openssl/pkcs12.h>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/rsa.h>

#endif

namespace vanillapdf {

class PKCS12Key::PKCS12KeyImpl {
public:
	explicit PKCS12KeyImpl(const Buffer& data);
	explicit PKCS12KeyImpl(const std::string& path);
	PKCS12KeyImpl(const std::string& path, const Buffer& password);
	PKCS12KeyImpl(const Buffer& data, const Buffer& password);

	// IEncryptionKey
	BufferPtr Decrypt(const Buffer& data);
	bool ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const;

	// ISigningKey
	void SignInitialize(MessageDigestAlgorithm algorithm);
	void SignUpdate(BufferPtr data);
	void SignUpdate(IInputStreamPtr data, types::stream_size length);
	BufferPtr SignFinal();
	void SignCleanup();

#if defined(VANILLAPDF_HAVE_OPENSSL)

private:
	PKCS12 *p12 = nullptr;
	EVP_PKEY *key = nullptr;
	EVP_PKEY_CTX *encryption_context = nullptr;
	X509 *cert = nullptr;
	ENGINE *rsa = nullptr;

	PKCS7 *p7 = nullptr;
	BIO *p7bio = nullptr;

#endif

	BufferArrayPtr m_certificates;

	void Load(const Buffer& data, const Buffer& password);
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

BufferPtr PKCS12Key::Decrypt(const Buffer& data) {
	return m_impl->Decrypt(data);
}

bool PKCS12Key::ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const {
	return m_impl->ContainsPrivateKey(issuer, serial);
}

void PKCS12Key::SignInitialize(MessageDigestAlgorithm algorithm) {
	return m_impl->SignInitialize(algorithm);
}

void PKCS12Key::SignUpdate(const BufferPtr data) {
	return m_impl->SignUpdate(data);
}

void PKCS12Key::SignUpdate(IInputStreamPtr data, types::stream_size length) {
	return m_impl->SignUpdate(data, length);
}

BufferPtr PKCS12Key::SignFinal() {
	return m_impl->SignFinal();
}

void PKCS12Key::SignCleanup() {
	return m_impl->SignCleanup();
}

#pragma endregion

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

	Load(data, password);
}

PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data) : PKCS12KeyImpl(data, Buffer()) {}

PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data, const Buffer& password) {
	Load(data, password);
}

void PKCS12Key::PKCS12KeyImpl::Load(const Buffer& data, const Buffer& password) {

	MiscUtils::InitializeOpenSSL();

	// The const cast seems to be unnecessary, but on my linux VM
	// with OpenSSL 1.0.1f method BIO_new_mem_buf takes only void*
	auto buffer_data = const_cast<char *>(data.data());
	int buffer_length = ValueConvertUtils::SafeConvert<int>(data.size());

	BIO* bio = BIO_new_mem_buf(buffer_data, buffer_length);
	p12 = d2i_PKCS12_bio(bio, nullptr);
	if (nullptr == p12) {
		throw GeneralException("Could not parse der structure PKCS#12, " + MiscUtils::GetLastOpensslError());
	}

	STACK_OF(X509) *additional_certs = NULL;
	int parsed = PKCS12_parse(p12, password.data(), &key, &cert, &additional_certs);
	if (1 != parsed) {
		throw GeneralException("Could not parse PKCS#12, " + MiscUtils::GetLastOpensslError());
	}

	auto additional_certs_size = sk_X509_num(additional_certs);
	for (decltype(additional_certs_size) i = 0; i < additional_certs_size; ++i) {
		auto additional_cert = sk_X509_value(additional_certs, i);

		int length = i2d_X509(additional_cert, nullptr);
		if (length < 0) {
			throw GeneralException("Could not get PKCS#7 size, " + MiscUtils::GetLastOpensslError());
		}

		BufferPtr additional_cert_data = make_deferred_container<Buffer>(length);
		auto data_pointer = (unsigned char *) additional_cert_data->data();
		int converted = i2d_X509(additional_cert, &data_pointer);
		if (converted < 0) {
			throw GeneralException("Could not convert PKCS#7, " + MiscUtils::GetLastOpensslError());
		}

		m_certificates->Append(additional_cert_data);
	}

	sk_X509_pop_free(additional_certs, X509_free);
}

BufferPtr PKCS12Key::PKCS12KeyImpl::Decrypt(const Buffer& data) {

	// Decryption is a licensed feature
	if (!LicenseInfo::IsValid()) {
		throw LicenseRequiredException("File decryption is a licensed feature");
	}

#if defined(VANILLAPDF_HAVE_OPENSSL)

	if (rsa == nullptr) {
		rsa = ENGINE_get_default_RSA();
	}

	if (encryption_context == nullptr) {
		encryption_context = EVP_PKEY_CTX_new(key, rsa);
	}

	int initialized = EVP_PKEY_decrypt_init(encryption_context);
	if (1 != initialized) {
		throw GeneralException("Could not initialize encryption engine");
	}

	size_t outlen = 0;
	int has_length = EVP_PKEY_decrypt(encryption_context, nullptr, &outlen, (unsigned char *) data.data(), data.std_size());
	if (has_length != 1) {
		throw GeneralException("Could not get decrypt message length");
	}

	BufferPtr output = make_deferred_container<Buffer>(outlen);
	int decrypted = EVP_PKEY_decrypt(encryption_context, (unsigned char *) output->data(), &outlen, (unsigned char *) data.data(), data.std_size());
	if (decrypted != 1) {
		throw GeneralException("Could not get decrypt message");
	}

	return output;

#else

	(void) data;
	throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

bool PKCS12Key::PKCS12KeyImpl::ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const {

#if defined(VANILLAPDF_HAVE_OPENSSL)

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

void PKCS12Key::PKCS12KeyImpl::SignInitialize(MessageDigestAlgorithm algorithm) {

	// Signing is a licensed feature
	if (!LicenseInfo::IsValid()) {
		throw LicenseRequiredException("Document signing is a licensed feature");
	}

#if defined(VANILLAPDF_HAVE_OPENSSL)

	if (p7 != nullptr) {
		PKCS7_free(p7);
		p7 = nullptr;
	}

	p7 = PKCS7_new();

	if (p7 == nullptr) {
		throw GeneralException("Could not create PKCS#7");
	}

	int type_set = PKCS7_set_type(p7, NID_pkcs7_signed);
	if (type_set != 1) {
		throw GeneralException("Could not set PKCS#7 type");
	}

	auto message_digest = MiscUtils::GetAlgorithm(algorithm);
	auto signer_info = PKCS7_add_signature(p7, cert, key, message_digest);
	if (signer_info == nullptr) {
		throw GeneralException("Could not add signature");
	}

	int content_type_added = PKCS7_add_signed_attribute(signer_info, NID_pkcs9_contentType, V_ASN1_OBJECT, OBJ_nid2obj(NID_pkcs7_data));
	if (content_type_added != 1) {
		throw GeneralException("Could not add signed attribute");
	}

	int signing_time_added = PKCS7_add0_attrib_signing_time(signer_info, nullptr);
	if (signing_time_added != 1) {
		throw GeneralException("Could not add signing time");
	}

	int certificate_added = PKCS7_add_certificate(p7, cert);
	if (certificate_added != 1) {
		throw GeneralException("Could not add certificate");
	}

	auto extra_certificates_size = m_certificates->GetSize();
	for (decltype(extra_certificates_size) i = 0; i < extra_certificates_size; ++i) {
		auto extra_certificate_data = m_certificates[i];

		auto extra_certificate_raw_data = (const unsigned char *) extra_certificate_data->data();
		auto extra_certificate_raw_data_size = ValueConvertUtils::SafeConvert<long>(extra_certificate_data->size());
		auto extra_certificate = d2i_X509(nullptr, &extra_certificate_raw_data, extra_certificate_raw_data_size);
		if (extra_certificate == nullptr) {
			throw GeneralException("Extra certificate is invalid");
		}

		SCOPE_GUARD([extra_certificate]() { X509_free(extra_certificate); });

		int extra_certificate_added = PKCS7_add_certificate(p7, extra_certificate);
		if (extra_certificate_added != 1) {
			throw GeneralException("Could not add extra certificate");
		}
	}

	int content_set = PKCS7_content_new(p7, NID_pkcs7_data);
	if (content_set != 1) {
		throw GeneralException("Could not set signing content");
	}

	int detached_set = PKCS7_set_detached(p7, 1);
	if (detached_set != 1) {
		throw GeneralException("Could not set PKCS#7 detached");
	}

	if (p7bio != nullptr) {
		BIO_free(p7bio);
		p7bio = nullptr;
	}

	p7bio = PKCS7_dataInit(p7, nullptr);

	if (p7bio == nullptr) {
		throw GeneralException("Could not initialize signing data");
	}

#else

	(void) algorithm;
	throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

void PKCS12Key::PKCS12KeyImpl::SignUpdate(BufferPtr data) {
	auto input_stream = data->ToInputStream();
	SignUpdate(input_stream, data->size());
}

void PKCS12Key::PKCS12KeyImpl::SignUpdate(IInputStreamPtr data, types::stream_size length) {

	// Signing is a licensed feature
	if (!LicenseInfo::IsValid()) {
		throw LicenseRequiredException("Document signing is a licensed feature");
	}

#if defined(VANILLAPDF_HAVE_OPENSSL)

	types::stream_size read_total = 0;

	Buffer buffer(constant::BUFFER_SIZE);
	for (;;) {
		if (read_total == length) {
			break;
		}

		types::stream_size block_size = std::min<types::stream_size>(length - read_total, constant::BUFFER_SIZE);
		types::size_type block_size_converted = ValueConvertUtils::SafeConvert<types::size_type>(block_size);
		types::stream_size read = data->Read(buffer, block_size_converted);
		int read_converted = ValueConvertUtils::SafeConvert<int>(read);

		int written = BIO_write(p7bio, buffer.data(), read_converted);
		if (written != read_converted) {
			throw GeneralException("Could not write data");
		}

		read_total = read_total + read;
	}

#else

	(void) data;
	throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

BufferPtr PKCS12Key::PKCS12KeyImpl::SignFinal() {

	// Signing is a licensed feature
	if (!LicenseInfo::IsValid()) {
		throw LicenseRequiredException("Document signing is a licensed feature");
	}

#if defined(VANILLAPDF_HAVE_OPENSSL)

	int finalized = PKCS7_dataFinal(p7, p7bio);
	if (finalized != 1) {
		throw GeneralException("Could not finalize PKCS#7");
	}

	int length = i2d_PKCS7(p7, nullptr);
	if (length < 0) {
		throw GeneralException("Could not get PKCS#7 size");
	}

	BufferPtr result = make_deferred_container<Buffer>(length);
	auto data_pointer = (unsigned char *) result->data();
	int converted = i2d_PKCS7(p7, &data_pointer);
	if (converted < 0) {
		throw GeneralException("Could not convert PKCS#7");
	}

	return result;

#else

	throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

void PKCS12Key::PKCS12KeyImpl::SignCleanup() {

#if defined(VANILLAPDF_HAVE_OPENSSL)

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

	if (nullptr != encryption_context) {
		EVP_PKEY_CTX_free(encryption_context);
		encryption_context = nullptr;
	}

	if (nullptr != rsa) {
		ENGINE_free(rsa);
		rsa = nullptr;
	}

	if (nullptr != p7) {
		PKCS7_free(p7);
		p7 = nullptr;
	}

	if (nullptr != p7bio) {
		BIO_free(p7bio);
		p7bio = nullptr;
	}

#endif
}

} // vanillapdf
