#include "precompiled.h"
#include "encryption.h"

#include "buffer.h"

#include <openssl/rc4.h>
#include <openssl/aes.h>
#include <openssl/x509.h>
#include <openssl/md5.h>
#include <openssl/pkcs7.h>
#include <openssl/objects.h>
#include <openssl/evp.h>

namespace gotchangpdf
{
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

	BufferPtr EncryptionUtils::PadTruncatePassword(const Buffer& password)
	{
		BufferPtr result(sizeof(HARDCODED_PFD_PAD));
		//result->reserve(sizeof(pad));
		std::copy_n(password.begin(), std::min(password.size(), sizeof(HARDCODED_PFD_PAD)), result.begin());
		std::copy_n(std::begin(HARDCODED_PFD_PAD), sizeof(HARDCODED_PFD_PAD) - password.size(), result.begin() + password.size());
		return result;
	}

	BufferPtr EncryptionUtils::ComputeRC4(const Buffer& key, int key_length, const Buffer& data)
	{
		BufferPtr result(data.size());

		RC4_KEY rc_key;
		RC4_set_key(&rc_key, key_length, (unsigned char*)key.data());
		RC4(&rc_key, data.size(), (unsigned char*)data.data(), (unsigned char*)result->data());

		return result;
	}

	BufferPtr EncryptionUtils::ComputeRC4(const Buffer& key, const Buffer& data)
	{
		return ComputeRC4(key, key.size(), data);
	}

	BufferPtr EncryptionUtils::AESDecrypt(const Buffer& key, const Buffer& data)
	{
		return AESDecrypt(key, key.size(), data);
	}

	BufferPtr EncryptionUtils::AESDecrypt(const Buffer& key, int key_length, const Buffer& data)
	{
		assert(data.size() >= static_cast<size_t>(key_length));
		if (data.size() < static_cast<size_t>(key_length)) {
			throw GeneralException("Cannot find IV for encrypted data");
		}

		Buffer iv(data.begin(), data.begin() + key_length);
		BufferPtr result(data.size() - key_length);

		AES_KEY dec_key;
		AES_set_decrypt_key((unsigned char*)key.data(), key_length * 8, &dec_key);
		AES_cbc_encrypt((unsigned char*)data.data() + key_length, (unsigned char*)result->data(), data.size() - key_length, &dec_key, (unsigned char*)iv.data(), AES_DECRYPT);

		return result;
	}

	bool EncryptionUtils::CheckKey(
		const Buffer& input,
		const Buffer& document_id,
		const Buffer& owner_data,
		const Buffer& user_data,
		const syntax::IntegerObject& permissions,
		const syntax::IntegerObject& revision,
		const syntax::IntegerObject& key_length,
		Buffer& decryption_key)
	{
		Buffer decryption_key_digest(MD5_DIGEST_LENGTH);

		MD5_CTX ctx;
		MD5_Init(&ctx);
		MD5_Update(&ctx, input.data(), input.size());
		MD5_Update(&ctx, owner_data.data(), owner_data.size());

		auto permissions_value = permissions.Value();
		uint32_t permissions_raw = reinterpret_cast<uint32_t&>(permissions_value);
		uint8_t permissions_array[sizeof(permissions_raw)];
		permissions_array[0] = permissions_raw & 0xFF;
		permissions_array[1] = (permissions_raw >> 8) & 0xFF;
		permissions_array[2] = (permissions_raw >> 16) & 0xFF;
		permissions_array[3] = (permissions_raw >> 24) & 0xFF;

		MD5_Update(&ctx, permissions_array, sizeof(permissions_array));
		MD5_Update(&ctx, document_id.data(), document_id.size());
		MD5_Final((unsigned char*)decryption_key_digest.data(), &ctx);

		auto length_bytes = SafeConvert<size_t>(key_length.Value() / 8);
		size_t decryption_key_length = std::min(length_bytes, decryption_key_digest.size());

		BufferPtr compare_data;
		if (revision >= 3) {
			Buffer temporary_digest(MD5_DIGEST_LENGTH);
			for (int i = 0; i < 50; ++i) {
				MD5_Init(&ctx);
				MD5_Update(&ctx, decryption_key_digest.data(), decryption_key_length);
				MD5_Final((unsigned char*)temporary_digest.data(), &ctx);
				std::copy_n(temporary_digest.begin(), decryption_key_length, decryption_key_digest.begin());
			}

			Buffer hardcoded_pad(&HARDCODED_PFD_PAD[0], HARDCODED_PFD_PAD_LENGTH);
			Buffer key_digest(MD5_DIGEST_LENGTH);

			MD5_Init(&ctx);
			MD5_Update(&ctx, hardcoded_pad.data(), hardcoded_pad.size());
			MD5_Update(&ctx, document_id.data(), document_id.size());
			MD5_Final((unsigned char*)key_digest.data(), &ctx);

			auto key = BufferPtr(length_bytes);
			compare_data = BufferPtr(key_digest);

			for (Buffer::value_type i = 0; i < 20; ++i) {
				for (decltype(decryption_key_length) j = 0; j < decryption_key_length; ++j) {
					key[j] = (decryption_key_digest[j] ^ i);
				}

				compare_data = EncryptionUtils::ComputeRC4(key, compare_data);
			}
		}
		else {
			assert(key_length.Value() == 40 && "Key length is not 5 bytes for revision <= 3");
			Buffer hardcoded_pad(&HARDCODED_PFD_PAD[0], HARDCODED_PFD_PAD_LENGTH);
			compare_data = EncryptionUtils::ComputeRC4(decryption_key_digest, 5, hardcoded_pad);
		}

		int compare_length = (revision >= 3 ? 16 : 32);
		if (std::equal(compare_data.begin(), compare_data.begin() + compare_length, user_data.begin())) {
			decryption_key = BufferPtr(decryption_key_digest.begin(), decryption_key_digest.begin() + decryption_key_length);
			return true;
		}

		return false;
	}

