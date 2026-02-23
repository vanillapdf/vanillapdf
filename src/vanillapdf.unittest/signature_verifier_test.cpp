#include "unittest.h"
#include "test_data.h"
#include "test_certificates.h"
#include "handle_guard.h"

#if defined(VANILLAPDF_HAVE_OPENSSL)
#include <openssl/opensslv.h>
#endif

namespace signature_verification {

// Test certificate info for parameterized tests
struct CertificateTestInfo {
    const unsigned char* data;
    size_t size;
    const char* name;
    const char* password;
    const char* expected_cn;
};

// Define test certificates using the CMake-generated header
// All test certificates use password "test"
// CN values match the certificate Subject CN field
static const CertificateTestInfo TEST_CERTIFICATES[] = {
    {TEST_2KDSA_SHA256_CERTIFICATE, TEST_2KDSA_SHA256_CERTIFICATE_SIZE, "2kDSA_SHA256", "test", "Test_2kDSA_SHA256"},
    {TEST_4KRSA_SHA3_512_CERTIFICATE, TEST_4KRSA_SHA3_512_CERTIFICATE_SIZE, "4kRSA_SHA3_512", "test", "Test_4kRSA_SHA3_512"},
    {TEST_EC384_SHA512_CERTIFICATE, TEST_EC384_SHA512_CERTIFICATE_SIZE, "EC384_SHA512", "test", "Test_EC384_SHA512"},

    // EdDSA CMS signing requires OpenSSL >= 3.2 (eddsa_digest_signverify_init
    // rejects any non-NULL digest; nullptr support in CMS was added in 3.2).
#if defined(VANILLAPDF_HAVE_OPENSSL) && OPENSSL_VERSION_NUMBER >= 0x30200000L
    {TEST_ED25519_CERTIFICATE, TEST_ED25519_CERTIFICATE_SIZE, "ED25519", "test", "Test_ED25519"},
    {TEST_ED448_CERTIFICATE, TEST_ED448_CERTIFICATE_SIZE, "ED448", "test", "Test_ED448"},
#endif
};

// TrustedCertificateStore Tests

TEST(TrustedCertificateStore, CreateRelease) {
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;

    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(store.get(), nullptr);
}

TEST(TrustedCertificateStore, NullCheck) {
    EXPECT_EQ(TrustedCertificateStore_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TrustedCertificateStore_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TrustedCertificateStore, AddCertificateFromPEM_NullChecks) {
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;
    HandleGuard<BufferHandle, Buffer_Release> pem_buffer;

    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Create(pem_buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    // Null store check
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromPEM(nullptr, pem_buffer),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null buffer check
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromPEM(store, nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TrustedCertificateStore, AddCertificateFromDER_NullChecks) {
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;
    HandleGuard<BufferHandle, Buffer_Release> der_buffer;

    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Create(der_buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    // Null store check
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromDER(nullptr, der_buffer),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null buffer check
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromDER(store, nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TrustedCertificateStore, LoadFromDirectory_NullChecks) {
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;

    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);

    // Null store check
    EXPECT_EQ(TrustedCertificateStore_LoadFromDirectory(nullptr, "some_path"),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null path check
    EXPECT_EQ(TrustedCertificateStore_LoadFromDirectory(store, nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TrustedCertificateStore, LoadSystemDefaults_NullCheck) {
    EXPECT_EQ(TrustedCertificateStore_LoadSystemDefaults(nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TrustedCertificateStore, ToUnknown_FromUnknown) {
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;
    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store_back;

    // Create a TrustedCertificateStore
    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(store.get(), nullptr);

    // Convert to IUnknown
    ASSERT_EQ(TrustedCertificateStore_ToUnknown(store, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    // Convert back to TrustedCertificateStore
    ASSERT_EQ(TrustedCertificateStore_FromUnknown(unknown, store_back.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(store_back.get(), nullptr);

    // The pointers should be the same (same underlying object)
    EXPECT_EQ(store.get(), store_back.get());

    // Cleanup - each conversion (ToUnknown/FromUnknown) adds a reference,
    // so we need to release: unknown (from ToUnknown), store_back (from FromUnknown),
    // and store (from original Create call)
}

TEST(TrustedCertificateStore, ToUnknown_NullChecks) {
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;
    IUnknownHandle* unknown = nullptr;

    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);

    // Null handle check
    EXPECT_EQ(TrustedCertificateStore_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null result check
    EXPECT_EQ(TrustedCertificateStore_ToUnknown(store, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TrustedCertificateStore, FromUnknown_NullChecks) {
    TrustedCertificateStoreHandle* store = nullptr;

    // Null handle check
    EXPECT_EQ(TrustedCertificateStore_FromUnknown(nullptr, &store), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// SignatureVerifier Tests

TEST(SignatureVerifier, Verify_NullChecks) {
    HandleGuard<BufferHandle, Buffer_Release> signed_data;
    HandleGuard<BufferHandle, Buffer_Release> signature_contents;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;
    SignatureVerificationResultHandle* result = nullptr;

    ASSERT_EQ(Buffer_Create(signed_data.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Create(signature_contents.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);

    // Null signed_data check
    EXPECT_EQ(SignatureVerifier_Verify(nullptr, signature_contents, store,
              nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null signature_contents check
    EXPECT_EQ(SignatureVerifier_Verify(signed_data, nullptr, store,
              nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null store check
    EXPECT_EQ(SignatureVerifier_Verify(signed_data, signature_contents, nullptr,
              nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null result check
    EXPECT_EQ(SignatureVerifier_Verify(signed_data, signature_contents, store,
              nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// SignatureVerificationResult Tests

TEST(SignatureVerificationResult, GetStatus_NullChecks) {
    SignatureVerificationStatusType status = SignatureStatus_Undefined;

    EXPECT_EQ(SignatureVerificationResult_GetStatus(nullptr, &status),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, GetMessage_NullChecks) {
    BufferHandle* message = nullptr;

    EXPECT_EQ(SignatureVerificationResult_GetMessage(nullptr, &message),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, IsSignatureValid_NullChecks) {
    boolean_type is_valid = VANILLAPDF_RV_FALSE;

    EXPECT_EQ(SignatureVerificationResult_IsSignatureValid(nullptr, &is_valid),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, IsDocumentIntact_NullChecks) {
    boolean_type is_intact = VANILLAPDF_RV_FALSE;

    EXPECT_EQ(SignatureVerificationResult_IsDocumentIntact(nullptr, &is_intact),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, IsCertificateTrusted_NullChecks) {
    boolean_type is_trusted = VANILLAPDF_RV_FALSE;

    EXPECT_EQ(SignatureVerificationResult_IsCertificateTrusted(nullptr, &is_trusted),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, GetSignerCertificate_NullChecks) {
    BufferHandle* cert = nullptr;

    EXPECT_EQ(SignatureVerificationResult_GetSignerCertificate(nullptr, &cert),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, GetSignerCommonName_NullChecks) {
    BufferHandle* common_name = nullptr;

    EXPECT_EQ(SignatureVerificationResult_GetSignerCommonName(nullptr, &common_name),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, GetCertificateChainCount_NullChecks) {
    size_type count = 0;

    EXPECT_EQ(SignatureVerificationResult_GetCertificateChainCount(nullptr, &count),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, GetCertificateChainAt_NullChecks) {
    BufferHandle* cert = nullptr;

    EXPECT_EQ(SignatureVerificationResult_GetCertificateChainAt(nullptr, 0, &cert),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, Release_NullCheck) {
    EXPECT_EQ(SignatureVerificationResult_Release(nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, ToUnknown_FromUnknown) {
    // Create a SignatureVerificationResult by performing a signature verification
    const char* test_message = "Test message for ToUnknown/FromUnknown";
    const size_t test_message_len = strlen(test_message);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<BufferHandle, Buffer_Release> message_buffer;
    HandleGuard<BufferHandle, Buffer_Release> signature_buffer;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Step 1: Create signing key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key.get(), nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key.get(), nullptr);

    // Step 2: Create signature
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(test_message, test_message_len, message_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignFinal(signing_key, signature_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key), VANILLAPDF_ERROR_SUCCESS);

    // Step 3: Create trust store
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store.get(), nullptr);

    // Step 4: Verify signature to get a SignatureVerificationResult
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              nullptr, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    // Step 5: Test ToUnknown/FromUnknown conversion
    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> result_back;

    // Convert to IUnknown
    ASSERT_EQ(SignatureVerificationResult_ToUnknown(verify_result, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    // Convert back to SignatureVerificationResult
    ASSERT_EQ(SignatureVerificationResult_FromUnknown(unknown, result_back.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(result_back.get(), nullptr);

    // The pointers should be the same (same underlying object)
    EXPECT_EQ(verify_result.get(), result_back.get());

    // Cleanup - each conversion (ToUnknown/FromUnknown) adds a reference,
    // so we need to release: unknown (from ToUnknown), result_back (from FromUnknown),
    // and verify_result (from original Verify call)
}

TEST(SignatureVerificationResult, ToUnknown_NullChecks) {
    // Create a SignatureVerificationResult for null checks
    const char* test_message = "Test message for null checks";
    const size_t test_message_len = strlen(test_message);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<BufferHandle, Buffer_Release> message_buffer;
    HandleGuard<BufferHandle, Buffer_Release> signature_buffer;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Create a verification result
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_CreateFromData(test_message, test_message_len, message_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SigningKey_SignFinal(signing_key, signature_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SigningKey_SignCleanup(signing_key), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              nullptr, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    IUnknownHandle* unknown = nullptr;

    // Null handle check
    EXPECT_EQ(SignatureVerificationResult_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null result check
    EXPECT_EQ(SignatureVerificationResult_ToUnknown(verify_result, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationResult, FromUnknown_NullChecks) {
    SignatureVerificationResultHandle* result = nullptr;

    // Null handle check
    EXPECT_EQ(SignatureVerificationResult_FromUnknown(nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// SignatureVerificationSettings Tests

TEST(SignatureVerificationSettings, CreateAndGetDefaults) {
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;

    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings.get(), nullptr);

    // Check default values (should all be false/disabled)
    boolean_type skip_cert_validation = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationSettings_GetSkipCertificateValidation(settings, &skip_cert_validation),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(skip_cert_validation, VANILLAPDF_RV_FALSE);

    boolean_type check_signing_time = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationSettings_GetCheckSigningTimeFlag(settings, &check_signing_time),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(check_signing_time, VANILLAPDF_RV_FALSE);

    boolean_type allow_weak = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationSettings_GetAllowWeakAlgorithmsFlag(settings, &allow_weak),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(allow_weak, VANILLAPDF_RV_FALSE);
}

TEST(SignatureVerificationSettings, SetAndGetSkipCertificateValidation) {
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;

    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings.get(), nullptr);

    // Set to true
    ASSERT_EQ(SignatureVerificationSettings_SetSkipCertificateValidation(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    // Verify it was set
    boolean_type skip_cert_validation = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationSettings_GetSkipCertificateValidation(settings, &skip_cert_validation),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(skip_cert_validation, VANILLAPDF_RV_TRUE);

    // Set back to false
    ASSERT_EQ(SignatureVerificationSettings_SetSkipCertificateValidation(settings, VANILLAPDF_RV_FALSE),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SignatureVerificationSettings_GetSkipCertificateValidation(settings, &skip_cert_validation),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(skip_cert_validation, VANILLAPDF_RV_FALSE);
}

TEST(SignatureVerifier, SkipCertificateValidation_ExpiredCert_Integration) {
    // Note: EXPIRED_CERTIFICATE is an expired certificate
    // Test that SkipCertificateValidation allows verification despite expiration
    const char* test_message = "Test message for expired cert verification";
    const size_t test_message_len = strlen(test_message);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<BufferHandle, Buffer_Release> message_buffer;
    HandleGuard<BufferHandle, Buffer_Release> signature_buffer;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Step 1: Create signing key from expired certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(EXPIRED_CERTIFICATE),
                                     EXPIRED_CERTIFICATE_SIZE, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key.get(), nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key.get(), nullptr);

    // Step 2: Create signature
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(test_message),
                                     test_message_len, message_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignFinal(signing_key, signature_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 3: Create trust store and add the signer certificate
    // (SIGNING_CERTIFICATE is self-signed, so we add it as a trusted root to isolate
    // expiration testing from trust issues)
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store.get(), nullptr);

    // Get certificate from the PKCS12 and add to trust store
    {
        HandleGuard<BufferHandle, Buffer_Release> cert_buffer;
        ASSERT_EQ(PKCS12Key_GetCertificate(pkcs12_key, cert_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(cert_buffer.get(), nullptr);

        ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, cert_buffer),
                  VANILLAPDF_ERROR_SUCCESS);
    }

    // Step 4: Verify with default settings (SkipCertificateValidation disabled)
    // Should fail with CertificateExpired status
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              nullptr, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // Must explicitly fail with CertificateExpired (not just any failure)
    EXPECT_EQ(status, SignatureStatus_CertificateExpired);

    boolean_type is_cert_trusted = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationResult_IsCertificateTrusted(verify_result, &is_cert_trusted),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_FALSE);  // Not trusted due to expiration

    verify_result = HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release>();

    // Step 5: Create settings with SkipCertificateValidation enabled
    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings.get(), nullptr);

    ASSERT_EQ(SignatureVerificationSettings_SetSkipCertificateValidation(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 6: Verify again with SkipCertificateValidation enabled
    // Should now succeed since certificate validation is skipped
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              settings, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // With SkipCertificateValidation, certificate errors are bypassed - must be Valid
    EXPECT_EQ(status, SignatureStatus_Valid);

    boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);

    is_cert_trusted = VANILLAPDF_RV_FALSE;  // Reuse from Step 4
    ASSERT_EQ(SignatureVerificationResult_IsCertificateTrusted(verify_result, &is_cert_trusted),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_FALSE);  // Still not trusted since validation was skipped
}

TEST(SignatureVerifier, SkipCertificateValidation_UntrustedRoot_Integration) {
    // Test self-signed certificate validation with SkipCertificateValidation
    const char* test_message = "Test message for self-signed cert verification";
    const size_t test_message_len = strlen(test_message);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<BufferHandle, Buffer_Release> message_buffer;
    HandleGuard<BufferHandle, Buffer_Release> signature_buffer;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Step 1: Create signing key from test certificate (self-signed)
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key.get(), nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key.get(), nullptr);

    // Step 2: Create signature
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(test_message, test_message_len, message_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignFinal(signing_key, signature_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key), VANILLAPDF_ERROR_SUCCESS);

    // Step 3: Create empty trust store (certificate is self-signed, not in store)
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store.get(), nullptr);

    // Step 4: Verify with default settings (SkipCertificateValidation disabled - STRICT by default)
    // Should FAIL with CertificateUntrusted (secure by default)
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              nullptr, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // Should be CertificateUntrusted (strict by default - secure)
    EXPECT_EQ(status, SignatureStatus_CertificateUntrusted);

    boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);  // Cryptographically valid

    boolean_type is_cert_trusted = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationResult_IsCertificateTrusted(verify_result, &is_cert_trusted),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_FALSE);  // Not trusted (self-signed)

    verify_result = HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release>();

    // Step 5: Create settings with SkipCertificateValidation enabled
    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings.get(), nullptr);

    ASSERT_EQ(SignatureVerificationSettings_SetSkipCertificateValidation(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 6: Verify again with SkipCertificateValidation enabled
    // Should now SUCCEED since certificate validation is skipped
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              settings, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // With SkipCertificateValidation, certificate errors are bypassed
    EXPECT_EQ(status, SignatureStatus_Valid);

    is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);

    is_cert_trusted = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationResult_IsCertificateTrusted(verify_result, &is_cert_trusted),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_FALSE);  // Still marked as not trusted
}

// Test CheckSigningTimeFlag - validates certificate chain at signing time
// TODO: Add negative test case - need ability to set custom signing time during signature creation
// Negative case: Sign with future date (beyond cert validity), verify should fail when CheckSigningTimeFlag enabled
TEST(SignatureVerifier, CheckSigningTimeFlag_Integration) {
    const char* test_message = "Test message for signing time verification";
    const size_t test_message_len = strlen(test_message);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<BufferHandle, Buffer_Release> message_buffer;
    HandleGuard<BufferHandle, Buffer_Release> signature_buffer;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Step 1: Create signing key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key.get(), nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key.get(), nullptr);

    // Step 2: Create message buffer
    ASSERT_EQ(Buffer_CreateFromData(test_message, test_message_len, message_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer.get(), nullptr);

    // Step 3: Sign the message (includes signing time attribute with current time)
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SigningKey_SignFinal(signing_key, signature_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer.get(), nullptr);
    ASSERT_EQ(SigningKey_SignCleanup(signing_key), VANILLAPDF_ERROR_SUCCESS);

    // Step 4: Create trust store with self-signed certificate
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store.get(), nullptr);

    {
        HandleGuard<BufferHandle, Buffer_Release> cert_buffer;
        ASSERT_EQ(PKCS12Key_GetCertificate(pkcs12_key, cert_buffer.out()),
                  VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(cert_buffer.get(), nullptr);

        ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, cert_buffer),
                  VANILLAPDF_ERROR_SUCCESS);
    }

    // Step 5: Verify WITHOUT CheckSigningTimeFlag (default behavior)
    // Since certificate is self-signed and added to trust store, it should pass
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              nullptr, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    SignatureVerificationStatusType status;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(status, SignatureStatus_Valid);

    verify_result = HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release>();

    // Step 6: Verify WITH CheckSigningTimeFlag enabled
    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings.get(), nullptr);

    ASSERT_EQ(SignatureVerificationSettings_SetCheckSigningTimeFlag(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              settings, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(status, SignatureStatus_Valid);

    // Both pass since certificate is valid both now and at signing time
    // This test exercises the code path but doesn't test the negative scenario
    // TODO: To fully test, we need ability to sign with custom signing time:
    //   - Sign with date beyond cert validity (e.g., year 2100)
    //   - Without flag: validates at current time (2025) -> cert valid -> PASS
    //   - With flag: validates at signing time (2100) -> cert expired -> FAIL
}

// Parameterized test for weak algorithm detection
class WeakAlgorithmTest : public ::testing::TestWithParam<MessageDigestAlgorithmType> {
};

TEST_P(WeakAlgorithmTest, AllowWeakAlgorithmsFlag_Integration) {
    MessageDigestAlgorithmType weak_algorithm = GetParam();

    const char* test_message = "Test message for weak algorithm verification";
    const size_t test_message_len = strlen(test_message);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<BufferHandle, Buffer_Release> message_buffer;
    HandleGuard<BufferHandle, Buffer_Release> signature_buffer;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Step 1: Create signing key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key.get(), nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key.get(), nullptr);

    // Step 2: Create signature with weak algorithm
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, weak_algorithm),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(test_message, test_message_len, message_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignFinal(signing_key, signature_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key), VANILLAPDF_ERROR_SUCCESS);

    // Step 3: Create trust store and add the signer certificate
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store.get(), nullptr);

    {
        HandleGuard<BufferHandle, Buffer_Release> cert_buffer;
        ASSERT_EQ(PKCS12Key_GetCertificate(pkcs12_key, cert_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(cert_buffer.get(), nullptr);

        ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, cert_buffer),
                  VANILLAPDF_ERROR_SUCCESS);
    }

    // Step 4: Verify with default settings (AllowWeakAlgorithmsFlag disabled)
    // Should fail with WeakAlgorithm status
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              nullptr, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // Must fail with WeakAlgorithm status
    EXPECT_EQ(status, SignatureStatus_WeakAlgorithm);

    boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);  // Cryptographically valid, just weak

    verify_result = HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release>();

    // Step 5: Create settings with AllowWeakAlgorithmsFlag enabled
    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings.get(), nullptr);

    ASSERT_EQ(SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 6: Verify again with AllowWeakAlgorithmsFlag enabled
    // Should now succeed despite weak algorithm
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              settings, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // With AllowWeakAlgorithmsFlag, weak algorithms are allowed - must be Valid
    EXPECT_EQ(status, SignatureStatus_Valid);

    is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);
}

// Instantiate the parameterized test with different weak digest algorithms
// Note: This tests weak digest algorithms (si->digest_alg). The signature algorithm
// (si->digest_enc_alg) is determined by the key type + digest and is also checked.
// MD2 is excluded because it's disabled in modern OpenSSL builds.
INSTANTIATE_TEST_SUITE_P(
    WeakAlgorithms,
    WeakAlgorithmTest,
    ::testing::Values(
        MessageDigestAlgorithmType_MD4,
        MessageDigestAlgorithmType_MD5,
        MessageDigestAlgorithmType_SHA1
    ),
    [](const ::testing::TestParamInfo<MessageDigestAlgorithmType>& info) {
        switch (info.param) {
            case MessageDigestAlgorithmType_MD4:  return "MD4";
            case MessageDigestAlgorithmType_MD5:  return "MD5";
            case MessageDigestAlgorithmType_SHA1: return "SHA1";
            default: return "Unknown";
        }
    }
);

// Real Certificate Tests

TEST(TrustedCertificateStore, AddCertificateFromPEM_Master) {
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;
    HandleGuard<BufferHandle, Buffer_Release> pem_buffer;

    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(store.get(), nullptr);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(MASTER_CERTIFICATE_PEM),
                                     MASTER_CERTIFICATE_PEM_SIZE, pem_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pem_buffer.get(), nullptr);

    // Add the master certificate in PEM format
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromPEM(store, pem_buffer),
              VANILLAPDF_ERROR_SUCCESS);
}

TEST(TrustedCertificateStore, LoadSystemDefaults) {
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> store;

    ASSERT_EQ(TrustedCertificateStore_Create(store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(store.get(), nullptr);

    // Load system default certificates
    EXPECT_EQ(TrustedCertificateStore_LoadSystemDefaults(store),
              VANILLAPDF_ERROR_SUCCESS);
}

// Complete Signature Creation and Verification Test
TEST(SignatureVerifier, CreateAndVerifySignature) {
    // Test message to sign
    const char* test_message = "Hello, World!";
    const size_t test_message_len = strlen(test_message);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<BufferHandle, Buffer_Release> message_buffer;
    HandleGuard<BufferHandle, Buffer_Release> signature_buffer;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Step 1: Create PKCS12 key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key.get(), nullptr);

    // Step 2: Convert PKCS12Key to SigningKey
    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key.get(), nullptr);

    // Step 3: Create signature using SignInitialize/Update/Final
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(test_message),
                                     test_message_len, message_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignFinal(signing_key, signature_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 4: Create trust store (initially empty)
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store.get(), nullptr);

    // Step 5: First verification pass to extract signer certificate
    ASSERT_EQ(SignatureVerifier_Verify(
        message_buffer,
        signature_buffer,
        trust_store,
        nullptr,  // Use default settings
        verify_result.out()
    ), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    // Step 6: Extract signer certificate and add to trust store
    HandleGuard<BufferHandle, Buffer_Release> signer_cert;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCertificate(verify_result, signer_cert.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signer_cert.get(), nullptr);

    ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, signer_cert),
              VANILLAPDF_ERROR_SUCCESS);

    // Release first verification result
    verify_result = HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release>();

    // Step 7: Verify again with signer certificate now in trust store
    ASSERT_EQ(SignatureVerifier_Verify(
        message_buffer,
        signature_buffer,
        trust_store,
        nullptr,  // Use default settings
        verify_result.out()
    ), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    // Step 8: Check verification results
    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);

    boolean_type is_doc_intact = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsDocumentIntact(verify_result, &is_doc_intact),
              VANILLAPDF_ERROR_SUCCESS);

    boolean_type is_cert_trusted = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsCertificateTrusted(verify_result, &is_cert_trusted),
              VANILLAPDF_ERROR_SUCCESS);

    // Get and validate signer common name
    HandleGuard<BufferHandle, Buffer_Release> common_name_buffer;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCommonName(verify_result, common_name_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(common_name_buffer.get(), nullptr);

    string_type cn_data = nullptr;
    size_type cn_len = 0;
    ASSERT_EQ(Buffer_GetData(common_name_buffer, &cn_data, &cn_len), VANILLAPDF_ERROR_SUCCESS);

    // Common name should be "Unit test signer" from SIGNING_CERTIFICATE
    EXPECT_STREQ(cn_data, "Unit test signer");

    // Validate certificate chain
    size_type chain_count = 0;
    ASSERT_EQ(SignatureVerificationResult_GetCertificateChainCount(verify_result, &chain_count),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_GT(chain_count, 0);  // Should have at least the signer certificate

    // Verify we can get certificates from the chain
    if (chain_count > 0) {
        HandleGuard<BufferHandle, Buffer_Release> chain_cert;
        ASSERT_EQ(SignatureVerificationResult_GetCertificateChainAt(verify_result, 0, chain_cert.out()),
                  VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(chain_cert.get(), nullptr);
    }

    // Validate signature and document integrity
    // Note: We manually add the signer certificate to the trust store for testing
    EXPECT_EQ(status, SignatureStatus_Valid);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_doc_intact, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_TRUE);
}

// DigitalSignatureExtensions Tests

TEST(DigitalSignatureExtensions, Verify_NullChecks) {
    DigitalSignatureHandle* signature = nullptr;
    DocumentHandle* document = nullptr;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    SignatureVerificationResultHandle* result = nullptr;

    // Create minimal objects for null checks
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);

    // Null signature check
    EXPECT_EQ(DigitalSignatureExtensions_Verify(nullptr, document, trust_store,
              nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null document check
    EXPECT_EQ(DigitalSignatureExtensions_Verify(signature, nullptr, trust_store,
              nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null trust store check
    EXPECT_EQ(DigitalSignatureExtensions_Verify(signature, document, nullptr,
              nullptr, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null result check
    EXPECT_EQ(DigitalSignatureExtensions_Verify(signature, document, trust_store,
              nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(DigitalSignatureExtensions, SignAndVerifyDocument) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> source_stream;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    HandleGuard<FileHandle, File_Release> source_file;
    HandleGuard<FileHandle, File_Release> signed_file;
    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<DateHandle, Date_Release> signing_time;
    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> signature_settings;
    HandleGuard<DocumentHandle, Document_Release> source_document;
    HandleGuard<DocumentHandle, Document_Release> signed_document;
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> acro_form;
    HandleGuard<FieldCollectionHandle, FieldCollection_Release> fields;
    HandleGuard<FieldHandle, Field_Release> field;
    HandleGuard<SignatureFieldHandle, SignatureField_Release> sig_field;
    HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> digital_signature;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Step 1: Create a simple PDF document in memory
    ASSERT_EQ(InputOutputStream_CreateFromMemory(source_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_stream.get(), nullptr);

    ASSERT_EQ(File_CreateStream(source_stream, "memory_source.pdf", source_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_file.get(), nullptr);

    ASSERT_EQ(Document_CreateFile(source_file, source_document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_document.get(), nullptr);

    // Step 2: Set up signing key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key.get(), nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key.get(), nullptr);

    ASSERT_EQ(Date_CreateCurrent(signing_time.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_time.get(), nullptr);

    // Step 3: Create signature settings
    ASSERT_EQ(DocumentSignatureSettings_Create(signature_settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_settings.get(), nullptr);

    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 4: Create separate memory stream for signed PDF (avoiding GitHub issue #156)
    ASSERT_EQ(InputOutputStream_CreateFromMemory(signed_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signed_stream.get(), nullptr);

    ASSERT_EQ(File_CreateStream(signed_stream, "memory_signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signed_file.get(), nullptr);

    // Step 5: Sign the document from source to signed stream
    ASSERT_EQ(Document_Sign(source_document, signed_file, signature_settings),
              VANILLAPDF_ERROR_SUCCESS);

    // Release source resources (no longer needed)
    source_document = HandleGuard<DocumentHandle, Document_Release>();
    source_file = HandleGuard<FileHandle, File_Release>();
    signed_file = HandleGuard<FileHandle, File_Release>();

    // Step 6: Open the signed document from memory
    ASSERT_EQ(File_OpenStream(signed_stream, "memory_signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signed_file.get(), nullptr);

    ASSERT_EQ(Document_OpenFile(signed_file, signed_document.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signed_document.get(), nullptr);

    // Step 7: Get the signature field from the signed document
    ASSERT_EQ(Document_GetCatalog(signed_document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(catalog.get(), nullptr);

    ASSERT_EQ(Catalog_GetAcroForm(catalog, acro_form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(acro_form.get(), nullptr);

    ASSERT_EQ(InteractiveForm_GetFields(acro_form, fields.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(fields.get(), nullptr);

    size_type field_count = 0;
    ASSERT_EQ(FieldCollection_GetSize(fields, &field_count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_GT(field_count, 0);

    ASSERT_EQ(FieldCollection_At(fields, 0, field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field.get(), nullptr);

    ASSERT_EQ(SignatureField_FromField(field, sig_field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(sig_field.get(), nullptr);

    ASSERT_EQ(SignatureField_GetValue(sig_field, digital_signature.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(digital_signature.get(), nullptr);

    // Step 8: Verify signature (first pass to extract certificate)
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store.get(), nullptr);

    ASSERT_EQ(DigitalSignatureExtensions_Verify(digital_signature, signed_document, trust_store,
              nullptr, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    // Extract signer certificate and add to trust store
    {
        HandleGuard<BufferHandle, Buffer_Release> signer_cert;
        ASSERT_EQ(SignatureVerificationResult_GetSignerCertificate(verify_result, signer_cert.out()),
                  VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(signer_cert.get(), nullptr);

        ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, signer_cert),
                  VANILLAPDF_ERROR_SUCCESS);
    }

    verify_result = HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release>();

    // Step 9: Verify signature again with certificate in trust store
    ASSERT_EQ(DigitalSignatureExtensions_Verify(digital_signature, signed_document, trust_store,
              nullptr, verify_result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result.get(), nullptr);

    // Step 10: Validate the verification result
    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);

    boolean_type is_doc_intact = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsDocumentIntact(verify_result, &is_doc_intact),
              VANILLAPDF_ERROR_SUCCESS);

    boolean_type is_cert_trusted = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsCertificateTrusted(verify_result, &is_cert_trusted),
              VANILLAPDF_ERROR_SUCCESS);

    // Get and validate signer common name
    HandleGuard<BufferHandle, Buffer_Release> common_name_buffer;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCommonName(verify_result, common_name_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(common_name_buffer.get(), nullptr);

    string_type cn_data = nullptr;
    size_type cn_len = 0;
    ASSERT_EQ(Buffer_GetData(common_name_buffer, &cn_data, &cn_len), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_STREQ(cn_data, "Unit test signer");

    // Validate results
    EXPECT_EQ(status, SignatureStatus_Valid);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_doc_intact, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_TRUE);
}

// Parameterized test for different certificate/key types (DSA, EC, ED25519, RSA)
class CertificateTypeTest : public ::testing::TestWithParam<CertificateTestInfo> {
};

TEST_P(CertificateTypeTest, SignAndVerify_Integration) {
    const CertificateTestInfo& cert_info = GetParam();

    const char* test_message = "Test message for key type verification";
    const size_t test_message_len = strlen(test_message);

    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<BufferHandle, Buffer_Release> message_buffer;
    HandleGuard<BufferHandle, Buffer_Release> signature_buffer;
    HandleGuard<TrustedCertificateStoreHandle, TrustedCertificateStore_Release> trust_store;
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;
    HandleGuard<SignatureVerificationResultHandle, SignatureVerificationResult_Release> verify_result;

    // Step 1: Create signing key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(cert_info.data),
                                     cert_info.size, pkcs12_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS) << "Failed to create buffer for " << cert_info.name;
    ASSERT_NE(pkcs12_buffer.get(), nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, cert_info.password, pkcs12_key.out()),
              VANILLAPDF_ERROR_SUCCESS) << "Failed to create PKCS12 key for " << cert_info.name;
    ASSERT_NE(pkcs12_key.get(), nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()),
              VANILLAPDF_ERROR_SUCCESS) << "Failed to convert to SigningKey for " << cert_info.name;
    ASSERT_NE(signing_key.get(), nullptr);

    // Step 2: Create signature with SHA256 (for RSA/DSA/EC) or default (for ED25519)
    // ED25519 uses its own digest internally, but our API still requires a digest type
    // The signing implementation should handle this appropriately
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS) << "Failed to initialize signing for " << cert_info.name;

    ASSERT_EQ(Buffer_CreateFromData(test_message, test_message_len, message_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS) << "Failed to update signature for " << cert_info.name;

    ASSERT_EQ(SigningKey_SignFinal(signing_key, signature_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS) << "Failed to finalize signature for " << cert_info.name;
    ASSERT_NE(signature_buffer.get(), nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key), VANILLAPDF_ERROR_SUCCESS);

    // Step 3: Create trust store and add the signer certificate
    ASSERT_EQ(TrustedCertificateStore_Create(trust_store.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store.get(), nullptr);

    {
        HandleGuard<BufferHandle, Buffer_Release> cert_buffer;
        ASSERT_EQ(PKCS12Key_GetCertificate(pkcs12_key, cert_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(cert_buffer.get(), nullptr);

        ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, cert_buffer),
                  VANILLAPDF_ERROR_SUCCESS);
    }

    // Step 4: Create settings with SkipCertificateValidation (test certs are self-signed)
    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings.get(), nullptr);

    ASSERT_EQ(SignatureVerificationSettings_SetSkipCertificateValidation(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 5: Verify signature
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              settings, verify_result.out()), VANILLAPDF_ERROR_SUCCESS)
        << "Failed to verify signature for " << cert_info.name;
    ASSERT_NE(verify_result.get(), nullptr);

    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // Signature must be valid
    EXPECT_EQ(status, SignatureStatus_Valid) << "Signature verification failed for " << cert_info.name;

    boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE) << "Signature not valid for " << cert_info.name;

    // Step 6: Verify signer common name
    HandleGuard<BufferHandle, Buffer_Release> common_name_buffer;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCommonName(verify_result, common_name_buffer.out()),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(common_name_buffer.get(), nullptr);

    string_type cn_data = nullptr;
    size_type cn_len = 0;
    ASSERT_EQ(Buffer_GetData(common_name_buffer, &cn_data, &cn_len), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_STREQ(cn_data, cert_info.expected_cn) << "Common name mismatch for " << cert_info.name;
}

// Instantiate the parameterized test with different certificate types
INSTANTIATE_TEST_SUITE_P(
    CertificateTypes,
    CertificateTypeTest,
    ::testing::ValuesIn(TEST_CERTIFICATES),
    [](const ::testing::TestParamInfo<CertificateTestInfo>& info) {
        return std::string(info.param.name);
    }
);

// SignatureVerificationSettings ToUnknown/FromUnknown Tests

TEST(SignatureVerificationSettings, ToUnknown_FromUnknown) {
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;
    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings_back;

    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings.get(), nullptr);

    ASSERT_EQ(SignatureVerificationSettings_ToUnknown(settings, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    ASSERT_EQ(SignatureVerificationSettings_FromUnknown(unknown, settings_back.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings_back.get(), nullptr);

    EXPECT_EQ(settings.get(), settings_back.get());
}

TEST(SignatureVerificationSettings, ToUnknown_NullChecks) {
    HandleGuard<SignatureVerificationSettingsHandle, SignatureVerificationSettings_Release> settings;
    IUnknownHandle* unknown = nullptr;

    ASSERT_EQ(SignatureVerificationSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(SignatureVerificationSettings_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(SignatureVerificationSettings_ToUnknown(settings, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(SignatureVerificationSettings, FromUnknown_NullChecks) {
    SignatureVerificationSettingsHandle* settings = nullptr;

    EXPECT_EQ(SignatureVerificationSettings_FromUnknown(nullptr, &settings), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// Field and DigitalSignature ToUnknown/FromUnknown Tests
// Uses a signed document to obtain real Field and DigitalSignature handles

TEST(FieldAndDigitalSignature, ToUnknown_FromUnknown) {
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> source_stream;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> signed_stream;
    HandleGuard<FileHandle, File_Release> source_file;
    HandleGuard<FileHandle, File_Release> signed_file;
    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    HandleGuard<SigningKeyHandle, SigningKey_Release> signing_key;
    HandleGuard<DateHandle, Date_Release> signing_time;
    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> signature_settings;
    HandleGuard<DocumentHandle, Document_Release> source_document;
    HandleGuard<DocumentHandle, Document_Release> signed_document;
    HandleGuard<CatalogHandle, Catalog_Release> catalog;
    HandleGuard<InteractiveFormHandle, InteractiveForm_Release> acro_form;
    HandleGuard<FieldCollectionHandle, FieldCollection_Release> fields;
    HandleGuard<FieldHandle, Field_Release> field;
    HandleGuard<SignatureFieldHandle, SignatureField_Release> sig_field;
    HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> digital_signature;

    // Create and sign a document to get Field and DigitalSignature handles
    ASSERT_EQ(InputOutputStream_CreateFromMemory(source_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(source_stream, "memory_source.pdf", source_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(source_file, source_document.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, pkcs12_buffer.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, pkcs12_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, signing_key.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_CreateCurrent(signing_time.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(DocumentSignatureSettings_Create(signature_settings.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(InputOutputStream_CreateFromMemory(signed_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(signed_stream, "memory_signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_Sign(source_document, signed_file, signature_settings), VANILLAPDF_ERROR_SUCCESS);

    source_document = HandleGuard<DocumentHandle, Document_Release>();
    source_file = HandleGuard<FileHandle, File_Release>();
    signed_file = HandleGuard<FileHandle, File_Release>();

    ASSERT_EQ(File_OpenStream(signed_stream, "memory_signed.pdf", signed_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_OpenFile(signed_file, signed_document.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Document_GetCatalog(signed_document, catalog.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Catalog_GetAcroForm(catalog, acro_form.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InteractiveForm_GetFields(acro_form, fields.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FieldCollection_At(fields, 0, field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field.get(), nullptr);

    ASSERT_EQ(SignatureField_FromField(field, sig_field.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(SignatureField_GetValue(sig_field, digital_signature.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(digital_signature.get(), nullptr);

    // Test Field_ToUnknown / Field_FromUnknown
    {
        HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
        HandleGuard<FieldHandle, Field_Release> field_back;

        ASSERT_EQ(Field_ToUnknown(field, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(unknown.get(), nullptr);

        ASSERT_EQ(Field_FromUnknown(unknown, field_back.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(field_back.get(), nullptr);

        EXPECT_EQ(field.get(), field_back.get());
    }

    // Test DigitalSignature_ToUnknown / DigitalSignature_FromUnknown
    {
        HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
        HandleGuard<DigitalSignatureHandle, DigitalSignature_Release> sig_back;

        ASSERT_EQ(DigitalSignature_ToUnknown(digital_signature, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(unknown.get(), nullptr);

        ASSERT_EQ(DigitalSignature_FromUnknown(unknown, sig_back.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(sig_back.get(), nullptr);

        EXPECT_EQ(digital_signature.get(), sig_back.get());
    }
}

} // namespace signature_verification
