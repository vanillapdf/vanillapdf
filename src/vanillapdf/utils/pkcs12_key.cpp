#include "precompiled.h"

#include "utils/crypto_utils.h"
#include "utils/misc_utils.h"
#include "utils/pkcs12_key.h"
#include "utils/license_info.h"
#include "utils/streams/input_stream.h"

#include <fstream>

#if defined(VANILLAPDF_HAVE_OPENSSL)

#include <openssl/pkcs12.h>
#include <openssl/cms.h>
#include <openssl/evp.h>
#include <openssl/engine.h>
#include <openssl/err.h>
#include <openssl/x509.h>

#endif

namespace vanillapdf {

class PKCS12Key::PKCS12KeyImpl {
public:
    explicit PKCS12KeyImpl(const Buffer& data);
    explicit PKCS12KeyImpl(const std::string& path);
    PKCS12KeyImpl(const std::string& path, std::string_view password);
    PKCS12KeyImpl(const Buffer& data, std::string_view password);
    ~PKCS12KeyImpl() = default;

    // IEncryptionKey
    BufferPtr Decrypt(const Buffer& data);
    bool ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const;

    // ISigningKey
    void SignInitialize(MessageDigestAlgorithm algorithm);
    void SignUpdate(const Buffer& data);
    void SignUpdate(IInputStreamPtr data, types::stream_size length);
    BufferPtr SignFinal();
    void SignCleanup();

    // Certificate access
    BufferPtr GetCertificate() const;

#if defined(VANILLAPDF_HAVE_OPENSSL)

private:
    // Custom deleters for OpenSSL types
    struct PKCS12Deleter { void operator()(PKCS12* p) const { if (p) PKCS12_free(p); } };
    struct EVPPKEYDeleter { void operator()(EVP_PKEY* p) const { if (p) EVP_PKEY_free(p); } };
    struct X509Deleter { void operator()(X509* p) const { if (p) X509_free(p); } };
    struct EVPPKEYCTXDeleter { void operator()(EVP_PKEY_CTX* p) const { if (p) EVP_PKEY_CTX_free(p); } };
    struct CMSDeleter { void operator()(CMS_ContentInfo* p) const { if (p) CMS_ContentInfo_free(p); } };
    struct BIODeleter { void operator()(BIO* p) const { if (p) BIO_free_all(p); } };

    std::unique_ptr<PKCS12, PKCS12Deleter> p12;
    std::unique_ptr<EVP_PKEY, EVPPKEYDeleter> key;
    std::unique_ptr<EVP_PKEY_CTX, EVPPKEYCTXDeleter> encryption_context;
    std::unique_ptr<X509, X509Deleter> cert;

    std::unique_ptr<CMS_ContentInfo, CMSDeleter> m_cms;
    std::unique_ptr<BIO, BIODeleter> m_data_bio;

#endif

    BufferArrayPtr m_certificates;

    void Load(const Buffer& data, std::string_view password);
};

#pragma region Forwards

// Forwards to implementation
PKCS12Key::PKCS12Key(const Buffer& data) {
    m_impl = make_unique<PKCS12KeyImpl>(data);
}

PKCS12Key::PKCS12Key(const std::string& path) {
    m_impl = make_unique<PKCS12KeyImpl>(path);
}

PKCS12Key::PKCS12Key(const std::string& path, std::string_view password) {
    m_impl = make_unique<PKCS12KeyImpl>(path, password);
}

PKCS12Key::PKCS12Key(const Buffer& data, std::string_view password) {
    m_impl = make_unique<PKCS12KeyImpl>(data, password);
}

BufferPtr PKCS12Key::Decrypt(const Buffer& data) {
    return m_impl->Decrypt(data);
}

bool PKCS12Key::ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const {
    return m_impl->ContainsPrivateKey(issuer, serial);
}

void PKCS12Key::SignInitialize(MessageDigestAlgorithm algorithm) {
    return m_impl->SignInitialize(algorithm);
}

void PKCS12Key::SignUpdate(const Buffer& data) {
    return m_impl->SignUpdate(data);
}

void PKCS12Key::SignUpdate(IInputStreamPtr data, types::stream_size length) {
    return m_impl->SignUpdate(data, length);
}

BufferPtr PKCS12Key::SignFinal() {
    return m_impl->SignFinal();
}

void PKCS12Key::SignCleanup() {
    return m_impl->SignCleanup();
}

BufferPtr PKCS12Key::GetCertificate() const {
    return m_impl->GetCertificate();
}

#pragma endregion

