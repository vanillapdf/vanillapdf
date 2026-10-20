#include "precompiled.h"

#include "utils/crypto_utils.h"
#include "utils/buffer.h"

#include <chrono>
#include <mutex>
#include <fmt/core.h>

#if defined(VANILLAPDF_HAVE_OPENSSL)
#include <openssl/x509.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#if OPENSSL_VERSION_MAJOR >= 3 && defined(VANILLAPDF_LOAD_OPENSSL_LEGACY_PROVIDER)
    #include <openssl/provider.h>
#endif
#endif

namespace vanillapdf {

#if defined(VANILLAPDF_HAVE_OPENSSL)

// Static variables for OpenSSL 3.x one-time initialization
#if OPENSSL_VERSION_MAJOR >= 3
static std::mutex g_openssl_lock;
static bool g_openssl_initialized = false;

#if defined(VANILLAPDF_LOAD_OPENSSL_LEGACY_PROVIDER)
static OSSL_PROVIDER* g_legacy_provider = nullptr;
static OSSL_PROVIDER* g_default_provider = nullptr;
#endif
#endif

const EVP_MD* CryptoUtils::GetAlgorithm(MessageDigestAlgorithm algorithm) {

    if (algorithm == MessageDigestAlgorithm::Undefined) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "No message digest algorithm was selected");
    }

    if (algorithm == MessageDigestAlgorithm::MDNULL) {
        return EVP_md_null();
    }

    if (algorithm == MessageDigestAlgorithm::MD2) {
    #ifndef OPENSSL_NO_MD2
        return EVP_md2();
    #else
        LOG_ERROR_AND_THROW(NotSupportedException, "OpenSSL was compiled without MD2 message digest support");
    #endif
    }

    if (algorithm == MessageDigestAlgorithm::MD4) {
        return EVP_md4();
    }

    if (algorithm == MessageDigestAlgorithm::MD5) {
        return EVP_md5();
    }

    if (algorithm == MessageDigestAlgorithm::SHA1) {
        return EVP_sha1();
    }

    if (algorithm == MessageDigestAlgorithm::SHA224) {
        return EVP_sha224();
    }

    if (algorithm == MessageDigestAlgorithm::SHA256) {
        return EVP_sha256();
    }

    if (algorithm == MessageDigestAlgorithm::SHA384) {
        return EVP_sha384();
    }

    if (algorithm == MessageDigestAlgorithm::SHA512) {
        return EVP_sha512();
    }

    if (algorithm == MessageDigestAlgorithm::MDC2) {
    #ifndef OPENSSL_NO_MDC2
        return EVP_mdc2();
    #else
        LOG_ERROR_AND_THROW(NotSupportedException, "OpenSSL was compiled without MDC2 message digest support");
    #endif
    }

    if (algorithm == MessageDigestAlgorithm::RIPEMD160) {
        return EVP_ripemd160();
    }

    if (algorithm == MessageDigestAlgorithm::WHIRLPOOL) {
        return EVP_whirlpool();
    }

    LOG_ERROR_AND_THROW(CryptoErrorException, "Unknown message digest algorithm: {}", static_cast<int>(algorithm));
}

std::string CryptoUtils::GetLastOpensslError() {

    const char* err_file = nullptr;
    const char* err_data = nullptr;

#if OPENSSL_VERSION_MAJOR >= 3
    const char* err_func = nullptr;
#endif

    int err_line = 0;
    int err_flags = 0;

#if OPENSSL_VERSION_MAJOR >= 3
    auto err_code = ERR_get_error_all(&err_file, &err_line, &err_func, &err_data, &err_flags);
#else
    auto err_code = ERR_get_error_line_data(&err_file, &err_line, &err_data, &err_flags);
#endif

    // OpenSSL leaves the strings null when it has nothing to report, fmt refuses null pointers
    auto to_string = [](const char* value) { return std::string_view(value != nullptr ? value : ""); };

#if OPENSSL_VERSION_MAJOR >= 3
    return fmt::format(
        "Error: '{}'\nFile: '{}'\nLine: '{}'\nFunction: '{}'\nData: '{}'\nFlags: '{}'\n",
        err_code, to_string(err_file), err_line, to_string(err_func), to_string(err_data), err_flags);
#else
    return fmt::format(
        "Error: '{}'\nFile: '{}'\nLine: '{}'\nData: '{}'\nFlags: '{}'\n",
        err_code, to_string(err_file), err_line, to_string(err_data), err_flags);
#endif
}

