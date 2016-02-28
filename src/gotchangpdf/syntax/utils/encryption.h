#ifndef _ENCRYPTION_H
#define _ENCRYPTION_H

#include "syntax_fwd.h"

namespace gotchangpdf
{
	extern const uint8_t HARDCODED_PFD_PAD[];
	extern const int HARDCODED_PFD_PAD_LENGTH;

	class EncryptionUtils
	{
	public:
		static BufferPtr PadTruncatePassword(const Buffer& password);
		static BufferPtr ComputeRC4(const Buffer& key, const Buffer& data);
		static BufferPtr ComputeRC4(const Buffer& key, int key_length, const Buffer& data);
	};
}

#endif /* _ENCRYPTION_H */
