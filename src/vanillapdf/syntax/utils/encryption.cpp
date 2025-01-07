#include "precompiled.h"

#include "utils/misc_utils.h"
#include "utils/math_utils.h"

#include "syntax/utils/encryption.h"
#include "syntax/utils/name_constants.h"

#if defined(VANILLAPDF_HAVE_OPENSSL)

#include <openssl/rc4.h>
#include <openssl/aes.h>
#include <openssl/x509.h>
#include <openssl/md5.h>
#include <openssl/pkcs7.h>
#include <openssl/objects.h>
#include <openssl/evp.h>

// For some interesting reason, openssl pulls in windows.h header
// which defines many unnecessary macros
#include <openssl/rand.h>
#undef max
#undef min
#undef X509_NAME
//-----------------------------

#endif

namespace vanillapdf {
namespace syntax {

	// Hard-coded in PDF specification
const uint8_t HARDCODED_PDF_PAD[] = {
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

const types::size_type AES_CBC_IV_LENGTH = 16;
const types::size_type AES_CBC_BLOCK_SIZE = 16;

BufferPtr EncryptionUtils::ComputeObjectKey(
	const Buffer& key,
	types::big_uint objNumber,
	types::ushort genNumber,
	EncryptionAlgorithm alg) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	BufferPtr object_key = make_deferred_container<Buffer>(MD5_DIGEST_LENGTH);

	uint8_t object_info[5];
	object_info[0] = objNumber & 0xFF;
	object_info[1] = (objNumber >> 8) & 0xFF;
	object_info[2] = (objNumber >> 16) & 0xFF;
	object_info[3] = (genNumber) & 0xFF;
	object_info[4] = (genNumber >> 8) & 0xFF;

	auto evp_md = EVP_md5();
	auto evp_md_ctx = EVP_MD_CTX_new();

	if (evp_md_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_MD_CTX");
	}

	SCOPE_GUARD([evp_md_ctx]() { EVP_MD_CTX_free(evp_md_ctx); });

	auto init_result = EVP_DigestInit(evp_md_ctx, evp_md);
	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize MD5 digest: {}", openssl_error);
	}

	auto update_key_result = EVP_DigestUpdate(evp_md_ctx, key.data(), key.std_size());
	if (update_key_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	auto update_object_result = EVP_DigestUpdate(evp_md_ctx, object_info, sizeof(object_info));
	if (update_object_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	if (alg == EncryptionAlgorithm::AES) {
		auto update_salt_result = EVP_DigestUpdate(evp_md_ctx, &AES_ADDITIONAL_SALT[0], sizeof(AES_ADDITIONAL_SALT));
		if (update_salt_result != 1) {
			auto openssl_error = MiscUtils::GetLastOpensslError();
			LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
		}
	}

	unsigned int final_size = 0;
	auto final_result = EVP_DigestFinal(evp_md_ctx, (unsigned char*)object_key->data(), &final_size);
	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not finalize MD5 digest: {}", openssl_error);
	}

	// Confirm the actual output matches  the digest
	assert(final_size == MD5_DIGEST_LENGTH);

	// 7.6.2 General Encryption Algorithm
	// Algorithm 1: Encryption of data using the RC4 or AES algorithms
	// Use the first (n + 5) bytes, up to a maximum of 16,
	// of the output from the MD5 hash as the key for the RC4 or AES symmetric key algorithms,
	// along with the string or stream data to be encrypted.
	types::size_type key_length = std::min<types::size_type>(key.size() + 5, 16);
	return make_deferred_container<Buffer>(object_key.begin(), object_key.begin() + key_length);

#else
	(void) key; (void) objNumber; (void) genNumber; (void) alg;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::PadTruncatePassword(const Buffer& password) {
	BufferPtr result = make_deferred_container<Buffer>(sizeof(HARDCODED_PDF_PAD));
	//result->reserve(sizeof(pad));
	std::copy_n(password.begin(), std::min<types::size_type>(password.size(), sizeof(HARDCODED_PDF_PAD)), result.begin());
	std::copy_n(std::begin(HARDCODED_PDF_PAD), sizeof(HARDCODED_PDF_PAD) - password.size(), result.begin() + password.size());
	return result;
}

BufferPtr EncryptionUtils::ComputeRC4(const Buffer& key, const Buffer& data) {
	return ComputeRC4(key, key.size(), data);
}

BufferPtr EncryptionUtils::ComputeRC4(const Buffer& key, types::size_type key_length, const Buffer& data) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	BufferPtr result = make_deferred_container<Buffer>(data.size());

	const EVP_CIPHER* evp_cipher = EVP_rc4();
	
	auto evp_cipher_ctx = EVP_CIPHER_CTX_new();
	if (evp_cipher_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_CIPHER_CTX");
	}
	
	SCOPE_GUARD([evp_cipher_ctx]() { EVP_CIPHER_CTX_free(evp_cipher_ctx); });

	// This is a trick required by the OpenSSL 3.0 to call EVP_EncryptInit two times

	// The issue is that EVP_rc4 is a variable length and the parameter for the key length cannot be set directly
	// https://www.openssl.org/docs/manmaster/man3/EVP_rc4.html
	// RC4 stream cipher. This is a variable key length cipher with a default key length of 128 bits.

	// In the first call of EVP_EncryptInit leave key empty to just initialize the engine.
	// In the second call of EVP_EncryptInit leave the cipher empty and set only the key.
	// https://github.com/openssl/openssl/issues/22629
	
	auto init_result = EVP_EncryptInit(evp_cipher_ctx, evp_cipher, nullptr, nullptr);
	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize RC4 digest: {}", openssl_error);
	}