bool CryptoUtils::IsAlgorithmUnavailableError() {

#if OPENSSL_VERSION_MAJOR >= 3
    return ERR_GET_REASON(ERR_peek_error()) == ERR_R_UNSUPPORTED;
#else
    // OpenSSL 1.x has no providers, every compiled-in algorithm is available
    return false;
#endif
}

void CryptoUtils::InitializeOpenSSL() {

#if OPENSSL_VERSION_MAJOR >= 3
    if (g_openssl_initialized) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_openssl_lock);
    if (g_openssl_initialized) {
        return;
    }

#if defined(VANILLAPDF_LOAD_OPENSSL_LEGACY_PROVIDER)

    // Opted in at build time (VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER) for legacy key files,
    // envelopes and signatures. Loading any provider explicitly stops OpenSSL from activating
    // the default provider implicitly, so the default provider has to be loaded as well.
    g_legacy_provider = OSSL_PROVIDER_load(nullptr, "legacy");
    if (g_legacy_provider == nullptr) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Failed to initialize legacy OSSL provider, {}", GetLastOpensslError());
    }

    g_default_provider = OSSL_PROVIDER_load(nullptr, "default");
    if (g_default_provider == nullptr) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Failed to initialize default OSSL provider, {}", GetLastOpensslError());
    }

#endif

    // Without the opt-in no provider is loaded explicitly. The default provider is activated
    // implicitly, and anything else (legacy, FIPS) is left to the process configuration.
    // The PDF algorithms the legacy provider used to supply (RC4, MD5) are implemented in-house.
    OpenSSL_add_all_algorithms();

    g_openssl_initialized = true;

    std::atexit(CryptoUtils::CleanupOpenSSL);
#else
    // OpenSSL 1.x - just add algorithms
    OpenSSL_add_all_algorithms();
#endif
}

void CryptoUtils::CleanupOpenSSL() {

#if OPENSSL_VERSION_MAJOR >= 3
    if (!g_openssl_initialized) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_openssl_lock);
    if (!g_openssl_initialized) {
        return;
    }

#if defined(VANILLAPDF_LOAD_OPENSSL_LEGACY_PROVIDER)
    if (g_legacy_provider != nullptr) {
        OSSL_PROVIDER_unload(g_legacy_provider);
        g_legacy_provider = nullptr;
    }

    if (g_default_provider != nullptr) {
        OSSL_PROVIDER_unload(g_default_provider);
        g_default_provider = nullptr;
    }
#endif

    OPENSSL_cleanup();
    g_openssl_initialized = false;
#endif
    // OpenSSL 1.x - no cleanup needed
}

#else

// Stub implementations when OpenSSL is not available

const EVP_MD* CryptoUtils::GetAlgorithm(MessageDigestAlgorithm) {
    LOG_ERROR_AND_THROW(NotSupportedException, "This library was compiled without OpenSSL support");
}

std::string CryptoUtils::GetLastOpensslError() {
    LOG_ERROR_AND_THROW(NotSupportedException, "This library was compiled without OpenSSL support");
}

bool CryptoUtils::IsAlgorithmUnavailableError() {
    LOG_ERROR_AND_THROW(NotSupportedException, "This library was compiled without OpenSSL support");
}


void CryptoUtils::InitializeOpenSSL() {
    LOG_ERROR_AND_THROW(NotSupportedException, "This library was compiled without OpenSSL support");
}

void CryptoUtils::CleanupOpenSSL() {
    // Ignore when OpenSSL support is disabled
}

#endif // VANILLAPDF_HAVE_OPENSSL

} // namespace vanillapdf