    // Actual implementation
PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const std::string& path) : PKCS12KeyImpl(path, std::string_view()) {}
PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const std::string& path, std::string_view password) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    SCOPE_GUARD_CAPTURE_REFERENCES(file.close());

    if (!file || !file.good()) {
        throw CryptoErrorException("Could not open file: " + path);
    }

    Buffer data;
    Buffer buffer(constant::BUFFER_SIZE);

    while (!file.eof()) {
        file.read(buffer.data(), constant::BUFFER_SIZE);
        auto read = file.gcount();
        if (0 == read) {
            break;
        }

        auto read_converted = ValueConvertUtils::SafeConvert<Buffer::iterator::difference_type>(read);
        data.insert(data.end(), buffer.begin(), buffer.begin() + read_converted);
    }

    Load(data, password);
}

PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data) : PKCS12KeyImpl(data, std::string_view()) {}

PKCS12Key::PKCS12KeyImpl::PKCS12KeyImpl(const Buffer& data, std::string_view password) {
    Load(data, password);
}

void PKCS12Key::PKCS12KeyImpl::Load(const Buffer& data, std::string_view password) {

    CryptoUtils::InitializeOpenSSL();

    // The const cast seems to be unnecessary, but on my linux VM
    // with OpenSSL 1.0.1f method BIO_new_mem_buf takes only void*
    auto buffer_data = const_cast<char *>(data.data());
    int buffer_length = ValueConvertUtils::SafeConvert<int>(data.size());

    BIO* bio = BIO_new_mem_buf(buffer_data, buffer_length);
    SCOPE_GUARD([bio]() { BIO_free(bio); } );

    PKCS12* p12_raw = d2i_PKCS12_bio(bio, nullptr);
    if (nullptr == p12_raw) {
        throw CryptoErrorException("Could not parse der structure PKCS#12, " + CryptoUtils::GetLastOpensslError());
    }
    p12 = std::unique_ptr<PKCS12, PKCS12Deleter>(p12_raw);

    STACK_OF(X509) *additional_certs = nullptr;
    EVP_PKEY* key_raw = nullptr;
    X509* cert_raw = nullptr;
    int parsed = PKCS12_parse(p12.get(), password.data(), &key_raw, &cert_raw, &additional_certs);
    if (1 != parsed) {
        throw CryptoErrorException("Could not parse PKCS#12, " + CryptoUtils::GetLastOpensslError());
    }

    key = std::unique_ptr<EVP_PKEY, EVPPKEYDeleter>(key_raw);
    cert = std::unique_ptr<X509, X509Deleter>(cert_raw);

    assert(key != nullptr);
    assert(cert != nullptr);

    SCOPE_GUARD([additional_certs]() { sk_X509_pop_free(additional_certs, X509_free); });

    auto additional_certs_size = sk_X509_num(additional_certs);
    for (decltype(additional_certs_size) i = 0; i < additional_certs_size; ++i) {
        auto additional_cert = sk_X509_value(additional_certs, i);

        int length = i2d_X509(additional_cert, nullptr);
        if (length < 0) {
            throw CryptoErrorException("Could not get PKCS#7 size, " + CryptoUtils::GetLastOpensslError());
        }

        BufferPtr additional_cert_data = make_deferred_container<Buffer>(length);
        auto data_pointer = (unsigned char *) additional_cert_data->data();
        int converted = i2d_X509(additional_cert, &data_pointer);
        if (converted < 0) {
            throw CryptoErrorException("Could not convert PKCS#7, " + CryptoUtils::GetLastOpensslError());
        }

        m_certificates->Append(additional_cert_data);
    }
}

