#include "precompiled.h"
#include "syntax/utils/encryption.h"

#include "utils/buffer.h"

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

#include <openssl/rc4.h>
#include <openssl/aes.h>
#include <openssl/x509.h>
#include <openssl/md5.h>
#include <openssl/pkcs7.h>
#include <openssl/objects.h>
#include <openssl/evp.h>

// For some interesting reason, openss pulls in windows.h header
// which defines many unnecessary macros
#include <openssl/rand.h>
#undef max
#undef min
#undef X509_NAME
//-----------------------------

#endif

namespace gotchangpdf {
namespace syntax {

	// Hard-coded in PDF specification
const uint8_t HARDCODED_PFD_PAD[] = {
	0x28, 0xBF, 0x4E, 0x5E,
	0x4E, 0x75, 0x8A, 0x41,
	0x64, 0x00, 0x4E, 0x56,
	0xFF, 0xFA, 0x01, 0x08,
	0x2E, 0x2E, 0x00, 0xB6,
	0xD0, 0x68, 0x3E, 0x80,
	0x2F, 0x0C, 0xA9, 0xFE,
	0x64, 0x53, 0x69, 0x7A
};

const uint8_t AES_ADDITIONAL_SALT[] = {
	0x73, 0x41, 0x6C, 0x54
};

const int HARDCODED_PFD_PAD_LENGTH = sizeof(HARDCODED_PFD_PAD);
const int AES_ADDITIONAL_SALT_LENGTH = sizeof(AES_ADDITIONAL_SALT);

const types::size_type AES_CBC_IV_LENGTH = 16;
const types::size_type AES_CBC_BLOCK_SIZE = 16;

BufferPtr EncryptionUtils::ComputeObjectKey(
	const Buffer& key,
	types::big_uint objNumber,
	types::ushort genNumber,
	EncryptionAlgorithm alg) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	Buffer object_key(MD5_DIGEST_LENGTH);

	uint8_t object_info[5];
	object_info[0] = objNumber & 0xFF;
	object_info[1] = (objNumber >> 8) & 0xFF;
	object_info[2] = (objNumber >> 16) & 0xFF;
	object_info[3] = (genNumber) & 0xFF;
	object_info[4] = (genNumber >> 8) & 0xFF;

	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, key.data(), key.size());
	MD5_Update(&ctx, object_info, sizeof(object_info));

	if (alg == EncryptionAlgorithm::AES) {
		MD5_Update(&ctx, &AES_ADDITIONAL_SALT[0], AES_ADDITIONAL_SALT_LENGTH);
	}

	MD5_Final((unsigned char*) object_key.data(), &ctx);

	auto key_length = std::min<size_t>(key.size() + 5, 16);
	return BufferPtr(object_key.begin(), object_key.begin() + key_length);

#else
	(void) key; (void) objNumber; (void) genNumber; (void) alg;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::PadTruncatePassword(const Buffer& password) {
	BufferPtr result(sizeof(HARDCODED_PFD_PAD));
	//result->reserve(sizeof(pad));
	std::copy_n(password.begin(), std::min(password.size(), sizeof(HARDCODED_PFD_PAD)), result.begin());
	std::copy_n(std::begin(HARDCODED_PFD_PAD), sizeof(HARDCODED_PFD_PAD) - password.size(), result.begin() + password.size());
	return result;
}

BufferPtr EncryptionUtils::ComputeRC4(const Buffer& key, types::size_type key_length, const Buffer& data) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	BufferPtr result(data.size());

	RC4_KEY rc_key;
	RC4_set_key(&rc_key, ValueConvertUtils::SafeConvert<int>(key_length), (unsigned char*) key.data());
	RC4(&rc_key, data.size(), (unsigned char*) data.data(), (unsigned char*) result->data());

	return result;

#else
	(void) key; (void) key_length; (void) data;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::ComputeRC4(const Buffer& key, const Buffer& data) {
	return ComputeRC4(key, key.size(), data);
}

BufferPtr EncryptionUtils::AESDecrypt(const Buffer& key, const Buffer& data) {
	return AESDecrypt(key, key.size(), data);
}

BufferPtr EncryptionUtils::AESDecrypt(const Buffer& key, types::size_type key_length, const Buffer& data) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	assert(data.size() >= static_cast<size_t>(AES_CBC_IV_LENGTH));
	if (data.size() < static_cast<size_t>(AES_CBC_IV_LENGTH)) {
		throw GeneralException("Cannot find IV for encrypted data");
	}

