#ifndef _MISC_UTILS_H
#define _MISC_UTILS_H

#include "utils/buffer.h"
#include "utils/message_digest_algorithm.h"

#include <string>

// Foward declaration from openssl instead of including
struct env_md_st;
typedef struct env_md_st EVP_MD;

namespace gotchangpdf {

class MiscUtils {
public:
	static std::string ToBase64(const Buffer& value);
	static BufferPtr FromBase64(const std::string& value);

	static const EVP_MD* GetAlgorithm(MessageDigestAlgorithm algorithm);
	static BufferPtr CalculateHash(const Buffer& data, MessageDigestAlgorithm digest_algorithm);

	static std::string ExtractFilename(const std::string& path);

	static void InitializeOpenSSL();

private:
	MiscUtils();
};

} // gotchangpdf

#endif /* _MISC_UTILS_H */