BufferPtr PKCS12Key::PKCS12KeyImpl::Decrypt(const Buffer& data) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

    if (!encryption_context) {
        auto evp_pkey_context = EVP_PKEY_CTX_new_from_pkey(nullptr, key.get(), nullptr);
        if (evp_pkey_context == nullptr) {
            LOG_ERROR_AND_THROW(CryptoErrorException, "Could not create PKEY context: {}", CryptoUtils::GetLastOpensslError());
        }

        encryption_context = std::unique_ptr<EVP_PKEY_CTX, EVPPKEYCTXDeleter>(evp_pkey_context);
    }

    int init_result = EVP_PKEY_decrypt_init(encryption_context.get());
    if (init_result != 1) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Could not initialize encryption engine: {}", CryptoUtils::GetLastOpensslError());
    }

    size_t outlen = 0;
    int length_result = EVP_PKEY_decrypt(encryption_context.get(), nullptr, &outlen, (unsigned char *) data.data(), data.std_size());
    if (length_result != 1) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Could not get decrypt message length: {}", CryptoUtils::GetLastOpensslError());
    }

    BufferPtr output = make_deferred_container<Buffer>(outlen);
    int decrypt_result = EVP_PKEY_decrypt(encryption_context.get(), (unsigned char *) output->data(), &outlen, (unsigned char *) data.data(), data.std_size());
    if (decrypt_result != 1) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Could not get decrypt message: {}", CryptoUtils::GetLastOpensslError());
    }

    return output;

#else

    (void) data;
    throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

