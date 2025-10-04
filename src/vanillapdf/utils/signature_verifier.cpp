#include "precompiled.h"

#include "utils/signature_verifier.h"
#include "utils/misc_utils.h"
#include "utils/exceptions.h"
#include "utils/signature_verification_options.h"

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
void ExtractCertificateChain(PKCS7* p7, SignatureVerificationResult* result) {
    STACK_OF(X509)* certs = PKCS7_get0_signers(p7, nullptr, 0);
    if (!certs) {
        return;
    }

    int cert_count = sk_X509_num(certs);
    for (int i = 0; i < cert_count; i++) {
        X509* cert = sk_X509_value(certs, i);

        // Convert to DER
        int der_len = i2d_X509(cert, nullptr);
        if (der_len < 0) {
            continue;
        }

        BufferPtr cert_buf = make_deferred_container<Buffer>(der_len);
        unsigned char* der_ptr = reinterpret_cast<unsigned char*>(cert_buf->data());
        i2d_X509(cert, &der_ptr);

        if (i == 0) {
            result->SetSignerCertificate(cert_buf);

            // Extract common name
            X509_NAME* subject = X509_get_subject_name(cert);
            char cn[256] = {0};
            X509_NAME_get_text_by_NID(subject, NID_commonName, cn, sizeof(cn));
            result->SetSignerCommonName(std::string(cn));

            // Extract validity dates
            const ASN1_TIME* not_before = X509_get0_notBefore(cert);
            const ASN1_TIME* not_after = X509_get0_notAfter(cert);

            // TODO: Convert ASN1_TIME to DatePtr
            // For now, just set the result
            (void)not_before;
            (void)not_after;
        }

        result->AddCertificateToChain(cert_buf);
    }

    sk_X509_free(certs);
}

// Helper to verify certificate chain
bool VerifyCertificateChain(PKCS7* p7, X509_STORE* store, SignatureVerificationResult* result) {
    X509_STORE_CTX* ctx = X509_STORE_CTX_new();
    if (!ctx) {
        return false;
    }
    SCOPE_GUARD([ctx]() { X509_STORE_CTX_free(ctx); });

    // Get signer certificate
    STACK_OF(X509)* signers = PKCS7_get0_signers(p7, nullptr, 0);
    if (!signers || sk_X509_num(signers) == 0) {
        sk_X509_free(signers);
        return false;
    }

    X509* signer_cert = sk_X509_value(signers, 0);

    // Get certificate chain from PKCS7
    STACK_OF(X509)* cert_chain = p7->d.sign->cert;

    // Initialize verification context
    if (!X509_STORE_CTX_init(ctx, store, signer_cert, cert_chain)) {
        sk_X509_free(signers);
        return false;
    }

    // Verify the chain
    int verify_result = X509_verify_cert(ctx);

    sk_X509_free(signers);

    if (verify_result == 1) {
        result->SetCertificateTrusted(true);
        return true;
    } else {
        result->SetCertificateTrusted(false);
        int error = X509_STORE_CTX_get_error(ctx);
        const char* error_str = X509_verify_cert_error_string(error);

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

        result->SetMessage(std::string("Certificate validation failed: ") + error_str);
        return false;
    }
}

} // anonymous namespace

#endif

SignatureVerificationResultPtr SignatureVerifier::Verify(
    const Buffer& signed_data,
    const Buffer& signature_contents,
    TrustedCertificateStorePtr trusted_store,
    VerificationFlags flags) {

    auto result = make_deferred<SignatureVerificationResult>();

#if defined(VANILLAPDF_HAVE_OPENSSL)

    MiscUtils::InitializeOpenSSL();

    // Parse PKCS7 signature
    const unsigned char* sig_ptr = reinterpret_cast<const unsigned char*>(signature_contents.data());
    PKCS7* p7 = d2i_PKCS7(nullptr, &sig_ptr, static_cast<long>(signature_contents.size()));

    if (!p7) {
        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage("Failed to parse PKCS#7 signature: " + MiscUtils::GetLastOpensslError());
        return result;
    }
    SCOPE_GUARD([p7]() { PKCS7_free(p7); });

    // Extract certificate chain
    ExtractCertificateChain(p7, result.Get());

    // Create BIO from signed data
    BIO* data_bio = BIO_new_mem_buf(signed_data.data(), static_cast<int>(signed_data.size()));
    if (!data_bio) {
        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage("Failed to create BIO from signed data");
        return result;
    }
    SCOPE_GUARD([data_bio]() { BIO_free(data_bio); });

    // Verify the signature cryptographically
    // For detached signatures, we need to verify against the data
    int verify_result = PKCS7_verify(p7, nullptr, nullptr, data_bio, nullptr, PKCS7_DETACHED | PKCS7_NOVERIFY);

    if (verify_result == 1) {
        result->SetSignatureValid(true);
        result->SetDocumentIntact(true);
    } else {
        result->SetSignatureValid(false);
        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage("Signature verification failed: " + MiscUtils::GetLastOpensslError());
        return result;
    }

    // Verify certificate chain if trusted store provided
    X509_STORE* store = nullptr;
    bool store_created = false;

    if (trusted_store) {
        store = static_cast<X509_STORE*>(trusted_store->GetNativeHandle());
    } else {
        // Create default store with system certificates
        store = X509_STORE_new();
        X509_STORE_set_default_paths(store);
        store_created = true;
    }

    if (store) {
        bool chain_valid = VerifyCertificateChain(p7, store, result.Get());

        if (store_created) {
            X509_STORE_free(store);
        }

        if (!chain_valid) {
            // Status already set by VerifyCertificateChain
            return result;
        }
    }

    // Check for weak algorithms if requested
    if (static_cast<int>(flags & VerificationFlags::CheckSigningTime)) {
        // Get digest algorithm
        PKCS7_SIGNER_INFO* si = sk_PKCS7_SIGNER_INFO_value(PKCS7_get_signer_info(p7), 0);
        if (si) {
            const EVP_MD* md = EVP_get_digestbyobj(si->digest_alg->algorithm);
            if (md) {
                int nid = EVP_MD_type(md);
                if (nid == NID_md5 || nid == NID_sha1) {
                    result->SetStatus(SignatureVerificationStatus::WeakAlgorithm);
                    result->SetMessage("Weak digest algorithm detected (MD5 or SHA1)");
                    return result;
                }
            }
        }
    }

    // All checks passed
    result->SetStatus(SignatureVerificationStatus::Valid);
    result->SetMessage("Signature is valid");

#else

    // No OpenSSL support
    (void)signed_data;
    (void)signature_contents;
    (void)trusted_store;
    (void)flags;

    result->SetStatus(SignatureVerificationStatus::Unknown);
    result->SetMessage("Signature verification requires OpenSSL support");

#endif

    return result;
}

} // vanillapdf