	// Key length is set in bytes and not bits, found this experimentally
	// auto test = EVP_CIPHER_CTX_get_key_length(evp_cipher_ctx);

	auto key_length_converted = ValueConvertUtils::SafeConvert<int>(key_length);
	auto set_key_length_result = EVP_CIPHER_CTX_set_key_length(evp_cipher_ctx, key_length_converted);
	if (set_key_length_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not set RC4 key length: {}", openssl_error);
	}

	auto init_result2 = EVP_EncryptInit(evp_cipher_ctx, nullptr, (unsigned char*)key.data(), nullptr);
	if (init_result2 != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not re-initialize RC4 digest: {}", openssl_error);
	}
	
	int current_offset = 0;
	int total_result_length = 0;

	int data_size = ValueConvertUtils::SafeConvert<int>(data.size());
	
	auto update_result = EVP_EncryptUpdate(
		evp_cipher_ctx,
		(unsigned char*) result->data(),
		&current_offset,
		(unsigned char*) data.data(),
		data_size);
	
	if (update_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update RC4 digest: {}", openssl_error);
	}

	total_result_length += current_offset;
	
	auto final_result = EVP_EncryptFinal(evp_cipher_ctx, (unsigned char*)result->data() + current_offset, &current_offset);
	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not finalize RC4 digest: {}", openssl_error);
	}

	total_result_length += current_offset;

	// Remove trailing zeroes
	result->resize(total_result_length);

	return result;

#else
	(void) key; (void) key_length; (void) data;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::ComputeMD5(const Buffer& data) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	auto evp_md = EVP_md5();
	auto evp_md_ctx = EVP_MD_CTX_new();
	if (evp_md_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_MD_CTX");
	}

	SCOPE_GUARD([evp_md_ctx]() { EVP_MD_CTX_free(evp_md_ctx); });

	BufferPtr key_digest = make_deferred_container<Buffer>(MD5_DIGEST_LENGTH);

	auto init_result = EVP_DigestInit(evp_md_ctx, evp_md);
	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize MD5 digest: {}", openssl_error);
	}

	auto update_document_id_result = EVP_DigestUpdate(evp_md_ctx, data.data(), data.std_size());
	if (update_document_id_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	unsigned int final_size = 0;
	auto final_result = EVP_DigestFinal(evp_md_ctx, (unsigned char*)key_digest->data(), &final_size);
	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not finalize MD5 digest: {}", openssl_error);
	}

	// Confirm the actual output matches  the digest
	assert(final_size == MD5_DIGEST_LENGTH);

	return key_digest;

#else
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::AESDecrypt(const Buffer& key, const Buffer& data) {
	return AESDecrypt(key, key.size(), data);
}