bool PKCS12Key::PKCS12KeyImpl::ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const {

#if defined(VANILLAPDF_HAVE_OPENSSL)

    // X509_NAME is conflicting with some windows headers
    auto issuer_name = X509_get_issuer_name(cert.get());
    ASN1_INTEGER* serial_asn = X509_get_serialNumber(cert.get());

    // Convert issuer to null terminated string
    char* oneline = X509_NAME_oneline(issuer_name, nullptr, 0);
    SCOPE_GUARD([oneline]() {OPENSSL_free(oneline); });

    if (oneline == nullptr) {
        spdlog::error("Could not print issuer to buffer");
        return false;
    }

    Buffer m_issuer(oneline);
    Buffer m_serial(serial_asn->data, serial_asn->length);
    return m_issuer.Equals(issuer) && m_serial.Equals(serial);

#else

    (void) issuer; (void) serial;
    throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

void PKCS12Key::PKCS12KeyImpl::SignInitialize(MessageDigestAlgorithm algorithm) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

    // Reset existing state
    m_cms.reset();
    m_data_bio.reset();

    // For EdDSA keys (Ed25519/Ed448), RFC 8419 requires a specific CMS digestAlgorithm:
    // Ed25519 → SHA-512, Ed448 → SHAKE256. OpenSSL cannot auto-detect the digest from
    // the key alone (EVP_PKEY_get_default_digest_name returns error), so we specify it
    // explicitly. At the raw EVP level Ed25519 still uses a pure (no prehash) scheme;
    // OpenSSL's CMS layer handles the two-step separation correctly.
    int key_type = EVP_PKEY_base_id(key.get());
    const EVP_MD* message_digest;
    if (key_type == EVP_PKEY_ED25519) {
        message_digest = EVP_sha512();
    } else if (key_type == EVP_PKEY_ED448) {
        message_digest = EVP_shake256();
    } else {
        message_digest = CryptoUtils::GetAlgorithm(algorithm);
    }

    // Create a partial detached CMS SignedData structure
    CMS_ContentInfo* cms_raw = CMS_sign(nullptr, nullptr, nullptr, nullptr,
                                        CMS_PARTIAL | CMS_DETACHED | CMS_BINARY);
    if (cms_raw == nullptr) {
        throw CryptoErrorException("Could not create CMS structure, " + CryptoUtils::GetLastOpensslError());
    }
    m_cms = std::unique_ptr<CMS_ContentInfo, CMSDeleter>(cms_raw);

    // Add signer. CMS_add1_signer automatically embeds the signer certificate
    // (unless CMS_NOCERTS is set). CMS_PARTIAL defers the actual signing to
    // CMS_final(), allowing us to add attributes before signing.
    CMS_SignerInfo* si = CMS_add1_signer(m_cms.get(), cert.get(), key.get(), message_digest,
                                         CMS_PARTIAL | CMS_NOSMIMECAP);
    if (si == nullptr) {
        throw CryptoErrorException("Could not add CMS signer, " + CryptoUtils::GetLastOpensslError());
    }

    // Add signing time signed attribute
    ASN1_UTCTIME* signing_time = X509_gmtime_adj(nullptr, 0);
    if (signing_time == nullptr) {
        throw CryptoErrorException("Could not create signing time, " + CryptoUtils::GetLastOpensslError());
    }
    SCOPE_GUARD([signing_time]() { ASN1_UTCTIME_free(signing_time); });

    if (!CMS_signed_add1_attr_by_NID(si, NID_pkcs9_signingTime,
                                      V_ASN1_UTCTIME, signing_time, -1)) {
        throw CryptoErrorException("Could not add signing time attribute, " + CryptoUtils::GetLastOpensslError());
    }

    // Embed certificate chain (intermediate CAs from the PKCS#12 bag).
    // The signer's own certificate is already embedded by CMS_add1_signer above.
    auto extra_certificates_size = m_certificates->GetSize();
    for (decltype(extra_certificates_size) i = 0; i < extra_certificates_size; ++i) {
        auto extra_certificate_data = m_certificates[i];

        auto raw_data = (const unsigned char*) extra_certificate_data->data();
        auto raw_data_size = ValueConvertUtils::SafeConvert<long>(extra_certificate_data->size());
        auto extra_certificate = d2i_X509(nullptr, &raw_data, raw_data_size);
        if (extra_certificate == nullptr) {
            throw CryptoErrorException("Extra certificate is invalid, " + CryptoUtils::GetLastOpensslError());
        }
        SCOPE_GUARD([extra_certificate]() { X509_free(extra_certificate); });

        if (!CMS_add1_cert(m_cms.get(), extra_certificate)) {
            throw CryptoErrorException("Could not add extra certificate, " + CryptoUtils::GetLastOpensslError());
        }
    }

    // Create a memory BIO to accumulate the signed data from SignUpdate calls.
    // CMS_final() reads this BIO to compute the message digest and perform signing.
    BIO* bio_raw = BIO_new(BIO_s_mem());
    if (bio_raw == nullptr) {
        throw CryptoErrorException("Could not create data BIO, " + CryptoUtils::GetLastOpensslError());
    }
    m_data_bio = std::unique_ptr<BIO, BIODeleter>(bio_raw);

#else

    (void) algorithm;
    throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

void PKCS12Key::PKCS12KeyImpl::SignUpdate(const Buffer& data) {
    auto input_stream = data.ToInputStream();
    SignUpdate(input_stream, data.size());
}

void PKCS12Key::PKCS12KeyImpl::SignUpdate(IInputStreamPtr data, types::stream_size length) {

#if defined(VANILLAPDF_HAVE_OPENSSL)

    types::stream_size read_total = 0;

    Buffer buffer(constant::BUFFER_SIZE);
    for (;;) {
        if (read_total == length) {
            break;
        }

        types::stream_size block_size = std::min<types::stream_size>(length - read_total, constant::BUFFER_SIZE);
        types::size_type block_size_converted = ValueConvertUtils::SafeConvert<types::size_type>(block_size);
        types::stream_size read = data->Read(buffer, block_size_converted);
        int read_converted = ValueConvertUtils::SafeConvert<int>(read);

        int written = BIO_write(m_data_bio.get(), buffer.data(), read_converted);
        if (written != read_converted) {
            throw CryptoErrorException("Could not write data");
        }

        read_total = read_total + read;
    }

#else

    (void) data;
    throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

BufferPtr PKCS12Key::PKCS12KeyImpl::SignFinal() {

#if defined(VANILLAPDF_HAVE_OPENSSL)

    // For a writable memory BIO, BIO_reset() clears the buffer rather than seeking to the
    // beginning. To give CMS_final() a readable view of the accumulated data, we use
    // BIO_get_mem_ptr() to obtain the underlying BUF_MEM and wrap it in a read-only BIO.
    BUF_MEM* mem_buf = nullptr;
    BIO_get_mem_ptr(m_data_bio.get(), &mem_buf);
    if (!mem_buf) {
        throw CryptoErrorException("Could not get memory buffer from data BIO");
    }

    BIO* read_bio = BIO_new_mem_buf(mem_buf->data, static_cast<int>(mem_buf->length));
    if (!read_bio) {
        throw CryptoErrorException("Could not create read BIO, " + CryptoUtils::GetLastOpensslError());
    }
    SCOPE_GUARD([read_bio]() { BIO_free(read_bio); });

    // Finalize: adds content-type and message-digest signed attributes,
    // then performs the actual signing operation
    int finalized = CMS_final(m_cms.get(), read_bio, nullptr, CMS_DETACHED | CMS_BINARY);
    if (finalized != 1) {
        throw CryptoErrorException("Could not finalize CMS, " + CryptoUtils::GetLastOpensslError());
    }

    int length = i2d_CMS_ContentInfo(m_cms.get(), nullptr);
    if (length < 0) {
        throw CryptoErrorException("Could not get CMS size, " + CryptoUtils::GetLastOpensslError());
    }

    BufferPtr result = make_deferred_container<Buffer>(length);
    auto data_pointer = (unsigned char*) result->data();
    int converted = i2d_CMS_ContentInfo(m_cms.get(), &data_pointer);
    if (converted < 0) {
        throw CryptoErrorException("Could not convert CMS, " + CryptoUtils::GetLastOpensslError());
    }

    return result;

#else

    throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

void PKCS12Key::PKCS12KeyImpl::SignCleanup() {

#if defined(VANILLAPDF_HAVE_OPENSSL)

    // Reset signing-related resources (unique_ptr handles cleanup automatically)
    m_cms.reset();
    m_data_bio.reset();

#endif
}

BufferPtr PKCS12Key::PKCS12KeyImpl::GetCertificate() const {

#if defined(VANILLAPDF_HAVE_OPENSSL)

    if (!cert) {
        throw CryptoErrorException("No certificate available in PKCS12 key");
    }

    // Convert X509* to DER format (same technique used for additional certs)
    int length = i2d_X509(cert.get(), nullptr);
    if (length < 0) {
        throw CryptoErrorException("Could not get certificate size, " + CryptoUtils::GetLastOpensslError());
    }

    BufferPtr cert_data = make_deferred_container<Buffer>(length);
    auto data_pointer = (unsigned char *) cert_data->data();
    int converted = i2d_X509(cert.get(), &data_pointer);
    if (converted < 0) {
        throw CryptoErrorException("Could not convert certificate to DER, " + CryptoUtils::GetLastOpensslError());
    }

    return cert_data;

#else

    throw NotSupportedException("This library was compiled without OpenSSL support");

#endif

}

} // vanillapdf
