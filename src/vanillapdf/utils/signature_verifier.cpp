#include "precompiled.h"

#include "utils/signature_verifier.h"
#include "utils/misc_utils.h"
#include "utils/exceptions.h"
#include "utils/log.h"

#include <chrono>

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

// Helper to check if signature uses weak cryptographic algorithms
// Returns true if weak algorithm detected, false otherwise
// Throws GeneralException for logic errors (parsing failures, structure errors)
bool IsWeakAlgorithm(PKCS7* p7, SignatureVerificationResultPtr& result) {
    // Get signer info
    STACK_OF(PKCS7_SIGNER_INFO)* signer_info_stack = PKCS7_get_signer_info(p7);
    if (!signer_info_stack || sk_PKCS7_SIGNER_INFO_num(signer_info_stack) == 0) {
        LOG_ERROR_AND_THROW_GENERAL("No signer info found in PKCS#7 signature");
    }

    PKCS7_SIGNER_INFO* si = sk_PKCS7_SIGNER_INFO_value(signer_info_stack, 0);
    if (!si) {
        LOG_ERROR_AND_THROW_GENERAL("Signer info is null (PKCS#7 structure error)");
    }

    // Check digest algorithm
    X509_ALGOR* digest_alg = si->digest_alg;
    if (digest_alg && digest_alg->algorithm) {
        int nid = OBJ_obj2nid(digest_alg->algorithm);
        const char* alg_name = OBJ_nid2sn(nid);

        // Check for weak digest algorithms (MD5, SHA-1, MD2, MD4)
        if (nid == NID_md5 || nid == NID_sha1 || nid == NID_md2 || nid == NID_md4) {
            spdlog::info("Weak digest algorithm detected: {}", alg_name ? alg_name : "unknown");
            result->SetMessage(fmt::format("Weak digest algorithm: {}", alg_name ? alg_name : "unknown"));
            return true;
        }
    }

    // Check public key size
    STACK_OF(X509)* signers = PKCS7_get0_signers(p7, nullptr, 0);
    if (!signers || sk_X509_num(signers) == 0) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to get signer certificates from PKCS#7");
    }
    SCOPE_GUARD([signers]() { sk_X509_free(signers); });

    X509* signer_cert = sk_X509_value(signers, 0);
    if (!signer_cert) {
        LOG_ERROR_AND_THROW_GENERAL("Signer certificate is null");
    }

    EVP_PKEY* pkey = X509_get0_pubkey(signer_cert);
    if (!pkey) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to get public key from signer certificate");
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

    return false;  // No weak algorithms detected
}

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