BufferPtr EncryptionUtils::AESDecrypt(const Buffer& key, types::size_type key_length, const Buffer& data) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	assert(data.size() >= static_cast<size_t>(AES_CBC_IV_LENGTH));
	if (data.size() < static_cast<size_t>(AES_CBC_IV_LENGTH)) {
		LOG_ERROR_AND_THROW_GENERAL("Cannot find IV for encrypted data");
	}

	// There are not data beyond the IV, return immediately
	// It currently works as a HACK for document ID hexadecimal strings
	// There are some leftover object streams after save and the
	// encryption exemption is not working properly
	if (data.size() == AES_CBC_IV_LENGTH) {
		return BufferPtr();
	}

	Buffer iv(data.begin(), data.begin() + AES_CBC_IV_LENGTH);
	BufferPtr result = make_deferred_container<Buffer>(data.size() - AES_CBC_IV_LENGTH);

	const EVP_CIPHER* evp_cipher = nullptr;

	if (key_length == 16) {
		evp_cipher = EVP_aes_128_cbc();
	}

	if (key_length == 32) {
		evp_cipher = EVP_aes_256_cbc();
	}

	if (evp_cipher == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Unknown AES key length: " + std::to_string(key_length));
	}

	auto evp_cipher_ctx = EVP_CIPHER_CTX_new();
	if (evp_cipher_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_CIPHER_CTX");
	}

	SCOPE_GUARD([evp_cipher_ctx]() { EVP_CIPHER_CTX_free(evp_cipher_ctx); });
	
	auto init_result = EVP_DecryptInit(
		evp_cipher_ctx,
		evp_cipher,
		reinterpret_cast<const unsigned char*>(key.data()),
		reinterpret_cast<const unsigned char*>(iv.data()));

	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize AES digest: {}", openssl_error);
	}

	// OpenSSL uses padding by default and sometimes it is not correct
	// We do handle the padding after the decryption has been done
	auto padding_result = EVP_CIPHER_CTX_set_padding(evp_cipher_ctx, 0);
	if (padding_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not set AES decryption padding: {}", openssl_error);
	}
	
	int current_offset = 0;
	int total_result_length = 0;

	int data_size = ValueConvertUtils::SafeConvert<int>(data.size() - AES_CBC_IV_LENGTH);
	
	auto update_result = EVP_DecryptUpdate(
		evp_cipher_ctx,
		reinterpret_cast<unsigned char*>(result->data()),
		&current_offset,
		reinterpret_cast<const unsigned char*>(data.data() + AES_CBC_IV_LENGTH),
		data_size);
	
	if (update_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not decrypt update AES cipher: {}", openssl_error);
	}

	total_result_length += current_offset;
	
	auto final_result = EVP_DecryptFinal(
		evp_cipher_ctx,
		reinterpret_cast<unsigned char*>(result->data() + current_offset),
		&current_offset);

	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not decrypt finalize AES cipher: {}", openssl_error);
	}

	total_result_length += current_offset;

	// Remove trailing zeroes
	result->resize(total_result_length);

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

#if defined(VANILLAPDF_HAVE_OPENSSL)

	if (data.size() == 0) {
		return BufferPtr();
	}

	Buffer iv(AES_CBC_IV_LENGTH);

	auto rand_result = RAND_bytes((unsigned char*) iv.data(), AES_CBC_IV_LENGTH);
	if (rand_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not generate initialization vector for AES: {}", openssl_error);
	}

	BufferPtr data_padded = AddPkcs7Padding(data, AES_CBC_BLOCK_SIZE);
	BufferPtr result = make_deferred_container<Buffer>(data_padded->size() + AES_CBC_IV_LENGTH);

	const EVP_CIPHER* evp_cipher = nullptr;

	if (key_length == 16) {
		evp_cipher = EVP_aes_128_cbc();
	}

	if (key_length == 32) {
		evp_cipher = EVP_aes_256_cbc();
	}

	if (evp_cipher == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Unknown AES key length: " + std::to_string(key_length));
	}

	auto evp_cipher_ctx = EVP_CIPHER_CTX_new();
	if (evp_cipher_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_CIPHER_CTX");
	}

	SCOPE_GUARD([evp_cipher_ctx]() { EVP_CIPHER_CTX_free(evp_cipher_ctx); });

	auto init_result = EVP_EncryptInit(
		evp_cipher_ctx,
		evp_cipher,
		reinterpret_cast<const unsigned char*>(key.data()),
		reinterpret_cast<const unsigned char*>(iv.data()));

	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize AES cipher: {}", openssl_error);
	}

	int current_offset = 0;
	int total_result_length = 0;

	int data_size = ValueConvertUtils::SafeConvert<int>(data_padded->size());

	auto update_result = EVP_EncryptUpdate(
		evp_cipher_ctx,
		reinterpret_cast<unsigned char*>(result->data()),
		&current_offset,
		reinterpret_cast<const unsigned char*>(data_padded->data()),
		data_size);

	if (update_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not encrypt update AES cipher: {}", openssl_error);
	}

	total_result_length += current_offset;

	auto final_result = EVP_EncryptFinal(
		evp_cipher_ctx,
		reinterpret_cast<unsigned char*>(result->data() + current_offset),
		&current_offset);

	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not encrypt finalize AES cipher: {}", openssl_error);
	}

	total_result_length += current_offset;

	// Remove trailing zeroes
	result->resize(total_result_length);

	// Insert the IV data in in the beginning
	result->insert(result->begin(), iv.begin(), iv.end());

	return result;

#else
	(void) key; (void) key_length; (void) data;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::AddPkcs7Padding(const Buffer& data, types::size_type block_size) {
	types::size_type remaining = data.size() % block_size;
	if (0 == remaining) {
		remaining = block_size;
	}

	Buffer::value_type converted = ValueConvertUtils::SafeConvert<Buffer::value_type>(remaining);

	BufferPtr result;
	result->reserve(data.size() + remaining);
	result->insert(result.begin(), data.begin(), data.end());
	result->insert(result.end(), remaining, converted);

	return result;
}

