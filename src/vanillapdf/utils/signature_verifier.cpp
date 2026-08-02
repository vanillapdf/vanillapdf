#include "precompiled.h"

#include "utils/signature_verifier.h"
#include "utils/crypto_utils.h"
#include "utils/misc_utils.h"
#include "utils/exceptions.h"
#include "utils/log.h"

#include <chrono>

#if defined(VANILLAPDF_HAVE_OPENSSL)
#include <openssl/cms.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/obj_mac.h>
#endif

namespace vanillapdf {

#if defined(VANILLAPDF_HAVE_OPENSSL)

// Helper to check if signature uses weak cryptographic algorithms.
// Called after CMS_verify() so that signer certs are matched to signer infos.
// Returns true if weak algorithm detected, false otherwise.
// Throws CryptoErrorException for logic errors (parsing failures, structure errors).
bool SignatureVerifier::IsWeakAlgorithm(CMS_ContentInfo* cms, SignatureVerificationResultPtr& result) {
    // Get signer info
    STACK_OF(CMS_SignerInfo)* si_stack = CMS_get0_SignerInfos(cms);
    if (!si_stack || sk_CMS_SignerInfo_num(si_stack) == 0) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "No signer info found in CMS signature");
    }

    CMS_SignerInfo* si = sk_CMS_SignerInfo_value(si_stack, 0);
    if (!si) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Signer info is null (CMS structure error)");
    }

    // Retrieve all fields from SignerInfo in one call (must be after CMS_verify() for signer_cert)
    X509* signer_cert = nullptr;
    X509_ALGOR* digest_alg = nullptr;
    X509_ALGOR* sig_alg = nullptr;
    CMS_SignerInfo_get0_algs(si, nullptr, &signer_cert, &digest_alg, &sig_alg);

    // Check digest algorithm (hash only — this field does NOT contain signature algorithms)
    // Signature algorithms like NID_sha1WithRSAEncryption belong in sig_alg
    if (digest_alg && digest_alg->algorithm) {
        int nid = OBJ_obj2nid(digest_alg->algorithm);
        const char* alg_name = OBJ_nid2sn(nid);

        if (nid == NID_md2 || nid == NID_md4 || nid == NID_md5 || nid == NID_sha1 || nid == NID_md5_sha1) {
            spdlog::info("Weak digest algorithm detected: {}", alg_name ? alg_name : "unknown");
            result->SetMessage(fmt::format("Weak digest algorithm: {}", alg_name ? alg_name : "unknown"));
            return true;
        }
    }

    // Check signature algorithm
    // - MD2/MD4/MD5 based: NID_md2WithRSAEncryption, NID_md4WithRSAEncryption, NID_md5WithRSAEncryption, NID_md5WithRSA
    // - SHA-0 based: NID_shaWithRSAEncryption, NID_dsaWithSHA
    // - SHA-1 based: NID_sha1WithRSAEncryption, NID_sha1WithRSA, NID_dsaWithSHA1, NID_dsaWithSHA1_2, NID_ecdsa_with_SHA1
    // - Deprecated: NID_ripemd160WithRSA, NID_mdc2WithRSA
    if (sig_alg && sig_alg->algorithm) {
        int nid = OBJ_obj2nid(sig_alg->algorithm);
        const char* alg_name = OBJ_nid2sn(nid);

        if (nid == NID_md2WithRSAEncryption || nid == NID_md4WithRSAEncryption ||
            nid == NID_md5WithRSAEncryption || nid == NID_md5WithRSA ||
            nid == NID_shaWithRSAEncryption || nid == NID_dsaWithSHA ||
            nid == NID_sha1WithRSAEncryption || nid == NID_sha1WithRSA ||
            nid == NID_dsaWithSHA1 || nid == NID_dsaWithSHA1_2 || nid == NID_ecdsa_with_SHA1 ||
            nid == NID_ripemd160WithRSA || nid == NID_mdc2WithRSA) {
            spdlog::info("Weak signature algorithm detected: {}", alg_name ? alg_name : "unknown");
            result->SetMessage(fmt::format("Weak signature algorithm: {}", alg_name ? alg_name : "unknown"));
            return true;
        }
    }

    // Check public key size using the signer certificate matched by CMS_verify()
    if (!signer_cert) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Signer certificate is null after CMS_verify");
    }

    EVP_PKEY* pkey = X509_get0_pubkey(signer_cert);
    if (!pkey) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Failed to get public key from signer certificate");
    }

    int key_type = EVP_PKEY_base_id(pkey);
    int key_bits = EVP_PKEY_bits(pkey);

    // Check for weak RSA key sizes (< 2048 bits)
    if (key_type == EVP_PKEY_RSA && key_bits < 2048) {
        spdlog::info("Weak RSA key size detected: {} bits", key_bits);
        result->SetMessage(fmt::format("Weak RSA key size: {} bits (minimum 2048)", key_bits));
        return true;
    }

    // Check for weak DSA key sizes (< 2048 bits)
    if (key_type == EVP_PKEY_DSA && key_bits < 2048) {
        spdlog::info("Weak DSA key size detected: {} bits", key_bits);
        result->SetMessage(fmt::format("Weak DSA key size: {} bits (minimum 2048)", key_bits));
        return true;
    }

    // Check for weak EC key sizes (< 256 bits)
    if (key_type == EVP_PKEY_EC && key_bits < 256) {
        spdlog::info("Weak EC key size detected: {} bits", key_bits);
        result->SetMessage(fmt::format("Weak EC key size: {} bits (minimum 256)", key_bits));
        return true;
    }

    return false;  // No weak algorithms detected
}

