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
		static BufferPtr PadTruncatePassword(const Buffer& password);
		static BufferPtr ComputeRC4(const Buffer& key, const Buffer& data);
		static BufferPtr ComputeRC4(const Buffer& key, int key_length, const Buffer& data);

		static BufferPtr AESDecrypt(const Buffer& key, const Buffer& data);
		static BufferPtr AESDecrypt(const Buffer& key, int key_length, const Buffer& data);

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
	};
}

#endif /* _ENCRYPTION_H */