BufferPtr EncryptionUtils::RemovePkcs7Padding(const Buffer& data, types::size_type block_size) {
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

		return make_deferred_container<Buffer>(data.begin(), data.end() - converted);
	} while (false);

	// I would really like to be strict about padding,
	// but in test document "example_128-aes.pdf" there is a signature field
	// that allocated space with zeroes and did not care about setting proper
	// pkcs padding. That means, that there is some trash after we decrypt the
	// signature contents and padding could not be validated
	spdlog::warn("Pkcs padding is incorrect");
	return make_deferred_container<Buffer>(data);
}

BufferPtr EncryptionUtils::GenerateOwnerEncryptionKey(
	const Buffer& document_id,
	const Buffer& owner_password,
	const Buffer& user_password,
	EncryptionAlgorithm algorithm,
	int32_t key_length,
	uint32_t permissions,
	int32_t revision) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	// These parameters are not used, however I prefer to keep the interface similar to GenerateUserEncryptionKey
	(void)document_id; (void)permissions;

	// Initialize algorithms is required for any calculations
	MiscUtils::InitializeOpenSSL();

	if (algorithm == EncryptionAlgorithm::Undefined) {
		throw GeneralException("Could not generate encryption key for Undefined encryption algorithm");
	}

	if (algorithm == EncryptionAlgorithm::None) {
		throw GeneralException("Could not generate encryption key for None encryption algorithm");
	}

	if (algorithm == EncryptionAlgorithm::RC4 ||
		algorithm == EncryptionAlgorithm::AES) {

		// Pad or truncate the owner password string as described in step (a) of "Algorithm 2: Computing an encryption key".
		// If there is no owner password, use the user password instead.
		BufferPtr pad_owner_password = EncryptionUtils::PadTruncatePassword(owner_password);

		// Initialize the MD5 hash function and pass the result of step (a) as input to this function.
		BufferPtr stepb = ComputeMD5(pad_owner_password);

		// (Security handlers of revision 3 or greater) Do the following 50 times:
		// Take the output from the previous MD5 hash and pass it as input into a new MD5 hash.
		BufferPtr stepc = stepb;

		if (revision >= 3) {
			for (int i = 0; i < 50; ++i) {
				stepc = ComputeMD5(stepc);
			}
		}

		// Create an RC4 encryption key using the first n bytes of the output from the final MD5 hash,
		// where n shall always be 5 for security handlers of revision 2 but, for security handlers of revision 3 or greater,
		// shall depend on the value of the encryption dictionary's Length entry.
		decltype(key_length) rc4_key_length = 5;
		if (revision >= 3) {
			rc4_key_length = (key_length / 8);
		}

		// Pad or truncate the user password string as described in step (a) of "Algorithm 2: Computing an encryption key".
		BufferPtr pad_user_password = EncryptionUtils::PadTruncatePassword(user_password);

		// Encrypt the result of step (e), using an RC4 encryption function with the encryption key obtained in step (d).
		auto rc4_key_length_converted = ValueConvertUtils::SafeConvert<types::size_type>(rc4_key_length);
		BufferPtr stepf = EncryptionUtils::ComputeRC4(stepc, rc4_key_length_converted, pad_user_password);

		// (Security handlers of revision 3 or greater) Do the following 19 times:
		// Take the output from the previous invocation of the RC4 function and pass it as input to a new invocation of the function;
		// use an encryption key generated by taking each byte of the encryption key obtained in step (d)
		// and performing an XOR (exclusive or) operation between that byte and the single-byte value of the iteration counter (from 1 to 19).
		BufferPtr stepg = stepf;

		if (revision >= 3) {
			for (int i = 1; i < 20; ++i) {
				BufferPtr key = make_deferred_container<Buffer>(stepc->size());
				for (int j = 0; j < stepc->size(); ++j) {
					key[j] = (stepc[j] ^ i) & 0xFF;
				}

				stepg = EncryptionUtils::ComputeRC4(key, key->size(), stepg);
			}
		}

		// Store the output from the final invocation of the RC4 function as the value of the O entry in the encryption dictionary.
		return stepg;
	}

	throw NotSupportedException("Unknown encryption algorithm: " + std::to_string(static_cast<int>(algorithm)));

#else
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif
}

BufferPtr EncryptionUtils::GenerateUserEncryptionKey(
	const Buffer& document_id,
	const Buffer& user_password,
	const Buffer& owner_data,
	EncryptionAlgorithm algorithm,
	int32_t key_length,
	int32_t permissions,
	int32_t revision) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	// Initialize algorithms is required for any calculations
	MiscUtils::InitializeOpenSSL();

	if (algorithm == EncryptionAlgorithm::Undefined) {
		throw GeneralException("Could not generate encryption key for Undefined encryption algorithm");
	}

	if (algorithm == EncryptionAlgorithm::None) {
		throw GeneralException("Could not generate encryption key for None encryption algorithm");
	}

	if (algorithm == EncryptionAlgorithm::RC4 ||
		algorithm == EncryptionAlgorithm::AES) {
		BufferPtr padPassword = EncryptionUtils::PadTruncatePassword(user_password);
		auto decryption_key_digest = CalculateDecryptionKeyDigest(padPassword, document_id, owner_data, permissions);

		if (revision >= 3) {
			return CalculateDecryptionCompareDataV3(decryption_key_digest, document_id, key_length);
		}

		Buffer hardcoded_pad(std::begin(HARDCODED_PDF_PAD), sizeof(HARDCODED_PDF_PAD));
		return EncryptionUtils::ComputeRC4(decryption_key_digest, 5, hardcoded_pad);
	}

	throw NotSupportedException("Unknown encryption algorithm: " + std::to_string(static_cast<int>(algorithm)));