// Helper to extract certificate chain from CMS signature.
// Must be called after CMS_verify() so that signer certs are matched to signer infos.
void SignatureVerifier::ExtractCertificateChain(CMS_ContentInfo* cms, SignatureVerificationResultPtr& result) {
    // Get the first signer's certificate (matched by CMS_verify())
    STACK_OF(CMS_SignerInfo)* si_stack = CMS_get0_SignerInfos(cms);
    if (!si_stack || sk_CMS_SignerInfo_num(si_stack) == 0) {
        spdlog::warn("No signers found in CMS signature");
        return;
    }

    CMS_SignerInfo* si = sk_CMS_SignerInfo_value(si_stack, 0);
    X509* signer_cert = nullptr;
    CMS_SignerInfo_get0_algs(si, nullptr, &signer_cert, nullptr, nullptr);

    if (!signer_cert) {
        spdlog::warn("Signer certificate not matched after CMS_verify");
    } else {
        // Convert signer certificate to DER and record it
        int der_len = i2d_X509(signer_cert, nullptr);
        if (der_len < 0) {
            spdlog::warn("Failed to determine DER length for signer certificate");
        } else {
            BufferPtr cert_buf = make_deferred_container<Buffer>(der_len);
            unsigned char* der_ptr = reinterpret_cast<unsigned char*>(cert_buf->data());
            if (i2d_X509(signer_cert, &der_ptr) < 0) {
                spdlog::warn("Failed to convert signer certificate to DER format");
            } else {
                result->SetSignerCertificate(cert_buf);
                result->AddCertificateToChain(cert_buf);
            }
        }

        // Extract common name from signing certificate
        // Note: Validity dates (not_before, not_after) will be exposed in a future phase
        X509_NAME* subject = X509_get_subject_name(signer_cert);
        if (!subject) {
            spdlog::warn("Common name not found in signing certificate");
        } else {
            // First get the required buffer size, then allocate (+1 for null terminator)
            int cn_len = X509_NAME_get_text_by_NID(subject, NID_commonName, nullptr, 0);
            if (cn_len < 0) {
                spdlog::warn("Failed to get subject name from signing certificate");
            } else {
                auto cn_buffer = make_deferred_container<Buffer>(cn_len + 1);
                X509_NAME_get_text_by_NID(subject, NID_commonName, cn_buffer->data(), cn_len + 1);

                // OpenSSL writes a terminating null, which is not part of the value.
                // Trim it so the buffer size is the length of the common name, the
                // same meaning it has for every other buffer the library hands out.
                cn_buffer->resize(cn_len);

                result->SetSignerCommonName(cn_buffer);
            }
        }
    }

    // Add all embedded certificates (intermediate CAs) to the chain.
    // CMS_get1_certs() returns a new STACK with new references; caller must free with sk_X509_pop_free.
    STACK_OF(X509)* embedded_certs = CMS_get1_certs(cms);
    if (!embedded_certs) {
        return;
    }
    SCOPE_GUARD([embedded_certs]() { sk_X509_pop_free(embedded_certs, X509_free); });

    int cert_count = sk_X509_num(embedded_certs);
    for (int i = 0; i < cert_count; i++) {
        X509* cert = sk_X509_value(embedded_certs, i);
        if (!cert) {
            spdlog::warn("Certificate at index {} is null, skipping", i);
            continue;
        }

        // Skip the signer cert — already added above
        if (signer_cert && X509_cmp(cert, signer_cert) == 0) {
            continue;
        }

        // Convert to DER
        int der_len = i2d_X509(cert, nullptr);
        if (der_len < 0) {
            spdlog::error("Failed to determine DER length for certificate at index {}: {}",
                         i, CryptoUtils::GetLastOpensslError());
            continue;
        }

        BufferPtr cert_buf = make_deferred_container<Buffer>(der_len);
        unsigned char* der_ptr = reinterpret_cast<unsigned char*>(cert_buf->data());
        int actual_len = i2d_X509(cert, &der_ptr);
        if (actual_len < 0) {
            spdlog::error("Failed to convert certificate at index {} to DER format: {}",
                         i, CryptoUtils::GetLastOpensslError());
            continue;
        }

        result->AddCertificateToChain(cert_buf);
    }
}

