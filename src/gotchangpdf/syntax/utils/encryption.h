#ifndef _ENCRYPTION_H
#define _ENCRYPTION_H

#include "syntax_fwd.h"

namespace gotchangpdf
{
	extern const uint8_t HARDCODED_PFD_PAD[];
	extern const uint8_t AES_ADDITIONAL_SALT[];
	extern const int HARDCODED_PFD_PAD_LENGTH;
	extern const int AES_ADDITIONAL_SALT_LENGTH;

	enum class EncryptionAlgorithm
	{
		None = 0,
		RC4,
		AES
	};

	class EncryptionUtils
	{
	public:
		static BufferPtr ComputeObjectKey(
			const Buffer& key,
			types::big_uint objNumber,
			types::ushort genNumber,
			EncryptionAlgorithm alg);

		static BufferPtr PadTruncatePassword(const Buffer& password);
		static BufferPtr ComputeRC4(const Buffer& key, const Buffer& data);
		static BufferPtr ComputeRC4(const Buffer& key, int key_length, const Buffer& data);

		static BufferPtr AESDecrypt(const Buffer& key, const Buffer& data);
		static BufferPtr AESDecrypt(const Buffer& key, int key_length, const Buffer& data);
		static BufferPtr AESEncrypt(const Buffer& key, const Buffer& data);
		static BufferPtr AESEncrypt(const Buffer& key, int key_length, const Buffer& data);

		static BufferPtr AddPkcs7Padding(const Buffer& data, size_t block_size);
		static BufferPtr RemovePkcs7Padding(const Buffer& data, size_t block_size);

		static BufferPtr DecryptEnvelopedData(const syntax::ArrayObject<syntax::StringObjectPtr>& recipients, const IEncryptionKey& key);
		static BufferPtr ComputeEncryptedOwnerData(const Buffer& pad_password, const syntax::DictionaryObject& encryption_dictionary);
		static BufferPtr GetRecipientKey(
			const syntax::ArrayObject<syntax::StringObjectPtr>& recipients,
			const syntax::IntegerObject& length_bits,
			EncryptionAlgorithm algorithm,
			const IEncryptionKey& key);

		static bool CheckKey(
			const Buffer& input,
			const Buffer& document_id,
			const Buffer& owner_data,
			const Buffer& user_data,
			const syntax::IntegerObject& permissions,
			const syntax::IntegerObject& revision,
			const syntax::IntegerObject& key_length,
			Buffer& decryption_key);
	};

	class IEncryptionKey
	{
	public:
		virtual BufferPtr Decrypt(const Buffer& data) const = 0;
		virtual bool Equals(const Buffer& issuer, const Buffer& serial) const = 0;
		virtual ~IEncryptionKey() {};
	};
}

#endif /* _ENCRYPTION_H */
