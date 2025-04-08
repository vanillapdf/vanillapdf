#ifndef _MISC_UTILS_H
#define _MISC_UTILS_H

#include "utils/buffer.h"
#include "utils/message_digest_algorithm.h"

// Foward declaration from openssl instead of including
// struct env_md_st;
// typedef struct env_md_st EVP_MD;

// I have tried forward declaring EVP_MD, but it did not work on the 64-bit
// openssl version. It might work in the future, but lets not waste more time.

#include <openssl/ossl_typ.h>

#include <string>

namespace vanillapdf {

class MiscUtils {
public:
    static BufferPtr ToBase64(const Buffer& value);
    static BufferPtr FromBase64(const Buffer& value);

    static const EVP_MD* GetAlgorithm(MessageDigestAlgorithm algorithm);
    static BufferPtr CalculateHash(const Buffer& data, MessageDigestAlgorithm digest_algorithm);

    static std::string ExtractFilename(const std::string& path);
    static bool CaseInsensitiveCompare(const std::string& left, const std::string& right);

    static void InitializeOpenSSL();
    static std::string GetLastOpensslError();

private:
    MiscUtils();
};

} // vanillapdf

#endif /* _MISC_UTILS_H */