// Helper to extract signing time from PKCS7 authenticated attributes
// Returns true if signing time was found and extracted, false if not present (valid per spec)
// Throws GeneralException for logic errors (parsing failures, memory allocation, etc.)
bool ExtractSigningTime(PKCS7* p7, time_t* signing_time) {
    if (!signing_time) {
        LOG_ERROR_AND_THROW_GENERAL("signing_time parameter is null");
    }

    // Get signer info
    STACK_OF(PKCS7_SIGNER_INFO)* signer_info_stack = PKCS7_get_signer_info(p7);
    if (!signer_info_stack || sk_PKCS7_SIGNER_INFO_num(signer_info_stack) == 0) {
        LOG_ERROR_AND_THROW_GENERAL("No signer info found in PKCS#7 signature");
    }

    PKCS7_SIGNER_INFO* si = sk_PKCS7_SIGNER_INFO_value(signer_info_stack, 0);
    if (!si) {
        LOG_ERROR_AND_THROW_GENERAL("Signer info is null (PKCS#7 structure error)");
    }

    // Get authenticated attributes
    STACK_OF(X509_ATTRIBUTE)* auth_attrs = PKCS7_get_signed_attributes(si);
    if (!auth_attrs) {
        // No authenticated attributes - signing time is optional per spec
        spdlog::debug("No authenticated attributes found, signing time not available");
        return false;
    }

    // Look for signing time attribute (NID_pkcs9_signingTime)
    int idx = X509at_get_attr_by_NID(auth_attrs, NID_pkcs9_signingTime, -1);
    if (idx < 0) {
        // Signing time attribute not present - this is valid per spec
        spdlog::debug("Signing time attribute not found in authenticated attributes");
        return false;
    }

    X509_ATTRIBUTE* attr = X509at_get_attr(auth_attrs, idx);
    if (!attr) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to get signing time attribute (X509at_get_attr returned null)");
    }

    // Get the ASN1_TYPE from the attribute
    ASN1_TYPE* asn1_time = X509_ATTRIBUTE_get0_type(attr, 0);
    if (!asn1_time) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to get ASN1_TYPE from signing time attribute");
    }

    // Get ASN1_TIME from ASN1_TYPE
    ASN1_TIME* time_value = nullptr;
    if (asn1_time->type == V_ASN1_UTCTIME || asn1_time->type == V_ASN1_GENERALIZEDTIME) {
        time_value = asn1_time->value.asn1_string;
    }

    if (!time_value) {
        LOG_ERROR_AND_THROW_GENERAL("Signing time has unexpected ASN1 type: {}", asn1_time->type);
    }

    // Use ASN1_TIME_diff to convert to time_t (avoids timezone/DST/leap second complexity)
    // Create epoch time (1970-01-01 00:00:00 UTC)
    ASN1_TIME* epoch = ASN1_TIME_new();
    if (!epoch) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to create ASN1_TIME for epoch: {}", MiscUtils::GetLastOpensslError());
    }
    SCOPE_GUARD([epoch]() { ASN1_TIME_free(epoch); });

    // Set epoch using time_t(0) instead of string
    if (!ASN1_TIME_set(epoch, 0)) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to set epoch time: {}", MiscUtils::GetLastOpensslError());
    }

    // Calculate difference between signing time and epoch
    int days = 0;
    int seconds = 0;
    if (!ASN1_TIME_diff(&days, &seconds, epoch, time_value)) {
        LOG_ERROR_AND_THROW_GENERAL("Failed to calculate time difference: {}", MiscUtils::GetLastOpensslError());
    }

    // Convert to time_t using chrono for clean duration calculation
    auto duration = std::chrono::hours(days * 24) + std::chrono::seconds(seconds);
    *signing_time = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

    spdlog::info("Extracted signing time: {} (days={}, seconds={})", *signing_time, days, seconds);
    return true;
}

// Helper to verify certificate chain
bool VerifyCertificateChain(PKCS7* p7, X509_STORE* store, SignatureVerificationResultPtr& result, SignatureVerificationSettingsPtr settings) {
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

    // Check if signing time validation is requested
    if (!settings.empty() && settings->GetCheckSigningTimeFlag()) {
        time_t signing_time = 0;
        if (!ExtractSigningTime(p7, &signing_time)) {
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

    // Verify the chain
    int verify_result = X509_verify_cert(ctx);

    if (verify_result != 1) {
        int error = X509_STORE_CTX_get_error(ctx);
        const char* error_str = X509_verify_cert_error_string(error);

        spdlog::info("Certificate chain validation failed: {}", error_str);

        // Set appropriate status based on error
        switch (error) {
            case X509_V_ERR_CERT_HAS_EXPIRED:
                // Check if expired certificates are allowed
                if (!settings.empty() && settings->GetAllowExpiredCertsFlag()) {
                    spdlog::info("Certificate has expired but AllowExpiredCertsFlag is enabled, continuing verification");
                    result->SetCertificateTrusted(true);
                    result->SetStatus(SignatureVerificationStatus::Valid);
                    result->SetMessage("Certificate has expired but is allowed by settings");
                    return true;  // Allow verification to proceed
                }
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
                // Check if untrusted roots are allowed
                if (!settings.empty() && settings->GetAllowUntrustedRootFlag()) {
                    spdlog::info("Certificate chain does not terminate in trusted root but AllowUntrustedRootFlag is enabled, continuing");
                    result->SetCertificateTrusted(false);  // Still mark as not trusted, but allow
                    result->SetStatus(SignatureVerificationStatus::Valid);
                    result->SetMessage("Certificate is self-signed but is allowed by settings");
                    return true;  // Allow verification to proceed
                }
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

    auto result = make_deferred<SignatureVerificationResult>();

    // Settings are passed to verification functions for validation behavior control
    // Currently implemented: AllowExpiredCertsFlag, AllowWeakAlgorithmsFlag, AllowUntrustedRootFlag
    // TODO: CheckRevocationFlag, CheckSigningTimeFlag

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

    bool chain_valid = VerifyCertificateChain(p7, store, result, settings);
    if (!chain_valid) {
        // Status already set by VerifyCertificateChain
        return result;
    }

    // Check for weak cryptographic algorithms (MD5, SHA-1, weak key sizes)
    bool has_weak_algorithm = IsWeakAlgorithm(p7, result);
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
