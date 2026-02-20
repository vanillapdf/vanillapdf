#ifndef _CRYPTO_UTILS_H
#define _CRYPTO_UTILS_H

#include "utils/utils_fwd.h"
#include "utils/message_digest_algorithm.h"

#include <ctime>
#include <string>

// OpenSSL forward declarations - avoid including heavy OpenSSL headers
#if defined(VANILLAPDF_HAVE_OPENSSL)

// Forward declarations for commonly used OpenSSL types
// Note: Only include types that are used in public header interfaces
// Types used only in .cpp files should not be forward declared here
struct x509_st;
struct X509_name_st;
struct x509_store_st;
struct x509_store_ctx_st;
struct evp_pkey_st;
struct evp_pkey_ctx_st;
struct evp_md_st;
struct evp_md_ctx_st;
struct evp_cipher_ctx_st;
struct bio_st;
struct pkcs7_st;
struct asn1_string_st;
struct stack_st_X509;
struct CMS_ContentInfo_st;

typedef struct x509_st X509;
typedef struct X509_name_st X509_NAME;
typedef struct x509_store_st X509_STORE;
typedef struct x509_store_ctx_st X509_STORE_CTX;
typedef struct evp_pkey_st EVP_PKEY;
typedef struct evp_pkey_ctx_st EVP_PKEY_CTX;
typedef struct evp_md_st EVP_MD;
typedef struct evp_md_ctx_st EVP_MD_CTX;
typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;
typedef struct bio_st BIO;
typedef struct pkcs7_st PKCS7;
typedef struct asn1_string_st ASN1_TIME;
typedef struct stack_st_X509 STACK_OF_X509;
typedef struct CMS_ContentInfo_st CMS_ContentInfo;

#endif // VANILLAPDF_HAVE_OPENSSL

namespace vanillapdf {

/**
 * \class CryptoUtils
 * \brief Centralized cryptographic utility functions
 *
 * This class provides common cryptographic operations used throughout
 * the vanillapdf library. All methods are static utility functions.
 */
class CryptoUtils {
public:
#if defined(VANILLAPDF_HAVE_OPENSSL)

    /**
     * \brief Get EVP_MD algorithm for a message digest algorithm
     * \param algorithm Message digest algorithm enum
     * \return EVP_MD pointer for the algorithm
     * \throws GeneralException if algorithm is unknown or unsupported
     */
    static const EVP_MD* GetAlgorithm(MessageDigestAlgorithm algorithm);

#endif // VANILLAPDF_HAVE_OPENSSL

    /**
     * \brief Initialize OpenSSL library
     *
     * Thread-safe one-time initialization of OpenSSL.
     * On OpenSSL 3.x, loads legacy and default providers.
     * Registers cleanup handler via atexit().
     *
     * \throws GeneralException if initialization fails
     * \throws NotSupportedException if compiled without OpenSSL
     */
    static void InitializeOpenSSL();

    /**
     * \brief Cleanup OpenSSL library resources
     *
     * Called automatically via atexit() on OpenSSL 3.x.
     * Unloads providers and cleans up OpenSSL state.
     */
    static void CleanupOpenSSL();

    /**
     * \brief Get last OpenSSL error message
     * \return Formatted error message with file, line, function, and data
     * \throws NotSupportedException if compiled without OpenSSL
     */
    static std::string GetLastOpensslError();

private:
    CryptoUtils() = delete;
    ~CryptoUtils() = delete;
};

} // namespace vanillapdf

#endif /* _CRYPTO_UTILS_H */
