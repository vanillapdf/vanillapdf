#include "precompiled.h"
#include "encryption.h"

#include "buffer.h"

#include <openssl/rc4.h>
#include <openssl/aes.h>
#include <openssl/x509.h>
#include <openssl/md5.h>

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
}