	Buffer iv(data.begin(), data.begin() + AES_CBC_IV_LENGTH);
	BufferPtr result(data.size() - AES_CBC_IV_LENGTH);

	using decrypt_key_type = unsigned char;
	const decrypt_key_type* key_data = reinterpret_cast<const decrypt_key_type*>(key.data());

	auto decrypt_key_type_size = std::numeric_limits<decrypt_key_type>::digits;
	int key_length_bits = SafeMultiply<int>(key_length, ValueConvertUtils::SafeConvert<decltype(key_length)>(decrypt_key_type_size));

	AES_KEY dec_key;
	int key_set = AES_set_decrypt_key(key_data, key_length_bits, &dec_key);
	if (0 != key_set) {
		throw GeneralException("Unable to set decryption key");
	}

	AES_cbc_encrypt((unsigned char*) data.data() + AES_CBC_IV_LENGTH, (unsigned char*) result->data(), data.size() - AES_CBC_IV_LENGTH, &dec_key, (unsigned char*) iv.data(), AES_DECRYPT);

	return RemovePkcs7Padding(result, AES_CBC_BLOCK_SIZE);

#else
	(void) key; (void) key_length; (void) data;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::AESEncrypt(const Buffer& key, const Buffer& data) {
	return AESEncrypt(key, key.size(), data);
}

BufferPtr EncryptionUtils::AESEncrypt(const Buffer& key, types::size_type key_length, const Buffer& data) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	Buffer iv(AES_CBC_IV_LENGTH);
	int rv = RAND_bytes((unsigned char*) iv.data(), AES_CBC_IV_LENGTH);
	if (1 != rv) {
		throw GeneralException("Could not generate initialization vector for AES");
	}

	BufferPtr buffer = AddPkcs7Padding(data, AES_CBC_BLOCK_SIZE);

	using decrypt_key_type = unsigned char;
	const decrypt_key_type* key_data = reinterpret_cast<const decrypt_key_type*>(key.data());

	auto decrypt_key_type_size = std::numeric_limits<decrypt_key_type>::digits;
	int key_length_bits = SafeMultiply<int>(key_length, ValueConvertUtils::SafeConvert<decltype(key_length)>(decrypt_key_type_size));

	AES_KEY enc_key;
	int key_set = AES_set_decrypt_key(key_data, key_length_bits, &enc_key);
	if (0 != key_set) {
		throw GeneralException("Unable to set encryption key");
	}

	AES_cbc_encrypt((unsigned char*) data.data(), (unsigned char*) buffer->data(), data.size(), &enc_key, (unsigned char*) iv.data(), AES_ENCRYPT);

