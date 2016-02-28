#include "precompiled.h"
#include "encryption.h"

#include "buffer.h"

#include <openssl/rc4.h>

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

	const int HARDCODED_PFD_PAD_LENGTH = sizeof(HARDCODED_PFD_PAD);

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
}