// Helper to extract signing time from CMS authenticated attributes.
// Returns true if signing time was found and extracted, false if not present (valid per spec).
// Throws CryptoErrorException for logic errors (parsing failures, memory allocation, etc.)
bool SignatureVerifier::ExtractSigningTime(CMS_ContentInfo* cms, time_t* signing_time) {
    if (!signing_time) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "signing_time parameter is null");
    }

    // Get signer info
    STACK_OF(CMS_SignerInfo)* si_stack = CMS_get0_SignerInfos(cms);
    if (!si_stack || sk_CMS_SignerInfo_num(si_stack) == 0) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "No signer info found in CMS signature");
    }

    CMS_SignerInfo* si = sk_CMS_SignerInfo_value(si_stack, 0);
    if (!si) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Signer info is null (CMS structure error)");
    }

    // Look for signing time attribute (NID_pkcs9_signingTime) in signed attributes
    int idx = CMS_signed_get_attr_by_NID(si, NID_pkcs9_signingTime, -1);
    if (idx < 0) {
        // Signing time attribute not present - this is valid per spec
        spdlog::debug("Signing time attribute not found in authenticated attributes");
        return false;
    }

    X509_ATTRIBUTE* attr = CMS_signed_get_attr(si, idx);
    if (!attr) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Failed to get signing time attribute (CMS_signed_get_attr returned null)");
    }

    // Get the ASN1_TYPE from the attribute
    ASN1_TYPE* asn1_time = X509_ATTRIBUTE_get0_type(attr, 0);
    if (!asn1_time) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Failed to get ASN1_TYPE from signing time attribute");
    }

    // Get ASN1_TIME from ASN1_TYPE
    ASN1_TIME* time_value = nullptr;
    if (asn1_time->type == V_ASN1_UTCTIME || asn1_time->type == V_ASN1_GENERALIZEDTIME) {
        time_value = asn1_time->value.asn1_string;
    }

    if (!time_value) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Signing time has unexpected ASN1 type: {}", asn1_time->type);
    }

    // Use ASN1_TIME_diff to convert to time_t (avoids timezone/DST/leap second complexity)
    // Create epoch time (1970-01-01 00:00:00 UTC)
    ASN1_TIME* epoch = ASN1_TIME_new();
    if (!epoch) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Failed to create ASN1_TIME for epoch: {}", CryptoUtils::GetLastOpensslError());
    }
    SCOPE_GUARD([epoch]() { ASN1_TIME_free(epoch); });

    // Set epoch using time_t(0) instead of string
    if (!ASN1_TIME_set(epoch, 0)) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Failed to set epoch time: {}", CryptoUtils::GetLastOpensslError());
    }

    // Calculate difference between signing time and epoch
    int days = 0;
    int seconds = 0;
    if (!ASN1_TIME_diff(&days, &seconds, epoch, time_value)) {
        LOG_ERROR_AND_THROW(CryptoErrorException, "Failed to calculate time difference: {}", CryptoUtils::GetLastOpensslError());
    }

    // Convert to time_t using chrono for clean duration calculation
    auto duration = std::chrono::hours(days * 24) + std::chrono::seconds(seconds);
    *signing_time = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    spdlog::info("Extracted signing time: {} (days={}, seconds={})", *signing_time, days, seconds);
    return true;
}