	buffer->insert(buffer.begin(), iv.begin(), iv.end());
	return buffer;

#else
	(void) key; (void) key_length; (void) data;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::AddPkcs7Padding(const Buffer& data, size_t block_size) {
	size_t remaining = data.size() % block_size;
	if (0 == remaining) {
		remaining = block_size;
	}

	Buffer::value_type converted = ValueConvertUtils::SafeConvert<Buffer::value_type>(remaining);

	BufferPtr result;
	result->reserve(data.size() + block_size);
	result->insert(result.begin(), data.begin(), data.end());
	result->insert(result.end(), remaining, converted);

	return result;
}

BufferPtr EncryptionUtils::RemovePkcs7Padding(const Buffer& data, size_t block_size) {
	do {
		// Empty data are invalid
		if (data.empty()) {
			break;
		}

		char bytes = data.back();
		unsigned char converted = reinterpret_cast<unsigned char&>(bytes);
		if (data.size() < converted || converted > block_size) {
			break;
		}

		// verify PKCS#7 padding
		// The value of each added byte is the number of bytes that are added, i.e. N bytes, each of value N are added
		bool padding_error = false;
		for (auto it = data.end() - converted; it != data.end(); it++) {
			if (*it != bytes) {
				padding_error = true;
				break;
			}
		}

		if (padding_error) {
			break;
		}

		return BufferPtr(data.begin(), data.end() - converted);
	} while (false);

	// I would really like to be strict about padding,
	// but in test document "example_128-aes.pdf" there is a signature field
	// that allocated space with zeroes and did not care about setting proper
	// pkcs padding. That means, that there is some trash after we decrypt the
	// signature contents and padding could not be validated
	LOG_WARNING_GLOBAL << "Pkcs padding is incorrect";
	return data;
}

bool EncryptionUtils::CheckKey(
	const Buffer& input,
	const Buffer& document_id,
	const Buffer& owner_data,
	const Buffer& user_data,
	const syntax::IntegerObject& permissions,
	const syntax::IntegerObject& revision,
	const syntax::IntegerObject& key_length,
	Buffer& decryption_key) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	Buffer decryption_key_digest(MD5_DIGEST_LENGTH);

	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, input.data(), input.size());
	MD5_Update(&ctx, owner_data.data(), owner_data.size());

	auto permissions_value = permissions.GetIntegerValue();
	uint32_t permissions_raw = reinterpret_cast<uint32_t&>(permissions_value);
	uint8_t permissions_array[sizeof(permissions_raw)];
	permissions_array[0] = permissions_raw & 0xFF;
	permissions_array[1] = (permissions_raw >> 8) & 0xFF;
	permissions_array[2] = (permissions_raw >> 16) & 0xFF;
	permissions_array[3] = (permissions_raw >> 24) & 0xFF;

	MD5_Update(&ctx, permissions_array, sizeof(permissions_array));
	MD5_Update(&ctx, document_id.data(), document_id.size());
	MD5_Final((unsigned char*) decryption_key_digest.data(), &ctx);

	auto length_bytes = ValueConvertUtils::SafeConvert<size_t>(key_length.GetIntegerValue() / 8);
	size_t decryption_key_length = std::min(length_bytes, decryption_key_digest.size());

	BufferPtr compare_data;
	if (revision >= 3) {
		Buffer temporary_digest(MD5_DIGEST_LENGTH);
		for (int i = 0; i < 50; ++i) {
			MD5_Init(&ctx);
			MD5_Update(&ctx, decryption_key_digest.data(), decryption_key_length);
			MD5_Final((unsigned char*) temporary_digest.data(), &ctx);
			std::copy_n(temporary_digest.begin(), decryption_key_length, decryption_key_digest.begin());
		}

		Buffer hardcoded_pad(&HARDCODED_PFD_PAD[0], HARDCODED_PFD_PAD_LENGTH);
		Buffer key_digest(MD5_DIGEST_LENGTH);

		MD5_Init(&ctx);
		MD5_Update(&ctx, hardcoded_pad.data(), hardcoded_pad.size());
		MD5_Update(&ctx, document_id.data(), document_id.size());
		MD5_Final((unsigned char*) key_digest.data(), &ctx);

		auto key = BufferPtr(length_bytes);
		compare_data = BufferPtr(key_digest);

		for (Buffer::value_type i = 0; i < 20; ++i) {
			for (decltype(decryption_key_length) j = 0; j < decryption_key_length; ++j) {
				key[j] = (decryption_key_digest[j] ^ i);
			}

			compare_data = EncryptionUtils::ComputeRC4(key, compare_data);
		}
	} else {
		assert(key_length.GetIntegerValue() == 40 && "Key length is not 5 bytes for revision <= 3");
		Buffer hardcoded_pad(&HARDCODED_PFD_PAD[0], HARDCODED_PFD_PAD_LENGTH);
		compare_data = EncryptionUtils::ComputeRC4(decryption_key_digest, 5, hardcoded_pad);
	}

	int compare_length = (revision >= 3 ? 16 : 32);
	if (std::equal(compare_data.begin(), compare_data.begin() + compare_length, user_data.begin())) {
		decryption_key = BufferPtr(decryption_key_digest.begin(), decryption_key_digest.begin() + decryption_key_length);
		return true;
	}

	return false;

#else
	(void) input; (void) document_id; (void) owner_data;
	(void) user_data; (void) permissions; (void) revision;
	(void) key_length; (void) decryption_key;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::GetRecipientKey