#else
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif
}

bool EncryptionUtils::CheckKey(
	const Buffer& input,
	const Buffer& document_id,
	const Buffer& owner_data,
	const Buffer& user_data,
	uint32_t permissions,
	int32_t revision,
	int32_t key_length,
	Buffer& decryption_key) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	auto decryption_key_digest = CalculateDecryptionKeyDigest(input, document_id, owner_data, permissions);

	auto length_bytes = ValueConvertUtils::SafeConvert<size_t>(key_length / 8);
	size_t decryption_key_length = std::min(length_bytes, decryption_key_digest->std_size());

	BufferPtr compare_data;
	if (revision >= 3) {
		compare_data = CalculateDecryptionCompareDataV3(decryption_key_digest, document_id, key_length);
	} else {
		Buffer hardcoded_pad(std::begin(HARDCODED_PDF_PAD), sizeof(HARDCODED_PDF_PAD));
		compare_data = EncryptionUtils::ComputeRC4(decryption_key_digest, 5, hardcoded_pad);
	}

	int compare_length = (revision >= 3 ? 16 : 32);
	if (std::equal(compare_data.begin(), compare_data.begin() + compare_length, user_data.begin())) {
		decryption_key = make_deferred_container<Buffer>(decryption_key_digest.begin(), decryption_key_digest.begin() + decryption_key_length);
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

BufferPtr EncryptionUtils::CalculateDecryptionCompareDataV3(
	BufferPtr decryption_key_digest,
	const Buffer& document_id,
	int32_t key_length) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	auto evp_md = EVP_md5();
	auto evp_md_ctx = EVP_MD_CTX_new();
	if (evp_md_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_CIPHER_CTX");
	}

	SCOPE_GUARD([evp_md_ctx]() { EVP_MD_CTX_free(evp_md_ctx); });

	auto length_bytes = ValueConvertUtils::SafeConvert<size_t>(key_length / 8);
	size_t decryption_key_length = std::min(length_bytes, decryption_key_digest->std_size());

	Buffer temporary_digest(MD5_DIGEST_LENGTH);

	for (int i = 0; i < 50; ++i) {

		auto init_result = EVP_DigestInit(evp_md_ctx, evp_md);
		if (init_result != 1) {
			auto openssl_error = MiscUtils::GetLastOpensslError();
			LOG_ERROR_AND_THROW_GENERAL("Could not initialize MD5 cipher: {}", openssl_error);
		}

		auto update_result = EVP_DigestUpdate(evp_md_ctx, decryption_key_digest->data(), decryption_key_length);
		if (update_result != 1) {
			auto openssl_error = MiscUtils::GetLastOpensslError();
			LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 cipher: {}", openssl_error);
		}

		unsigned int final_size = 0;
		auto final_result = EVP_DigestFinal(evp_md_ctx, (unsigned char*)temporary_digest.data(), &final_size);
		if (final_result != 1) {
			auto openssl_error = MiscUtils::GetLastOpensslError();
			LOG_ERROR_AND_THROW_GENERAL("Could not finalize MD5 cipher: {}", openssl_error);
		}

		// Confirm the actual output matches  the digest
		assert(final_size == MD5_DIGEST_LENGTH);

		std::copy_n(temporary_digest.begin(), decryption_key_length, decryption_key_digest.begin());
	}

	Buffer key_digest(MD5_DIGEST_LENGTH);

	auto init_result = EVP_DigestInit(evp_md_ctx, evp_md);
	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize MD5 digest: {}", openssl_error);
	}

	auto update_pad_result = EVP_DigestUpdate(evp_md_ctx, HARDCODED_PDF_PAD, sizeof(HARDCODED_PDF_PAD));
	if (update_pad_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	auto update_document_id_result = EVP_DigestUpdate(evp_md_ctx, document_id.data(), document_id.std_size());
	if (update_document_id_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	unsigned int final_size = 0;
	auto final_result = EVP_DigestFinal(evp_md_ctx, (unsigned char*)key_digest.data(), &final_size);
	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not finalize MD5 digest: {}", openssl_error);
	}

	// Confirm the actual output matches  the digest
	assert(final_size == MD5_DIGEST_LENGTH);

	// Encrypt the 16-byte result of the hash,
	// using an RC4 encryption function with the encryption key from step (a).
	BufferPtr compare_data = EncryptionUtils::ComputeRC4(decryption_key_digest, key_digest);

	// Do the following 19 times: Take the output from the previous invocation of the RC4 function and pass it as
	// input to a new invocation of the function; use an encryption key generated by taking each byte of the
	// original encryption key obtained in step(a) and performing an XOR(exclusive or ) operation between that
	// byte and the single - byte value of the iteration counter(from 1 to 19).
	for (Buffer::value_type i = 1; i < 20; ++i) {
		BufferPtr key = make_deferred_container<Buffer>(decryption_key_digest->size());

		for (int j = 0; j < decryption_key_digest->size(); ++j) {
			key[j] = (decryption_key_digest[j] ^ i);
		}

		compare_data = EncryptionUtils::ComputeRC4(key, compare_data);
	}

	// Append 16 bytes of arbitrary padding to the output from the final invocation of the RC4 function and
	// store the 32-byte result as the value of the U entry in the encryption dictionary.
	BufferPtr arbitrary_padding = EncryptionUtils::GenerateRandomData(16);

	compare_data->insert(compare_data.end(), arbitrary_padding->begin(), arbitrary_padding->end());

	return compare_data;

#else
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif
}