// Helper to verify certificate chain.
// Must be called after CMS_verify() so that signer certs are matched to signer infos.
bool SignatureVerifier::VerifyCertificateChain(CMS_ContentInfo* cms, X509_STORE* store,
                                               SignatureVerificationResultPtr& result,
                                               SignatureVerificationSettingsPtr settings) {
    X509_STORE_CTX* ctx = X509_STORE_CTX_new();
    if (!ctx) {
        spdlog::error("Failed to create X509_STORE_CTX: {}", CryptoUtils::GetLastOpensslError());

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Failed to initialize certificate verification context");
        result->SetCertificateTrusted(false);
        return false;
    }

    SCOPE_GUARD([ctx]() { X509_STORE_CTX_free(ctx); });

    // Get signer certificate (matched by CMS_verify())
    STACK_OF(CMS_SignerInfo)* si_stack = CMS_get0_SignerInfos(cms);
    if (!si_stack || sk_CMS_SignerInfo_num(si_stack) == 0) {
        spdlog::error("No signer info found in CMS signature");

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Failed to extract signer certificates from signature");
        result->SetCertificateTrusted(false);
        return false;
    }

    CMS_SignerInfo* si = sk_CMS_SignerInfo_value(si_stack, 0);
    X509* signer_cert = nullptr;
    CMS_SignerInfo_get0_algs(si, nullptr, &signer_cert, nullptr, nullptr);

    if (!signer_cert) {
        spdlog::error("Signer certificate is null after CMS_verify");

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Invalid signer certificate");
        result->SetCertificateTrusted(false);
        return false;
    }

    // Get certificate chain from CMS (untrusted certs for chain building).
    // CMS_get1_certs() returns a new STACK with new references; caller must free with sk_X509_pop_free.
    STACK_OF(X509)* cert_chain = CMS_get1_certs(cms);
    SCOPE_GUARD([cert_chain]() { if (cert_chain) sk_X509_pop_free(cert_chain, X509_free); });

    // Initialize verification context
    if (!X509_STORE_CTX_init(ctx, store, signer_cert, cert_chain)) {
        spdlog::error("Failed to initialize X509_STORE_CTX: {}", CryptoUtils::GetLastOpensslError());

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Failed to initialize certificate verification context");
        result->SetCertificateTrusted(false);
        return false;
    }

    // Check if signing time validation is requested
    if (!settings.empty() && settings->GetCheckSigningTimeFlag()) {
        time_t signing_time = 0;
        if (!SignatureVerifier::ExtractSigningTime(cms, &signing_time)) {
            // CheckSigningTimeFlag is set but signing time is not present in signature
            spdlog::error("CheckSigningTimeFlag is enabled but signing time not found in signature");
            result->SetStatus(SignatureVerificationStatus::Invalid);
            result->SetMessage("Signing time validation requested but signing time not found in signature");
            result->SetCertificateTrusted(false);
            return false;
        }

        // Validate certificate chain at signing time instead of current time
        X509_STORE_CTX_set_time(ctx, 0, signing_time);
        spdlog::info("Validating certificate chain at signing time: {}", signing_time);
    }

    // Check if certificate validation should be skipped
    if (!settings.empty() && settings->GetSkipCertificateValidation()) {
        spdlog::info("SkipCertificateValidation is enabled, bypassing X509 chain validation");
        result->SetCertificateTrusted(false);  // Mark as not trusted since validation was skipped
        return true;  // Allow verification to proceed without chain validation
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
            case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
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

#endif

SignatureVerificationResultPtr SignatureVerifier::Verify(
    const Buffer& signed_data,
    const Buffer& signature_contents,
    TrustedCertificateStorePtr trusted_store,
    SignatureVerificationSettingsPtr settings) {

    auto result = make_deferred<SignatureVerificationResult>();

    // Settings are passed to verification functions for validation behavior control
    // Currently implemented: SkipCertificateValidation, AllowWeakAlgorithmsFlag, CheckSigningTimeFlag
    // TODO: CheckRevocationFlag - CRL/OCSP implementation (https://github.com/vanillapdf/vanillapdf/issues/157)

#if defined(VANILLAPDF_HAVE_OPENSSL)

    CryptoUtils::InitializeOpenSSL();

    // Parse CMS signature
    const unsigned char* sig_ptr = reinterpret_cast<const unsigned char*>(signature_contents.data());
    CMS_ContentInfo* cms = d2i_CMS_ContentInfo(nullptr, &sig_ptr, static_cast<long>(signature_contents.size()));

    if (!cms) {
        std::string error = CryptoUtils::GetLastOpensslError();
        spdlog::error("Failed to parse CMS signature: {}", error);

        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage(fmt::format("Failed to parse CMS signature: {}", error));
        return result;
    }

    SCOPE_GUARD([cms]() { CMS_ContentInfo_free(cms); });

    // Create BIO from signed data
    BIO* data_bio = BIO_new_mem_buf(signed_data.data(), static_cast<int>(signed_data.size()));
    if (!data_bio) {
        std::string error = CryptoUtils::GetLastOpensslError();
        spdlog::error("Failed to create BIO from signed data: {}", error);

        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage(fmt::format("Failed to create BIO from signed data: {}", error));
        return result;
    }

    SCOPE_GUARD([data_bio]() { BIO_free(data_bio); });

    // Verify the signature cryptographically (certificate chain validated separately below).
    // CMS_DETACHED: content is supplied separately (PDF detached signatures).
    // CMS_BINARY: no MIME canonicalization (required for binary PDF data).
    // CMS_NOVERIFY: skip certificate chain validation (done separately below).
    int verify_result = CMS_verify(cms, nullptr, nullptr, data_bio, nullptr,
                                   CMS_DETACHED | CMS_BINARY | CMS_NOVERIFY);

    if (verify_result != 1) {
        std::string error = CryptoUtils::GetLastOpensslError();
        spdlog::error("Cryptographic signature verification failed: {}", error);

        result->SetSignatureValid(false);
        result->SetStatus(SignatureVerificationStatus::Invalid);
        result->SetMessage(fmt::format("Signature verification failed: {}", error));
        return result;
    }

    result->SetSignatureValid(true);
    result->SetDocumentIntact(true);

    // Extract certificate chain (must be after CMS_verify() so signer cert is matched to signer info)
    SignatureVerifier::ExtractCertificateChain(cms, result);

    // Verify certificate chain with provided trusted store
    if (trusted_store.empty()) {
        spdlog::error("Trusted certificate store is required for signature verification");

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Trusted certificate store is required");
        return result;
    }

    X509_STORE* store = trusted_store->GetNativeHandle();
    if (!store) {
        spdlog::error("Trusted store native handle is null");

        result->SetStatus(SignatureVerificationStatus::CertificateUntrusted);
        result->SetMessage("Invalid trusted certificate store");
        return result;
    }

    bool chain_valid = SignatureVerifier::VerifyCertificateChain(cms, store, result, settings);
    if (!chain_valid) {
        // Status already set by VerifyCertificateChain
        return result;
    }

    // Check for weak cryptographic algorithms (MD5, SHA-1, weak key sizes)
    bool has_weak_algorithm = SignatureVerifier::IsWeakAlgorithm(cms, result);
    if (has_weak_algorithm) {
        // Check if weak algorithms are allowed by settings
        if (settings.empty() || !settings->GetAllowWeakAlgorithmsFlag()) {
            spdlog::info("Weak algorithm detected and AllowWeakAlgorithmsFlag is disabled");
            result->SetStatus(SignatureVerificationStatus::WeakAlgorithm);
            // Message already set by IsWeakAlgorithm
            return result;
        }
        spdlog::info("Weak algorithm detected but AllowWeakAlgorithmsFlag is enabled, continuing");
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