(const syntax::ArrayObject<syntax::StringObjectPtr>& enveloped_data,
	const syntax::IntegerObject& length_bits,
	EncryptionAlgorithm algorithm,
	const IEncryptionKey& key) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	auto decrypted_data = EncryptionUtils::DecryptEnvelopedData(enveloped_data, key);

	Buffer decrypted_key;
	if (length_bits == 256 && algorithm == EncryptionAlgorithm::AES) {
		decrypted_key.resize(SHA256_DIGEST_LENGTH);
		SHA256_CTX ctx;
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, decrypted_data->data(), 20);

		auto length = enveloped_data.Size();
		for (decltype(length) i = 0; i < length; ++i) {
			auto enveloped_bytes = enveloped_data.At(i);
			SHA256_Update(&ctx, enveloped_bytes->GetValue()->data(), enveloped_bytes->GetValue()->size());
		}

		SHA256_Final((unsigned char*) decrypted_key.data(), &ctx);
	} else {
		decrypted_key.resize(SHA_DIGEST_LENGTH);
		SHA_CTX ctx;
		SHA1_Init(&ctx);
		SHA1_Update(&ctx, decrypted_data->data(), 20);

		auto length = enveloped_data.Size();
		for (decltype(length) i = 0; i < length; ++i) {
			auto enveloped_bytes = enveloped_data.At(i);
			SHA1_Update(&ctx, enveloped_bytes->GetValue()->data(), enveloped_bytes->GetValue()->size());
		}

		SHA1_Final((unsigned char*) decrypted_key.data(), &ctx);
	}

	auto length_bytes = ValueConvertUtils::SafeConvert<size_t>(length_bits.GetIntegerValue() / 8);
	size_t decryption_key_length = std::min(length_bytes, decrypted_key.size());
	return BufferPtr(decrypted_key.begin(), decrypted_key.begin() + decryption_key_length);

#else
	(void) enveloped_data; (void) length_bits; (void) algorithm; (void) key;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::DecryptEnvelopedData(const syntax::ArrayObject<syntax::StringObjectPtr>& enveloped_data, const IEncryptionKey& key) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	auto length = enveloped_data.Size();
	for (decltype(length) i = 0; i < length; ++i) {
		auto enveloped_bytes = enveloped_data.At(i);

		auto enveloped_bytes_data = enveloped_bytes->GetValue()->data();
		auto enveloped_bytes_size = enveloped_bytes->GetValue()->size();

		BIO* enveloped_bytes_bio = BIO_new_mem_buf(enveloped_bytes_data, ValueConvertUtils::SafeConvert<int>(enveloped_bytes_size));
		SCOPE_GUARD_CAPTURE_REFERENCES(BIO_free(enveloped_bytes_bio));
		if (nullptr == enveloped_bytes_bio) {
			LOG_ERROR_GLOBAL << "Could not create BIO structure from enveloped data";
			continue;
		}

		PKCS7* p7 = d2i_PKCS7_bio(enveloped_bytes_bio, nullptr);
		SCOPE_GUARD_CAPTURE_REFERENCES(PKCS7_free(p7));
		if (nullptr == p7) {
			LOG_ERROR_GLOBAL << "Could not parse PKCS#7 structure from enveloped data";
			continue;
		}

		assert(PKCS7_type_is_enveloped(p7) && "PKCS#7 container is not enveloped");
		if (!PKCS7_type_is_enveloped(p7)) {
			LOG_ERROR_GLOBAL << "PKCS#7 container is not enveloped";
			continue;
		}

		PKCS7_ENVELOPE* envelope = p7->d.enveloped;

		auto pkcs7_recipient_count = sk_PKCS7_RECIP_INFO_num(envelope->recipientinfo);
		for (decltype(pkcs7_recipient_count) j = 0; j < pkcs7_recipient_count; ++j) {
			PKCS7_RECIP_INFO* pkcs7_recipient = sk_PKCS7_RECIP_INFO_value(envelope->recipientinfo, j);

			X509_NAME *issuer = pkcs7_recipient->issuer_and_serial->issuer;
			ASN1_INTEGER *serial = pkcs7_recipient->issuer_and_serial->serial;

			Buffer issuer_buffer(issuer->bytes->data, issuer->bytes->length);
			Buffer serial_buffer(serial->data, serial->length);
			if (!key.ContainsPrivateKey(issuer_buffer, serial_buffer)) {
				continue;
			}

			Buffer encrypted_recipient_key(pkcs7_recipient->enc_key->data, pkcs7_recipient->enc_key->length);
			Buffer decrypted_recipient_key = key.Decrypt(encrypted_recipient_key);

			BIO* etmp = BIO_new(BIO_f_cipher());
			SCOPE_GUARD_CAPTURE_REFERENCES(BIO_free(etmp));

			const EVP_CIPHER *evp_cipher = EVP_get_cipherbyobj(envelope->enc_data->algorithm->algorithm);
			assert(nullptr != evp_cipher && "EVP cipher is null");

			int rv;
			EVP_CIPHER_CTX *evp_ctx = nullptr;
			rv = BIO_get_cipher_ctx(etmp, &evp_ctx);
			if (1 != rv || nullptr == evp_ctx) {
				LOG_ERROR_GLOBAL << "Could not obtain cipher context";
				continue;
			}

			rv = EVP_CipherInit_ex(evp_ctx, evp_cipher, NULL, NULL, NULL, 0);
			if (1 != rv) {
				LOG_ERROR_GLOBAL << "Could not initialize cipher";
				continue;
			}

			rv = EVP_CIPHER_asn1_to_param(evp_ctx, envelope->enc_data->algorithm->parameter);
			if (rv <= 0) {
				LOG_ERROR_GLOBAL << "Could not convert asn1 to parameter";
				continue;
			}

			//if (decrypted_recipient_key->size() != EVP_CIPHER_CTX_key_length(evp_ctx)) {
			//	/*
			//	* Some S/MIME clients don't use the same key and effective key
			//	* length. The key length is determined by the size of the
			//	* decrypted RSA key.
			//	*/
			//	EVP_CIPHER_CTX_set_key_length(evp_ctx, decrypted_recipient_key->size());
			//}

			rv = EVP_CipherInit_ex(evp_ctx, NULL, NULL, (unsigned char *) decrypted_recipient_key.data(), NULL, 0);
			if (1 != rv) {
				LOG_ERROR_GLOBAL << "Could not decrypt enveloped data";
				continue;
			}

			auto out = BIO_new_mem_buf(envelope->enc_data->enc_data->data, envelope->enc_data->enc_data->length);
			SCOPE_GUARD_CAPTURE_REFERENCES(BIO_free(out));
			if (nullptr == out) {
				LOG_ERROR_GLOBAL << "Could not create BIO structure for decrypted data";
				continue;
			}

			BIO_push(etmp, out);

			BufferPtr decrypted_data;
			Buffer copy_buffer(constant::BUFFER_SIZE);
			for (;;) {
				void* data = copy_buffer.data();
				int data_length = ValueConvertUtils::SafeConvert<int>(copy_buffer.size());

				int result = BIO_read(etmp, data, data_length);
				if (result <= 0) {
					break;
				}

				decrypted_data->insert(decrypted_data.end(), copy_buffer.begin(), copy_buffer.begin() + result);
			}

			return decrypted_data;
		}
	}

	throw GeneralException("Could not find matching certificate");