BufferPtr EncryptionUtils::CalculateDecryptionKeyDigest(
	const Buffer& input,
	const Buffer& document_id,
	const Buffer& owner_data,
	uint32_t permissions) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	BufferPtr decryption_key_digest = make_deferred_container<Buffer>(MD5_DIGEST_LENGTH);

	auto evp_md = EVP_md5();
	auto evp_md_ctx = EVP_MD_CTX_new();
	if (evp_md_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_CIPHER_CTX");
	}

	SCOPE_GUARD([evp_md_ctx]() { EVP_MD_CTX_free(evp_md_ctx); });

	auto init_result = EVP_DigestInit(evp_md_ctx, evp_md);
	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize MD5 digest: {}", openssl_error);
	}

	auto update_input_result = EVP_DigestUpdate(evp_md_ctx, input.data(), input.std_size());
	if (update_input_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	auto update_owner_result = EVP_DigestUpdate(evp_md_ctx, owner_data.data(), owner_data.std_size());
	if (update_owner_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	uint8_t permissions_array[sizeof(permissions)];

	for (size_t i = 0; i < sizeof(permissions); ++i) {
		permissions_array[i] = (permissions >> (i * 8)) & 0xFF;
	}

	auto update_permissions_result = EVP_DigestUpdate(evp_md_ctx, permissions_array, sizeof(permissions_array));
	if (update_permissions_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	auto update_document_id_result = EVP_DigestUpdate(evp_md_ctx, document_id.data(), document_id.std_size());
	if (update_document_id_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	unsigned int final_size = 0;
	auto final_result = EVP_DigestFinal(evp_md_ctx, (unsigned char*)decryption_key_digest->data(), &final_size);
	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not finalize MD5 digest: {}", openssl_error);
	}

	return decryption_key_digest;

#else
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::GetRecipientKey
(const syntax::ArrayObject<syntax::StringObjectPtr>& enveloped_data,
	const syntax::IntegerObject& length_bits,
	EncryptionAlgorithm algorithm,
	IEncryptionKey& key) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	const EVP_MD* evp_md = nullptr;
	unsigned int expected_final_digest_size = 0;

	BufferPtr decrypted_key;
	if (length_bits == 256 && algorithm == EncryptionAlgorithm::AES) {
		evp_md = EVP_sha256();
		decrypted_key->resize(SHA256_DIGEST_LENGTH);
		expected_final_digest_size = SHA256_DIGEST_LENGTH;
	}
	else {
		evp_md = EVP_sha1();
		decrypted_key->resize(SHA_DIGEST_LENGTH);
		expected_final_digest_size = SHA_DIGEST_LENGTH;
	}

	auto evp_md_ctx = EVP_MD_CTX_new();
	if (evp_md_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_CIPHER_CTX");
	}

	SCOPE_GUARD([evp_md_ctx]() { EVP_MD_CTX_free(evp_md_ctx); });

	auto init_result = EVP_DigestInit(evp_md_ctx, evp_md);
	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize SHA digest: {}", openssl_error);
	}

	auto decrypted_data = EncryptionUtils::DecryptEnvelopedData(enveloped_data, key);
	auto update_key_result = EVP_DigestUpdate(evp_md_ctx, decrypted_data->data(), 20);
	if (update_key_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update SHA digest: {}", openssl_error);
	}

	auto length = enveloped_data.GetSize();
	for (decltype(length) i = 0; i < length; ++i) {
		auto enveloped_bytes = enveloped_data.GetValue(i);
		auto enveloped_buffer = enveloped_bytes->GetValue();

		auto update_enveloped_result = EVP_DigestUpdate(evp_md_ctx, enveloped_buffer->data(), enveloped_buffer->std_size());
		if (update_enveloped_result != 1) {
			auto openssl_error = MiscUtils::GetLastOpensslError();
			LOG_ERROR_AND_THROW_GENERAL("Could not update SHA digest: {}", openssl_error);
		}
	}

	unsigned int final_size = 0;
	auto final_result = EVP_DigestFinal(evp_md_ctx, (unsigned char*)decrypted_key->data(), &final_size);
	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not finalize SHA digest: {}", openssl_error);
	}

	// Confirm the actual output matches  the digest
	assert(final_size == expected_final_digest_size); (void)expected_final_digest_size;

	auto length_bytes = ValueConvertUtils::SafeConvert<Buffer::size_type>(length_bits.GetIntegerValue() / 8);

	if (length_bytes < decrypted_key->size()) {
		decrypted_key->resize(length_bytes);
	}

	return decrypted_key;

#else
	(void) enveloped_data; (void) length_bits; (void) algorithm; (void) key;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::DecryptEnvelopedData(const syntax::ArrayObject<syntax::StringObjectPtr>& enveloped_data, IEncryptionKey& key) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	auto length = enveloped_data.GetSize();
	for (decltype(length) i = 0; i < length; ++i) {
		auto enveloped_bytes = enveloped_data.GetValue(i);

		auto enveloped_bytes_data = enveloped_bytes->GetValue()->data();
		auto enveloped_bytes_size = enveloped_bytes->GetValue()->size();

		BIO* enveloped_bytes_bio = BIO_new_mem_buf(enveloped_bytes_data, ValueConvertUtils::SafeConvert<int>(enveloped_bytes_size));
		SCOPE_GUARD_CAPTURE_REFERENCES(BIO_free(enveloped_bytes_bio));
		if (nullptr == enveloped_bytes_bio) {
			spdlog::error("Could not create BIO structure from enveloped data");
			continue;
		}

		PKCS7* p7 = d2i_PKCS7_bio(enveloped_bytes_bio, nullptr);
		SCOPE_GUARD_CAPTURE_REFERENCES(PKCS7_free(p7));
		if (nullptr == p7) {
			spdlog::error("Could not parse PKCS#7 structure from enveloped data");
			continue;
		}

		assert(PKCS7_type_is_enveloped(p7) && "PKCS#7 container is not enveloped");
		if (!PKCS7_type_is_enveloped(p7)) {
			spdlog::error("PKCS#7 container is not enveloped");
			continue;
		}

		PKCS7_ENVELOPE* envelope = p7->d.enveloped;

		auto pkcs7_recipient_count = sk_PKCS7_RECIP_INFO_num(envelope->recipientinfo);
		for (decltype(pkcs7_recipient_count) j = 0; j < pkcs7_recipient_count; ++j) {
			PKCS7_RECIP_INFO* pkcs7_recipient = sk_PKCS7_RECIP_INFO_value(envelope->recipientinfo, j);

			X509_NAME *issuer = pkcs7_recipient->issuer_and_serial->issuer;
			ASN1_INTEGER *serial = pkcs7_recipient->issuer_and_serial->serial;

			// Convert issuer to null terminated string
			char* oneline = X509_NAME_oneline(issuer, nullptr, 0);
			SCOPE_GUARD([oneline]() {OPENSSL_free(oneline); });

			if (oneline == nullptr) {
				spdlog::error("Could not print issuer to buffer");
				continue;
			}

			Buffer issuer_buffer(oneline);
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
				spdlog::error("Could not obtain cipher context");
				continue;
			}

			rv = EVP_CipherInit_ex(evp_ctx, evp_cipher, NULL, NULL, NULL, 0);
			if (1 != rv) {
				spdlog::error("Could not initialize cipher");
				continue;
			}

			rv = EVP_CIPHER_asn1_to_param(evp_ctx, envelope->enc_data->algorithm->parameter);
			if (rv <= 0) {
				spdlog::error("Could not convert asn1 to parameter");
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
				spdlog::error("Could not decrypt enveloped data");
				continue;
			}

			auto out = BIO_new_mem_buf(envelope->enc_data->enc_data->data, envelope->enc_data->enc_data->length);
			SCOPE_GUARD_CAPTURE_REFERENCES(BIO_free(out));
			if (nullptr == out) {
				spdlog::error("Could not create BIO structure for decrypted data");
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

BufferPtr EncryptionUtils::ComputeAuthenticationOwnerData(const Buffer& pad_password, const syntax::DictionaryObject& encryption_dictionary) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	// Initialize algorithms is required for any calculations
	MiscUtils::InitializeOpenSSL();

	auto user_value = encryption_dictionary.FindAs<syntax::StringObjectPtr>(constant::Name::U);
	auto owner_value = encryption_dictionary.FindAs<syntax::StringObjectPtr>(constant::Name::O);
	auto permissions = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::P);
	auto revision = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::R);
	auto version = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::V);

	syntax::IntegerObjectPtr length_bits = make_deferred<IntegerObject>(40);
	if (encryption_dictionary.Contains(constant::Name::Length)) {
		length_bits = encryption_dictionary.FindAs<syntax::IntegerObjectPtr>(constant::Name::Length);
		assert(length_bits->GetIntegerValue() % 8 == 0 && "Key length is not multiplier of 8");
	}

	// check owner key
	Buffer password_digest(MD5_DIGEST_LENGTH);

	auto evp_md = EVP_md5();
	auto evp_md_ctx = EVP_MD_CTX_new();
	if (evp_md_ctx == nullptr) {
		LOG_ERROR_AND_THROW_GENERAL("Failed to allocate new EVP_CIPHER_CTX");
	}

	SCOPE_GUARD([evp_md_ctx]() { EVP_MD_CTX_free(evp_md_ctx); });

	auto init_result = EVP_DigestInit(evp_md_ctx, evp_md);
	if (init_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not initialize MD5 digest: {}", openssl_error);
	}

	auto update_password_result = EVP_DigestUpdate(evp_md_ctx, pad_password.data(), pad_password.std_size());
	if (update_password_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
	}

	unsigned int final_size = 0;
	auto final_result = EVP_DigestFinal(evp_md_ctx, (unsigned char*)password_digest.data(), &final_size);
	if (final_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not finalize MD5 digest: {}", openssl_error);
	}

	// Confirm the actual output matches  the digest
	assert(final_size == MD5_DIGEST_LENGTH);

	if (*revision >= 3) {

		Buffer temporary_digest(MD5_DIGEST_LENGTH);

		auto length_bytes = ValueConvertUtils::SafeConvert<size_t>(length_bits->GetIntegerValue() / 8);
		auto password_length = std::min(length_bytes, password_digest.std_size());

		for (int i = 0; i < 50; ++i) {

			auto init_loop_result = EVP_DigestInit(evp_md_ctx, evp_md);
			if (init_loop_result != 1) {
				auto openssl_error = MiscUtils::GetLastOpensslError();
				LOG_ERROR_AND_THROW_GENERAL("Could not initialize MD5 digest: {}", openssl_error);
			}

			auto update_loop_result = EVP_DigestUpdate(evp_md_ctx, password_digest.data(), password_length);
			if (update_loop_result != 1) {
				auto openssl_error = MiscUtils::GetLastOpensslError();
				LOG_ERROR_AND_THROW_GENERAL("Could not update MD5 digest: {}", openssl_error);
			}

			unsigned int final_loop_size = 0;
			auto final_loop_result = EVP_DigestFinal(evp_md_ctx, (unsigned char*)temporary_digest.data(), &final_loop_size);
			if (final_loop_result != 1) {
				auto openssl_error = MiscUtils::GetLastOpensslError();
				LOG_ERROR_AND_THROW_GENERAL("Could not finalize MD5 digest: {}", openssl_error);
			}

			// Confirm the actual output matches  the digest
			assert(final_loop_size == MD5_DIGEST_LENGTH);

			std::copy_n(temporary_digest.begin(), password_length, password_digest.begin());
		}

		// (Security handlers of revision 3 or greater) Do the following 20 times: Decrypt the value of the encryption
		// dictionary's O entry (first iteration) or the output from the previous iteration (all subsequent iterations),
		// using an RC4 encryption function with a different encryption key at each iteration. The key shall be
		// generated by taking the original key (obtained in step (a)) and performing an XOR (exclusive or) operation
		// between each byte of the key and the single-byte value of the iteration counter (from 19 to 0).
		BufferPtr encrypted_owner_data = make_deferred_container<Buffer>(*owner_value->GetValue());

		for (Buffer::value_type i = 19; i >= 0; --i) {
			BufferPtr key = make_deferred_container<Buffer>(password_digest.size());

			for (int j = 0; j < password_digest.size(); ++j) {
				key[j] = (password_digest[j] ^ i);
			}

			encrypted_owner_data = EncryptionUtils::ComputeRC4(key, encrypted_owner_data);
		}

		return encrypted_owner_data;
	}

	return EncryptionUtils::ComputeRC4(password_digest, 5, owner_value->GetValue());

#else
	(void) pad_password; (void) encryption_dictionary;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

BufferPtr EncryptionUtils::GenerateRandomData(int length) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

	// Initialize algorithms is required for any calculations
	MiscUtils::InitializeOpenSSL();

	BufferPtr result = make_deferred_container<Buffer>(length);

	auto rand_result = RAND_bytes((unsigned char*)result->data(), length);
	if (rand_result != 1) {
		auto openssl_error = MiscUtils::GetLastOpensslError();
		LOG_ERROR_AND_THROW_GENERAL("Could not generate random data: {}", openssl_error);
	}

	return result;

#else
	(void)length;
	throw NotSupportedException("This library was compiled without OpenSSL support");
#endif

}

} // syntax
} // vanillapdf
