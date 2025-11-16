#include "precompiled.h"

#include "utils/signature_verifier.h"
#include "utils/misc_utils.h"
#include "utils/exceptions.h"
#include "utils/log.h"

#if defined(VANILLAPDF_HAVE_OPENSSL)
#include <openssl/pkcs7.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#endif

namespace vanillapdf {

#if defined(VANILLAPDF_HAVE_OPENSSL)

namespace {

// Helper to extract certificate chain from PKCS7
void ExtractCertificateChain(PKCS7* p7, SignatureVerificationResultPtr& result) {
    STACK_OF(X509)* certs = PKCS7_get0_signers(p7, nullptr, 0);
    if (!certs) {
        spdlog::warn("No signers found in PKCS7 signature");
        return;
    }

    SCOPE_GUARD([certs]() { sk_X509_free(certs); });

    int cert_count = sk_X509_num(certs);

    for (int i = 0; i < cert_count; i++) {
        X509* cert = sk_X509_value(certs, i);
        if (!cert) {
            spdlog::warn("Certificate at index {} is null, skipping", i);
            continue;
        }

        // Convert to DER
        int der_len = i2d_X509(cert, nullptr);
        if (der_len < 0) {
            spdlog::error("Failed to determine DER length for certificate at index {}: {}",
                         i, MiscUtils::GetLastOpensslError());
            continue;
        }

        BufferPtr cert_buf = make_deferred_container<Buffer>(der_len);
        unsigned char* der_ptr = reinterpret_cast<unsigned char*>(cert_buf->data());
        int actual_len = i2d_X509(cert, &der_ptr);
        if (actual_len < 0) {
            spdlog::error("Failed to convert certificate at index {} to DER format: {}",
                         i, MiscUtils::GetLastOpensslError());
            continue;
        }

        if (i == 0) {

            result->SetSignerCertificate(cert_buf);

            // Extract common name from signing certificate
            X509_NAME* subject = X509_get_subject_name(cert);
            if (!subject) {
                spdlog::warn("Common name not found in signing certificate");
                continue;
            }

            // First get the required buffer size
            int cn_len = X509_NAME_get_text_by_NID(subject, NID_commonName, nullptr, 0);
            if (cn_len < 0) {
                spdlog::warn("Failed to get subject name from signing certificate");
                continue;
            }

            // Allocate buffer with exact size needed (+1 for null terminator)
            auto cn_buffer = make_deferred_container<Buffer>(cn_len + 1);
            X509_NAME_get_text_by_NID(subject, NID_commonName, cn_buffer->data(), cn_len + 1);

            // Convert to BufferPtr (UTF-8 encoded)
            result->SetSignerCommonName(cn_buffer);

            // Note: Validity dates (not_before, not_after) will be exposed in a future phase
            // For now, we only extract the common name and certificate data
        }

        result->AddCertificateToChain(cert_buf);
    }
}

// Helper to verify certificate chain
bool VerifyCertificateChain(PKCS7* p7, X509_STORE* store, SignatureVerificationResultPtr& result) {
    X509_STORE_CTX* ctx = X509_STORE_CTX_new();
    if (!ctx) {
        spdlog::error("Failed to create X509_STORE_CTX: {}", MiscUtils::GetLastOpensslError());

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Failed to initialize certificate verification context");
        result->SetCertificateTrusted(false);
        return false;
    }

    SCOPE_GUARD([ctx]() { X509_STORE_CTX_free(ctx); });

    // Get signer certificate
    STACK_OF(X509)* signers = PKCS7_get0_signers(p7, nullptr, 0);
    if (!signers) {
        spdlog::error("Failed to get signers from PKCS7: {}", MiscUtils::GetLastOpensslError());

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Failed to extract signer certificates from signature");
        result->SetCertificateTrusted(false);
        return false;
    }

    SCOPE_GUARD([signers]() { sk_X509_free(signers); });

    if (sk_X509_num(signers) == 0) {
        spdlog::error("No signer certificates found in PKCS7");

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("No signer certificates found in signature");
        result->SetCertificateTrusted(false);
        return false;
    }

    X509* signer_cert = sk_X509_value(signers, 0);
    if (!signer_cert) {
        spdlog::error("Signer certificate is null");

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Invalid signer certificate");
        result->SetCertificateTrusted(false);
        return false;
    }

    // Get certificate chain from PKCS7
    STACK_OF(X509)* cert_chain = p7->d.sign->cert;

    // Initialize verification context
    if (!X509_STORE_CTX_init(ctx, store, signer_cert, cert_chain)) {
        spdlog::error("Failed to initialize X509_STORE_CTX: {}", MiscUtils::GetLastOpensslError());

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Failed to initialize certificate verification context");
        result->SetCertificateTrusted(false);
        return false;
    }

    // Verify the chain
    int verify_result = X509_verify_cert(ctx);

    if (verify_result != 1) {
        int error = X509_STORE_CTX_get_error(ctx);
        const char* error_str = X509_verify_cert_error_string(error);

        spdlog::info("Certificate chain validation failed: {}", error_str);

        // Set appropriate status based on error
        switch (error) {
            case X509_V_ERR_CERT_HAS_EXPIRED:
                result->SetStatus(SignatureVerificationStatus::CertificateExpired);
                break;
            case X509_V_ERR_CERT_NOT_YET_VALID:
                result->SetStatus(SignatureVerificationStatus::CertificateNotYetValid);
                break;
            case X509_V_ERR_CERT_REVOKED:
                result->SetStatus(SignatureVerificationStatus::CertificateRevoked);
                break;
            case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT:
            case X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY:
            case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:
                result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
                break;
            default:
                result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
                break;
        }

        result->SetMessage(fmt::format("Certificate validation failed: {}", error_str));
        result->SetCertificateTrusted(false);

        return false;
    }

    result->SetCertificateTrusted(true);
    return true;
}

} // anonymous namespace

#endif

SignatureVerificationResultPtr SignatureVerifier::Verify(
    const Buffer& signed_data,
    const Buffer& signature_contents,
    TrustedCertificateStorePtr trusted_store,
    SignatureVerificationSettingsPtr settings) {

    // Note: settings parameter reserved for future verification options
    // (e.g., CheckRevocation, RequireTrustedRoot, AllowExpiredCerts, CheckSigningTime)
    // For now, we acknowledge the settings but don't use them yet
    (void)settings;

    auto result = make_deferred<SignatureVerificationResult>();

#if defined(VANILLAPDF_HAVE_OPENSSL)

    MiscUtils::InitializeOpenSSL();

    // Parse PKCS7 signature
    const unsigned char* sig_ptr = reinterpret_cast<const unsigned char*>(signature_contents.data());
    PKCS7* p7 = d2i_PKCS7(nullptr, &sig_ptr, static_cast<long>(signature_contents.size()));

    if (!p7) {
        std::string error = MiscUtils::GetLastOpensslError();
        spdlog::error("Failed to parse PKCS#7 signature: {}", error);

        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage(fmt::format("Failed to parse PKCS#7 signature: {}", error));
        return result;
    }

    SCOPE_GUARD([p7]() { PKCS7_free(p7); });

    // Extract certificate chain
    ExtractCertificateChain(p7, result);

    // Create BIO from signed data
    BIO* data_bio = BIO_new_mem_buf(signed_data.data(), static_cast<int>(signed_data.size()));
    if (!data_bio) {
        std::string error = MiscUtils::GetLastOpensslError();
        spdlog::error("Failed to create BIO from signed data: {}", error);

        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage(fmt::format("Failed to create BIO from signed data: {}", error));
        return result;
    }

    SCOPE_GUARD([data_bio]() { BIO_free(data_bio); });

    // Verify the signature cryptographically
    // For detached signatures, we need to verify against the data
    int verify_result = PKCS7_verify(p7, nullptr, nullptr, data_bio, nullptr, PKCS7_DETACHED | PKCS7_NOVERIFY);

    if (verify_result != 1) {
        std::string error = MiscUtils::GetLastOpensslError();
        spdlog::error("Cryptographic signature verification failed: {}", error);

        result->SetSignatureValid(false);
        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage(fmt::format("Signature verification failed: {}", error));
        return result;
    }

    result->SetSignatureValid(true);
    result->SetDocumentIntact(true);

    // Verify certificate chain with provided trusted store
    if (trusted_store.empty()) {
        spdlog::error("Trusted certificate store is required for signature verification");

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Trusted certificate store is required");
        return result;
    }

    X509_STORE* store = static_cast<X509_STORE*>(trusted_store->GetNativeHandle());
    if (!store) {
        spdlog::error("Trusted store native handle is null");

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Invalid trusted certificate store");
        return result;
    }

    bool chain_valid = VerifyCertificateChain(p7, store, result);
    if (!chain_valid) {
        // Status already set by VerifyCertificateChain
        return result;
    }

    // All checks passed
    result->SetStatus(SignatureVerificationStatus::Valid);
    result->SetMessage("Signature is valid");

#else

    // No OpenSSL support
    (void)signed_data;
    (void)signature_contents;
    (void)trusted_store;

    spdlog::warn("Signature verification not available - OpenSSL support not compiled");
    result->SetStatus(SignatureVerificationStatus::Unknown);
    result->SetMessage("Signature verification requires OpenSSL support");

#endif

    return result;
}

} // vanillapdf