#else
	(void) enveloped_data; (void) key;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::ComputeEncryptedOwnerData(const Buffer& pad_password, const syntax::DictionaryObject& encryption_dictionary) {

#if defined(GOTCHANG_PDF_HAVE_OPENSSL)

	auto user_value = encryption_dictionary.FindAs<syntax::StringObjectPtr>(constant::Name::U);
	auto owner_value = encryption_dictionary.FindAs<syntax::StringObjectPtr>(constant::Name::O);
	auto permissions = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::P);
	auto revision = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::R);
	auto version = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::V);

	syntax::IntegerObjectPtr length_bits = 40;
	if (encryption_dictionary.Contains(constant::Name::Length)) {
		length_bits = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::Length);
		assert(length_bits->GetIntegerValue() % 8 == 0 && "Key length is not multiplier of 8");
	}

	// check owner key
	Buffer password_digest(MD5_DIGEST_LENGTH);
	MD5((unsigned char*) pad_password.data(), pad_password.size(), (unsigned char*) password_digest.data());

	BufferPtr encrypted_owner_data;
	if (*revision >= 3) {
		MD5_CTX ctx;
		Buffer temporary_digest(MD5_DIGEST_LENGTH);

		auto length_bytes = ValueConvertUtils::SafeConvert<size_t>(length_bits->GetIntegerValue() / 8);
		size_t password_length = std::min(length_bytes, password_digest.size());
		for (int i = 0; i < 50; ++i) {
			MD5_Init(&ctx);
			MD5_Update(&ctx, password_digest.data(), password_length);
			MD5_Final((unsigned char*) temporary_digest.data(), &ctx);
			std::copy_n(temporary_digest.begin(), password_length, password_digest.begin());
		}

		auto key = BufferPtr(length_bytes);
		encrypted_owner_data = BufferPtr(*owner_value->GetValue());

		for (Buffer::value_type i = 0; i < 20; ++i) {
			for (decltype(password_length) j = 0; j < password_length; ++j) {
				key[j] = (password_digest[j] ^ i);
			}

			encrypted_owner_data = EncryptionUtils::ComputeRC4(key, encrypted_owner_data);
		}
	} else {
		encrypted_owner_data = EncryptionUtils::ComputeRC4(password_digest, 5, owner_value->GetValue());
	}

	return encrypted_owner_data;

#else
	(void) pad_password; (void) encryption_dictionary;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

} // syntax
} // gotchangpdf