	BufferPtr EncryptionUtils::DecryptRecipientKey(const syntax::ArrayObject<syntax::StringObjectPtr>& recipients, const IEncryptionKey& key)
	{
		Buffer decrypted_data;
		auto length = recipients.Size();
		for (decltype(length) i = 0; i < length; ++i) {
			auto recipient_bytes = recipients.At(i);

			BIO* bio = BIO_new_mem_buf(recipient_bytes->Value()->data(), recipient_bytes->Value()->size());
			PKCS7* p7 = d2i_PKCS7_bio(bio, nullptr);

			assert(PKCS7_type_is_enveloped(p7) && "PKCS#7 container is enveloped");
			if (!PKCS7_type_is_enveloped(p7)) {
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
				if (!key.Equals(issuer_buffer, serial_buffer)) {
					continue;
				}

				Buffer encrypted_recipient_key(pkcs7_recipient->enc_key->data, pkcs7_recipient->enc_key->length);
				BufferPtr decrypted_recipient_key = key.Decrypt(encrypted_recipient_key);

				EVP_CIPHER_CTX *evp_ctx = NULL;
				const EVP_CIPHER *evp_cipher = EVP_get_cipherbyobj(envelope->enc_data->algorithm->algorithm);
				BIO* etmp = BIO_new(BIO_f_cipher());
				BIO_get_cipher_ctx(etmp, &evp_ctx);
				EVP_CipherInit_ex(evp_ctx, evp_cipher, NULL, NULL, NULL, 0);
				EVP_CIPHER_asn1_to_param(evp_ctx, envelope->enc_data->algorithm->parameter);

				//if (decrypted_recipient_key->size() != EVP_CIPHER_CTX_key_length(evp_ctx)) {
				//	/*
				//	* Some S/MIME clients don't use the same key and effective key
				//	* length. The key length is determined by the size of the
				//	* decrypted RSA key.
				//	*/
				//	EVP_CIPHER_CTX_set_key_length(evp_ctx, decrypted_recipient_key->size());
				//}

				EVP_CipherInit_ex(evp_ctx, NULL, NULL, (unsigned char *)decrypted_recipient_key->data(), NULL, 0);

				BUF_MEM *bptr;
				BIO_get_mem_ptr(etmp, &bptr);
				return BufferPtr(bptr->data, bptr->length);
			}
		}

		throw GeneralException("Could not find matching certificate");
	}

	BufferPtr EncryptionUtils::ComputeEncryptedOwnerData(const Buffer& pad_password, const syntax::DictionaryObject& encryption_dictionary)
	{
		auto user_value = encryption_dictionary.FindAs<syntax::StringObjectPtr>(constant::Name::U);
		auto owner_value = encryption_dictionary.FindAs<syntax::StringObjectPtr>(constant::Name::O);
		auto permissions = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::P);
		auto revision = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::R);
		auto version = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::V);

		syntax::IntegerObjectPtr length_bits = 40;
		if (encryption_dictionary.Contains(constant::Name::Length)) {
			length_bits = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::Length);
			assert(length_bits->Value() % 8 == 0 && "Key length is not multiplier of 8");
		}

		// check owner key
		Buffer password_digest(MD5_DIGEST_LENGTH);
		MD5((unsigned char*)pad_password.data(), pad_password.size(), (unsigned char*)password_digest.data());

		BufferPtr encrypted_owner_data;
		if (*revision >= 3) {
			MD5_CTX ctx;
			Buffer temporary_digest(MD5_DIGEST_LENGTH);

			auto length_bytes = SafeConvert<size_t>(length_bits->Value() / 8);
			size_t password_length = std::min(length_bytes, password_digest.size());
			for (int i = 0; i < 50; ++i) {
				MD5_Init(&ctx);
				MD5_Update(&ctx, password_digest.data(), password_length);
				MD5_Final((unsigned char*)temporary_digest.data(), &ctx);
				std::copy_n(temporary_digest.begin(), password_length, password_digest.begin());
			}

			auto key = BufferPtr(length_bytes);
			encrypted_owner_data = BufferPtr(*owner_value->Value());

			for (Buffer::value_type i = 0; i < 20; ++i) {
				for (decltype(password_length) j = 0; j < password_length; ++j) {
					key[j] = (password_digest[j] ^ i);
				}

				encrypted_owner_data = EncryptionUtils::ComputeRC4(key, encrypted_owner_data);
			}
		}
		else {
			encrypted_owner_data = EncryptionUtils::ComputeRC4(password_digest, 5, owner_value->Value());
		}

		return encrypted_owner_data;
	}
}
