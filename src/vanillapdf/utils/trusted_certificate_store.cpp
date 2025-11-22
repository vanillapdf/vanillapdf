#include "precompiled.h"

#include "utils/trusted_certificate_store.h"
#include "utils/crypto_utils.h"
#include "utils/exceptions.h"

#include <fstream>
#include <filesystem>

#if defined(VANILLAPDF_HAVE_OPENSSL)
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#endif

namespace vanillapdf {

class TrustedCertificateStore::TrustedCertificateStoreImpl {
public:
    TrustedCertificateStoreImpl();
    ~TrustedCertificateStoreImpl();

    void AddCertificateFromPEM(const Buffer& pem_data);
    void AddCertificateFromDER(const Buffer& der_data);
    void LoadFromDirectory(const std::string& directory_path);
    void LoadSystemDefaults();
    void* GetNativeHandle() const;

#if defined(VANILLAPDF_HAVE_OPENSSL)
private:
    X509_STORE* m_store = nullptr;
#endif
};

#pragma region Forwards

TrustedCertificateStore::TrustedCertificateStore() {
    m_impl = make_unique<TrustedCertificateStoreImpl>();
}

TrustedCertificateStore::~TrustedCertificateStore() = default;

void TrustedCertificateStore::AddCertificateFromPEM(const Buffer& pem_data) {
    m_impl->AddCertificateFromPEM(pem_data);
}

void TrustedCertificateStore::AddCertificateFromDER(const Buffer& der_data) {
    m_impl->AddCertificateFromDER(der_data);
}

void TrustedCertificateStore::LoadFromDirectory(const std::string& directory_path) {
    m_impl->LoadFromDirectory(directory_path);
}

void TrustedCertificateStore::LoadSystemDefaults() {
    m_impl->LoadSystemDefaults();
}

void* TrustedCertificateStore::GetNativeHandle() const {
    return m_impl->GetNativeHandle();
}

#pragma endregion

// Implementation

#if defined(VANILLAPDF_HAVE_OPENSSL)

TrustedCertificateStore::TrustedCertificateStoreImpl::TrustedCertificateStoreImpl() {
    CryptoUtils::InitializeOpenSSL();

    m_store = X509_STORE_new();
    if (!m_store) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to create X509_STORE");
    }
}

TrustedCertificateStore::TrustedCertificateStoreImpl::~TrustedCertificateStoreImpl() {
    if (m_store) {
        X509_STORE_free(m_store);
        m_store = nullptr;
    }
}

void TrustedCertificateStore::TrustedCertificateStoreImpl::AddCertificateFromPEM(const Buffer& pem_data) {
    BIO* bio = BIO_new_mem_buf(pem_data.data(), ValueConvertUtils::SafeConvert<int>(pem_data.size()));
    if (!bio) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to create BIO from PEM data");
    }

    SCOPE_GUARD([bio]() { BIO_free(bio); });

    X509* cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
    if (!cert) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to parse PEM certificate");
    }

    SCOPE_GUARD([cert]() { X509_free(cert); });

    int result = X509_STORE_add_cert(m_store, cert);

    if (result != 1) {
        // Ignore duplicate certificate errors
        unsigned long err = ERR_peek_last_error();
        if (ERR_GET_REASON(err) != X509_R_CERT_ALREADY_IN_HASH_TABLE) {
            std::string error = CryptoUtils::GetLastOpensslError();
            LOG_ERROR_AND_THROW_GENERAL("Failed to add PEM certificate to store: {}", error);
        }
    }
}

void TrustedCertificateStore::TrustedCertificateStoreImpl::AddCertificateFromDER(const Buffer& der_data) {
    const unsigned char* data_ptr = reinterpret_cast<const unsigned char*>(der_data.data());
    X509* cert = d2i_X509(nullptr, &data_ptr, ValueConvertUtils::SafeConvert<long>(der_data.size()));

    if (!cert) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to parse DER certificate");
    }

    SCOPE_GUARD([cert]() { X509_free(cert); });

    int result = X509_STORE_add_cert(m_store, cert);

    if (result != 1) {
        // Ignore duplicate certificate errors
        unsigned long err = ERR_peek_last_error();
        if (ERR_GET_REASON(err) != X509_R_CERT_ALREADY_IN_HASH_TABLE) {
            std::string error = CryptoUtils::GetLastOpensslError();
            LOG_ERROR_AND_THROW_GENERAL("Failed to add DER certificate to store: {}", error);
        }
    }
}

void TrustedCertificateStore::TrustedCertificateStoreImpl::LoadFromDirectory(const std::string& directory_path) {

    int result = X509_STORE_load_locations(m_store, nullptr, directory_path.c_str());
    if (result != 1) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to load certificates from directory: {}", directory_path);
    }
}

void TrustedCertificateStore::TrustedCertificateStoreImpl::LoadSystemDefaults() {
    int result = X509_STORE_set_default_paths(m_store);
    if (result != 1) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to load system default certificates");
    }
}

void* TrustedCertificateStore::TrustedCertificateStoreImpl::GetNativeHandle() const {
    return m_store;
}

#else

// Stub implementation when OpenSSL is not available

TrustedCertificateStore::TrustedCertificateStoreImpl::TrustedCertificateStoreImpl() {
    LOG_ERROR_AND_THROW_GENERAL("TrustedCertificateStore requires OpenSSL support");
}

TrustedCertificateStore::TrustedCertificateStoreImpl::~TrustedCertificateStoreImpl() = default;

void TrustedCertificateStore::TrustedCertificateStoreImpl::AddCertificateFromPEM(const std::string&) {
    LOG_ERROR_AND_THROW_GENERAL("TrustedCertificateStore requires OpenSSL support");
}

void TrustedCertificateStore::TrustedCertificateStoreImpl::AddCertificateFromDER(const Buffer&) {
    LOG_ERROR_AND_THROW_GENERAL("TrustedCertificateStore requires OpenSSL support");
}

void TrustedCertificateStore::TrustedCertificateStoreImpl::AddCertificateFromFile(const std::string&) {
    LOG_ERROR_AND_THROW_GENERAL("TrustedCertificateStore requires OpenSSL support");
}

void TrustedCertificateStore::TrustedCertificateStoreImpl::LoadFromDirectory(const std::string&) {
    LOG_ERROR_AND_THROW_GENERAL("TrustedCertificateStore requires OpenSSL support");
}

void TrustedCertificateStore::TrustedCertificateStoreImpl::LoadSystemDefaults() {
    LOG_ERROR_AND_THROW_GENERAL("TrustedCertificateStore requires OpenSSL support");
}

void* TrustedCertificateStore::TrustedCertificateStoreImpl::GetNativeHandle() const {
    LOG_ERROR_AND_THROW_GENERAL("TrustedCertificateStore requires OpenSSL support");
    return nullptr;
}

#endif

} // vanillapdf
