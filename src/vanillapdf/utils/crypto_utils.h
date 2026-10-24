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
     * \brief Initialize OpenSSL with the default settings, unless already initialized
     *
     * Called before every cryptographic operation. Does nothing once initialized,
     * whichever settings the initialization used.
     *
     * \throws CryptoErrorException if initialization fails
     * \throws NotSupportedException if compiled without OpenSSL
     */
    static void EnsureInitialized();

    /**
     * \brief Initialize OpenSSL with explicit settings
     *
     * Thread-safe. Loads the providers the settings ask for: with legacy algorithms
     * enabled the legacy and the default provider, otherwise none explicitly when built
     * without VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER, so the default provider and
     * the process configuration apply.
     *
     * Does nothing once initialized, whichever settings the initialization used, so that
     * no operation runs under changing providers. Different settings need Cleanup() first.
     * Use IsLegacyAlgorithmsAvailable() to check the outcome.
     *
     * \throws InvalidParameterException if the legacy algorithms policy is not set
     * \throws NotSupportedException if the settings require legacy algorithms that cannot be enabled
     * \throws CryptoErrorException if initialization fails
     */
    static void Initialize(const CryptoSettings& settings);

    /**
     * \brief Check whether the legacy algorithms (RC2, MD2, MD4) can be used
     *
     * Initializes OpenSSL with the default settings first, unless already initialized.
     * Reports the provider as OpenSSL sees it, so a legacy provider the process
     * configuration loaded counts as well.
     */
    static bool IsLegacyAlgorithmsAvailable();

    /**
     * \brief Unload the providers the initialization loaded
     *
     * Optional: OpenSSL releases every loaded provider when the process exits. Does not
     * shut OpenSSL down, which would affect the whole process and could not be undone,
     * so the library can be initialized again afterwards.
     */
    static void Cleanup();

    /**
     * \brief Get last OpenSSL error message
     * \return Formatted error message with file, line, function, and data
     * \throws NotSupportedException if compiled without OpenSSL
     */
    static std::string GetLastOpensslError();

    /**
     * \brief Check whether the oldest queued OpenSSL error reports an unavailable algorithm
     *
     * OpenSSL 3 reports an algorithm no loaded provider implements, such as RC2 without
     * the legacy provider, as unsupported. Leaves the error queue intact.
     * \return true if the algorithm is unavailable, false for any other error
     */
    static bool IsAlgorithmUnavailableError();

private:
    CryptoUtils() = delete;
    ~CryptoUtils() = delete;
};

} // namespace vanillapdf

#endif /* _CRYPTO_UTILS_H */
