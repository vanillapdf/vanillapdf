#ifndef _ENCRYPTION_H
#define _ENCRYPTION_H

#include "syntax/utils/syntax_fwd.h"

#include "utils/buffer.h"
#include "utils/encryption_key_interface.h"

namespace vanillapdf {
namespace syntax {

extern const uint8_t HARDCODED_PDF_PAD[];
extern const uint8_t AES_ADDITIONAL_SALT[];

enum class EncryptionAlgorithm {
	Undefined = 0,
	None,
	RC4,
	AES
};

class EncryptionUtils {
public:
	static BufferPtr ComputeObjectKey(
		const Buffer& key,
		types::big_uint objNumber,
		types::ushort genNumber,
		EncryptionAlgorithm alg);

	static BufferPtr PadTruncatePassword(const Buffer& password);
	static BufferPtr ComputeRC4(const Buffer& key, const Buffer& data);
	static BufferPtr ComputeRC4(const Buffer& key, types::size_type key_length, const Buffer& data);

	static BufferPtr ComputeMD5(const Buffer& data);

	static BufferPtr AESDecrypt(const Buffer& key, const Buffer& data);
	static BufferPtr AESDecrypt(const Buffer& key, types::size_type key_length, const Buffer& data);
	static BufferPtr AESEncrypt(const Buffer& key, const Buffer& data);
	static BufferPtr AESEncrypt(const Buffer& key, types::size_type key_length, const Buffer& data);

	static BufferPtr AddPkcs7Padding(const Buffer& data, types::size_type block_size);
	static BufferPtr RemovePkcs7Padding(const Buffer& data, types::size_type block_size);

	static BufferPtr DecryptEnvelopedData(const syntax::ArrayObject<syntax::StringObjectPtr>& recipients, IEncryptionKey& key);
	static BufferPtr ComputeEncryptedOwnerData(const Buffer& pad_password, const syntax::DictionaryObject& encryption_dictionary);
	static BufferPtr GetRecipientKey(
		const syntax::ArrayObject<syntax::StringObjectPtr>& recipients,
		const syntax::IntegerObject& length_bits,
		EncryptionAlgorithm algorithm,
		IEncryptionKey& key);

	static BufferPtr GenerateOwnerEncryptionKey(
		const Buffer& document_id,
		const Buffer& owner_password,
		const Buffer& user_password,
		EncryptionAlgorithm algorithm,
		int32_t key_length,
		uint32_t permissions,
		int32_t revision);

	static BufferPtr GenerateUserEncryptionKey(
		const Buffer& document_id,
		const Buffer& user_password,
		const Buffer& owner_data,
		EncryptionAlgorithm algorithm,
		int32_t key_length,
		int32_t permissions,
		int32_t revision);

	static bool CheckKey(
		const Buffer& input,
		const Buffer& document_id,
		const Buffer& owner_data,
		const Buffer& user_data,
		uint32_t permissions,
		int32_t revision,
		int32_t key_length,
		Buffer& decryption_key);

	static BufferPtr CalculateDecryptionKeyDigest(
		const Buffer& input,
		const Buffer& document_id,
		const Buffer& owner_data,
		uint32_t permissions);

	static BufferPtr CalculateDecryptionCompareDataV3(
		BufferPtr decryption_key_digest,
		const Buffer& document_id,
		int32_t key_length);

	static BufferPtr GenerateRandomData(int length);
};

} // syntax
} // vanillapdf

#endif /* _ENCRYPTION_H */
