#include "unittest.h"
#include "test_data.h"

namespace signature_verification {

// TrustedCertificateStore Tests

TEST(TrustedCertificateStore, CreateRelease) {
    TrustedCertificateStoreHandle* store = nullptr;

    ASSERT_EQ(TrustedCertificateStore_Create(&store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(store, nullptr);

    ASSERT_EQ(TrustedCertificateStore_Release(store), VANILLAPDF_ERROR_SUCCESS);
}

TEST(TrustedCertificateStore, NullCheck) {
    EXPECT_EQ(TrustedCertificateStore_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TrustedCertificateStore_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TrustedCertificateStore, AddCertificateFromPEM_NullChecks) {
    TrustedCertificateStoreHandle* store = nullptr;
    BufferHandle* pem_buffer = nullptr;

    ASSERT_EQ(TrustedCertificateStore_Create(&store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Create(&pem_buffer), VANILLAPDF_ERROR_SUCCESS);

    // Null store check
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromPEM(nullptr, pem_buffer),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null buffer check
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromPEM(store, nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    ASSERT_EQ(Buffer_Release(pem_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TrustedCertificateStore_Release(store), VANILLAPDF_ERROR_SUCCESS);
}

TEST(TrustedCertificateStore, AddCertificateFromDER_NullChecks) {
    TrustedCertificateStoreHandle* store = nullptr;
    BufferHandle* der_buffer = nullptr;

    ASSERT_EQ(TrustedCertificateStore_Create(&store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Create(&der_buffer), VANILLAPDF_ERROR_SUCCESS);

    // Null store check
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromDER(nullptr, der_buffer),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null buffer check
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromDER(store, nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    ASSERT_EQ(Buffer_Release(der_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TrustedCertificateStore_Release(store), VANILLAPDF_ERROR_SUCCESS);
}

TEST(TrustedCertificateStore, LoadFromDirectory_NullChecks) {
    TrustedCertificateStoreHandle* store = nullptr;

    ASSERT_EQ(TrustedCertificateStore_Create(&store), VANILLAPDF_ERROR_SUCCESS);

    // Null store check
    EXPECT_EQ(TrustedCertificateStore_LoadFromDirectory(nullptr, "some_path"),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null path check
    EXPECT_EQ(TrustedCertificateStore_LoadFromDirectory(store, nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);

    ASSERT_EQ(TrustedCertificateStore_Release(store), VANILLAPDF_ERROR_SUCCESS);
}

TEST(TrustedCertificateStore, LoadSystemDefaults_NullCheck) {
    EXPECT_EQ(TrustedCertificateStore_LoadSystemDefaults(nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// SignatureVerifier Tests

TEST(SignatureVerifier, Verify_NullChecks) {
    BufferHandle* signed_data = nullptr;
    BufferHandle* signature_contents = nullptr;
    TrustedCertificateStoreHandle* store = nullptr;
    SignatureVerificationResultHandle* result = nullptr;

    ASSERT_EQ(Buffer_Create(&signed_data), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Create(&signature_contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TrustedCertificateStore_Create(&store), VANILLAPDF_ERROR_SUCCESS);

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

    ASSERT_EQ(TrustedCertificateStore_Release(store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(signature_contents), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(signed_data), VANILLAPDF_ERROR_SUCCESS);
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

// SignatureVerificationSettings Tests

TEST(SignatureVerificationSettings, CreateAndGetDefaults) {
    SignatureVerificationSettingsHandle* settings = nullptr;

    ASSERT_EQ(SignatureVerificationSettings_Create(&settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings, nullptr);

    // Check default values (should all be false/disabled)
    boolean_type check_revocation = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationSettings_GetCheckRevocationFlag(settings, &check_revocation),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(check_revocation, VANILLAPDF_RV_FALSE);

    boolean_type require_trusted_root = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationSettings_GetRequireTrustedRootFlag(settings, &require_trusted_root),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(require_trusted_root, VANILLAPDF_RV_FALSE);

    boolean_type allow_expired = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationSettings_GetAllowExpiredCertsFlag(settings, &allow_expired),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(allow_expired, VANILLAPDF_RV_FALSE);

    boolean_type check_signing_time = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationSettings_GetCheckSigningTimeFlag(settings, &check_signing_time),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(check_signing_time, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(SignatureVerificationSettings_Release(settings), VANILLAPDF_ERROR_SUCCESS);
}

TEST(SignatureVerificationSettings, SetAndGetAllowExpiredCerts) {
    SignatureVerificationSettingsHandle* settings = nullptr;

    ASSERT_EQ(SignatureVerificationSettings_Create(&settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings, nullptr);

    // Set to true
    ASSERT_EQ(SignatureVerificationSettings_SetAllowExpiredCertsFlag(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    // Verify it was set
    boolean_type allow_expired = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationSettings_GetAllowExpiredCertsFlag(settings, &allow_expired),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(allow_expired, VANILLAPDF_RV_TRUE);

    // Set back to false
    ASSERT_EQ(SignatureVerificationSettings_SetAllowExpiredCertsFlag(settings, VANILLAPDF_RV_FALSE),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SignatureVerificationSettings_GetAllowExpiredCertsFlag(settings, &allow_expired),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(allow_expired, VANILLAPDF_RV_FALSE);

    ASSERT_EQ(SignatureVerificationSettings_Release(settings), VANILLAPDF_ERROR_SUCCESS);
}

TEST(SignatureVerifier, AllowExpiredCertsFlag_Integration) {
    // Note: EXPIRED_CERTIFICATE is an expired certificate
    const char* test_message = "Test message for expired cert verification";
    const size_t test_message_len = strlen(test_message);

    BufferHandle* pkcs12_buffer = nullptr;
    PKCS12KeyHandle* pkcs12_key = nullptr;
    SigningKeyHandle* signing_key = nullptr;
    BufferHandle* message_buffer = nullptr;
    BufferHandle* signature_buffer = nullptr;
    TrustedCertificateStoreHandle* trust_store = nullptr;
    SignatureVerificationSettingsHandle* settings = nullptr;
    SignatureVerificationResultHandle* verify_result = nullptr;

    // Step 1: Create signing key from expired certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(EXPIRED_CERTIFICATE),
                                     EXPIRED_CERTIFICATE_SIZE, &pkcs12_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer, nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, &pkcs12_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key, nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, &signing_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key, nullptr);

    // Step 2: Create signature
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(test_message),
                                     test_message_len, &message_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer, nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignFinal(signing_key, &signature_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer, nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 3: Create trust store and add the signer certificate
    // (SIGNING_CERTIFICATE is self-signed, so we add it as a trusted root to isolate
    // expiration testing from trust issues)
    ASSERT_EQ(TrustedCertificateStore_Create(&trust_store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store, nullptr);

    // Get certificate from the PKCS12 and add to trust store
    BufferHandle* cert_buffer = nullptr;
    ASSERT_EQ(PKCS12Key_GetCertificate(pkcs12_key, &cert_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(cert_buffer, nullptr);

    ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, cert_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(cert_buffer), VANILLAPDF_ERROR_SUCCESS);

    // Step 4: Verify with default settings (AllowExpiredCertsFlag disabled)
    // Should fail with CertificateExpired status
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              nullptr, &verify_result), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // Must explicitly fail with CertificateExpired (not just any failure)
    EXPECT_EQ(status, SignatureStatus_CertificateExpired);

    boolean_type is_cert_trusted = VANILLAPDF_RV_TRUE;
    ASSERT_EQ(SignatureVerificationResult_IsCertificateTrusted(verify_result, &is_cert_trusted),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_FALSE);  // Not trusted due to expiration

    ASSERT_EQ(SignatureVerificationResult_Release(verify_result), VANILLAPDF_ERROR_SUCCESS);
    verify_result = nullptr;

    // Step 5: Create settings with AllowExpiredCertsFlag enabled
    ASSERT_EQ(SignatureVerificationSettings_Create(&settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings, nullptr);

    ASSERT_EQ(SignatureVerificationSettings_SetAllowExpiredCertsFlag(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 6: Verify again with AllowExpiredCertsFlag enabled
    // Should now succeed despite expiration
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              settings, &verify_result), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // With AllowExpiredCertsFlag, expired certs are allowed - must be Valid
    EXPECT_EQ(status, SignatureStatus_Valid);

    boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);

    is_cert_trusted = VANILLAPDF_RV_FALSE;  // Reuse from Step 4
    ASSERT_EQ(SignatureVerificationResult_IsCertificateTrusted(verify_result, &is_cert_trusted),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_TRUE);  // Now trusted despite expiration

    // Cleanup
    if (verify_result) SignatureVerificationResult_Release(verify_result);
    if (settings) SignatureVerificationSettings_Release(settings);
    if (trust_store) TrustedCertificateStore_Release(trust_store);
    if (signature_buffer) Buffer_Release(signature_buffer);
    if (message_buffer) Buffer_Release(message_buffer);
    if (signing_key) SigningKey_Release(signing_key);
    if (pkcs12_key) PKCS12Key_Release(pkcs12_key);
    if (pkcs12_buffer) Buffer_Release(pkcs12_buffer);
}

// Parameterized test for weak algorithm detection
class WeakAlgorithmTest : public ::testing::TestWithParam<MessageDigestAlgorithmType> {
};

TEST_P(WeakAlgorithmTest, AllowWeakAlgorithmsFlag_Integration) {
    MessageDigestAlgorithmType weak_algorithm = GetParam();

    const char* test_message = "Test message for weak algorithm verification";
    const size_t test_message_len = strlen(test_message);

    BufferHandle* pkcs12_buffer = nullptr;
    PKCS12KeyHandle* pkcs12_key = nullptr;
    SigningKeyHandle* signing_key = nullptr;
    BufferHandle* message_buffer = nullptr;
    BufferHandle* signature_buffer = nullptr;
    TrustedCertificateStoreHandle* trust_store = nullptr;
    SignatureVerificationSettingsHandle* settings = nullptr;
    SignatureVerificationResultHandle* verify_result = nullptr;

    // Step 1: Create signing key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, &pkcs12_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer, nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, &pkcs12_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key, nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, &signing_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key, nullptr);

    // Step 2: Create signature with weak algorithm
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, weak_algorithm),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(test_message, test_message_len, &message_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer, nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignFinal(signing_key, &signature_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer, nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key), VANILLAPDF_ERROR_SUCCESS);

    // Step 3: Create trust store and add the signer certificate
    ASSERT_EQ(TrustedCertificateStore_Create(&trust_store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store, nullptr);

    BufferHandle* cert_buffer = nullptr;
    ASSERT_EQ(PKCS12Key_GetCertificate(pkcs12_key, &cert_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(cert_buffer, nullptr);

    ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, cert_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(cert_buffer), VANILLAPDF_ERROR_SUCCESS);

    // Step 4: Verify with default settings (AllowWeakAlgorithmsFlag disabled)
    // Should fail with WeakAlgorithm status
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              nullptr, &verify_result), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

    SignatureVerificationStatusType status = SignatureStatus_Undefined;
    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // Must fail with WeakAlgorithm status
    EXPECT_EQ(status, SignatureStatus_WeakAlgorithm);

    boolean_type is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);  // Cryptographically valid, just weak

    ASSERT_EQ(SignatureVerificationResult_Release(verify_result), VANILLAPDF_ERROR_SUCCESS);
    verify_result = nullptr;

    // Step 5: Create settings with AllowWeakAlgorithmsFlag enabled
    ASSERT_EQ(SignatureVerificationSettings_Create(&settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(settings, nullptr);

    ASSERT_EQ(SignatureVerificationSettings_SetAllowWeakAlgorithmsFlag(settings, VANILLAPDF_RV_TRUE),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 6: Verify again with AllowWeakAlgorithmsFlag enabled
    // Should now succeed despite weak algorithm
    ASSERT_EQ(SignatureVerifier_Verify(message_buffer, signature_buffer, trust_store,
              settings, &verify_result), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

    ASSERT_EQ(SignatureVerificationResult_GetStatus(verify_result, &status),
              VANILLAPDF_ERROR_SUCCESS);

    // With AllowWeakAlgorithmsFlag, weak algorithms are allowed - must be Valid
    EXPECT_EQ(status, SignatureStatus_Valid);

    is_signature_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(SignatureVerificationResult_IsSignatureValid(verify_result, &is_signature_valid),
              VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);

    // Cleanup
    if (verify_result) SignatureVerificationResult_Release(verify_result);
    if (settings) SignatureVerificationSettings_Release(settings);
    if (trust_store) TrustedCertificateStore_Release(trust_store);
    if (signature_buffer) Buffer_Release(signature_buffer);
    if (message_buffer) Buffer_Release(message_buffer);
    if (signing_key) SigningKey_Release(signing_key);
    if (pkcs12_key) PKCS12Key_Release(pkcs12_key);
    if (pkcs12_buffer) Buffer_Release(pkcs12_buffer);
}

// Instantiate the parameterized test with different weak algorithms
INSTANTIATE_TEST_SUITE_P(
    WeakAlgorithms,
    WeakAlgorithmTest,
    ::testing::Values(
        MessageDigestAlgorithmType_MD5,
        MessageDigestAlgorithmType_MD4,
        MessageDigestAlgorithmType_SHA1
    ),
    [](const ::testing::TestParamInfo<MessageDigestAlgorithmType>& info) {
        switch (info.param) {
            case MessageDigestAlgorithmType_MD5:  return "MD5";
            case MessageDigestAlgorithmType_MD4:  return "MD4";
            case MessageDigestAlgorithmType_SHA1: return "SHA1";
            default: return "Unknown";
        }
    }
);

// Real Certificate Tests

TEST(TrustedCertificateStore, AddCertificateFromPEM_Master) {
    TrustedCertificateStoreHandle* store = nullptr;
    BufferHandle* pem_buffer = nullptr;

    ASSERT_EQ(TrustedCertificateStore_Create(&store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(store, nullptr);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(MASTER_CERTIFICATE_PEM),
                                     MASTER_CERTIFICATE_PEM_SIZE, &pem_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pem_buffer, nullptr);

    // Add the master certificate in PEM format
    EXPECT_EQ(TrustedCertificateStore_AddCertificateFromPEM(store, pem_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_Release(pem_buffer), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(TrustedCertificateStore_Release(store), VANILLAPDF_ERROR_SUCCESS);
}

TEST(TrustedCertificateStore, LoadSystemDefaults) {
    TrustedCertificateStoreHandle* store = nullptr;

    ASSERT_EQ(TrustedCertificateStore_Create(&store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(store, nullptr);

    // Load system default certificates
    EXPECT_EQ(TrustedCertificateStore_LoadSystemDefaults(store),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(TrustedCertificateStore_Release(store), VANILLAPDF_ERROR_SUCCESS);
}

// Complete Signature Creation and Verification Test
TEST(SignatureVerifier, CreateAndVerifySignature) {
    // Test message to sign
    const char* test_message = "Hello, World!";
    const size_t test_message_len = strlen(test_message);

    BufferHandle* pkcs12_buffer = nullptr;
    PKCS12KeyHandle* pkcs12_key = nullptr;
    SigningKeyHandle* signing_key = nullptr;
    BufferHandle* message_buffer = nullptr;
    BufferHandle* signature_buffer = nullptr;
    TrustedCertificateStoreHandle* trust_store = nullptr;
    SignatureVerificationResultHandle* verify_result = nullptr;

    // Step 1: Create PKCS12 key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, &pkcs12_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer, nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, &pkcs12_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key, nullptr);

    // Step 2: Convert PKCS12Key to SigningKey
    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, &signing_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key, nullptr);

    // Step 3: Create signature using SignInitialize/Update/Final
    ASSERT_EQ(SigningKey_SignInitialize(signing_key, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(test_message),
                                     test_message_len, &message_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(message_buffer, nullptr);

    ASSERT_EQ(SigningKey_SignUpdate(signing_key, message_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SigningKey_SignFinal(signing_key, &signature_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_buffer, nullptr);

    ASSERT_EQ(SigningKey_SignCleanup(signing_key),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 4: Create trust store (initially empty)
    ASSERT_EQ(TrustedCertificateStore_Create(&trust_store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store, nullptr);

    // Step 5: First verification pass to extract signer certificate
    ASSERT_EQ(SignatureVerifier_Verify(
        message_buffer,
        signature_buffer,
        trust_store,
        nullptr,  // Use default settings
        &verify_result
    ), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

    // Step 6: Extract signer certificate and add to trust store
    BufferHandle* signer_cert = nullptr;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCertificate(verify_result, &signer_cert),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signer_cert, nullptr);

    ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, signer_cert),
              VANILLAPDF_ERROR_SUCCESS);

    // Release first verification result
    ASSERT_EQ(SignatureVerificationResult_Release(verify_result), VANILLAPDF_ERROR_SUCCESS);
    verify_result = nullptr;

    // Step 7: Verify again with signer certificate now in trust store
    ASSERT_EQ(SignatureVerifier_Verify(
        message_buffer,
        signature_buffer,
        trust_store,
        nullptr,  // Use default settings
        &verify_result
    ), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

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
    BufferHandle* common_name_buffer = nullptr;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCommonName(verify_result, &common_name_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(common_name_buffer, nullptr);

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
        BufferHandle* chain_cert = nullptr;
        ASSERT_EQ(SignatureVerificationResult_GetCertificateChainAt(verify_result, 0, &chain_cert),
                  VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(chain_cert, nullptr);
        Buffer_Release(chain_cert);
    }

    // Validate signature and document integrity
    // Note: We manually add the signer certificate to the trust store for testing
    EXPECT_EQ(status, SignatureStatus_Valid);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_doc_intact, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_TRUE);

    // Cleanup
    if (verify_result) SignatureVerificationResult_Release(verify_result);
    if (signer_cert) Buffer_Release(signer_cert);
    if (common_name_buffer) Buffer_Release(common_name_buffer);
    if (trust_store) TrustedCertificateStore_Release(trust_store);
    if (signature_buffer) Buffer_Release(signature_buffer);
    if (message_buffer) Buffer_Release(message_buffer);
    if (signing_key) SigningKey_Release(signing_key);
    if (pkcs12_key) PKCS12Key_Release(pkcs12_key);
    if (pkcs12_buffer) Buffer_Release(pkcs12_buffer);
}

// DigitalSignatureExtensions Tests

TEST(DigitalSignatureExtensions, Verify_NullChecks) {
    DigitalSignatureHandle* signature = nullptr;
    DocumentHandle* document = nullptr;
    TrustedCertificateStoreHandle* trust_store = nullptr;
    SignatureVerificationResultHandle* result = nullptr;

    // Create minimal objects for null checks
    ASSERT_EQ(TrustedCertificateStore_Create(&trust_store), VANILLAPDF_ERROR_SUCCESS);

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

    ASSERT_EQ(TrustedCertificateStore_Release(trust_store), VANILLAPDF_ERROR_SUCCESS);
}

TEST(DigitalSignatureExtensions, SignAndVerifyDocument) {
    InputOutputStreamHandle* source_stream = nullptr;
    InputOutputStreamHandle* signed_stream = nullptr;
    FileHandle* source_file = nullptr;
    FileHandle* signed_file = nullptr;
    BufferHandle* pkcs12_buffer = nullptr;
    PKCS12KeyHandle* pkcs12_key = nullptr;
    SigningKeyHandle* signing_key = nullptr;
    DateHandle* signing_time = nullptr;
    DocumentSignatureSettingsHandle* signature_settings = nullptr;
    DocumentHandle* source_document = nullptr;
    DocumentHandle* signed_document = nullptr;
    CatalogHandle* catalog = nullptr;
    InteractiveFormHandle* acro_form = nullptr;
    FieldCollectionHandle* fields = nullptr;
    FieldHandle* field = nullptr;
    SignatureFieldHandle* sig_field = nullptr;
    DigitalSignatureHandle* digital_signature = nullptr;
    TrustedCertificateStoreHandle* trust_store = nullptr;
    SignatureVerificationResultHandle* verify_result = nullptr;

    // Step 1: Create a simple PDF document in memory
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&source_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_stream, nullptr);

    ASSERT_EQ(File_CreateStream(source_stream, "memory_source.pdf", &source_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_file, nullptr);

    ASSERT_EQ(Document_CreateFile(source_file, &source_document), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(source_document, nullptr);

    // Step 2: Set up signing key from test certificate
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(SIGNING_CERTIFICATE),
                                     SIGNING_CERTIFICATE_SIZE, &pkcs12_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_buffer, nullptr);

    ASSERT_EQ(PKCS12Key_CreateFromBuffer(pkcs12_buffer, nullptr, &pkcs12_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pkcs12_key, nullptr);

    ASSERT_EQ(PKCS12Key_ToSigningKey(pkcs12_key, &signing_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_key, nullptr);

    ASSERT_EQ(Date_CreateCurrent(&signing_time), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signing_time, nullptr);

    // Step 3: Create signature settings
    ASSERT_EQ(DocumentSignatureSettings_Create(&signature_settings), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signature_settings, nullptr);

    ASSERT_EQ(DocumentSignatureSettings_SetSigningKey(signature_settings, signing_key),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetDigest(signature_settings, MessageDigestAlgorithmType_SHA256),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(DocumentSignatureSettings_SetSigningTime(signature_settings, signing_time),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 4: Create separate memory stream for signed PDF (avoiding GitHub issue #156)
    ASSERT_EQ(InputOutputStream_CreateFromMemory(&signed_stream), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signed_stream, nullptr);

    ASSERT_EQ(File_CreateStream(signed_stream, "memory_signed.pdf", &signed_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signed_file, nullptr);

    // Step 5: Sign the document from source to signed stream
    ASSERT_EQ(Document_Sign(source_document, signed_file, signature_settings),
              VANILLAPDF_ERROR_SUCCESS);

    // Release source resources (no longer needed)
    ASSERT_EQ(Document_Release(source_document), VANILLAPDF_ERROR_SUCCESS);
    source_document = nullptr;
    ASSERT_EQ(File_Release(source_file), VANILLAPDF_ERROR_SUCCESS);
    source_file = nullptr;
    ASSERT_EQ(File_Release(signed_file), VANILLAPDF_ERROR_SUCCESS);
    signed_file = nullptr;

    // Step 6: Open the signed document from memory
    ASSERT_EQ(File_OpenStream(signed_stream, "memory_signed.pdf", &signed_file), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signed_file, nullptr);

    ASSERT_EQ(Document_OpenFile(signed_file, &signed_document), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signed_document, nullptr);

    // Step 7: Get the signature field from the signed document
    ASSERT_EQ(Document_GetCatalog(signed_document, &catalog), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(catalog, nullptr);

    ASSERT_EQ(Catalog_GetAcroForm(catalog, &acro_form), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(acro_form, nullptr);

    ASSERT_EQ(InteractiveForm_GetFields(acro_form, &fields), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(fields, nullptr);

    size_type field_count = 0;
    ASSERT_EQ(FieldCollection_GetSize(fields, &field_count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_GT(field_count, 0);

    ASSERT_EQ(FieldCollection_At(fields, 0, &field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(field, nullptr);

    ASSERT_EQ(Field_ToSignature(field, &sig_field), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(sig_field, nullptr);

    ASSERT_EQ(SignatureField_GetValue(sig_field, &digital_signature), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(digital_signature, nullptr);

    // Step 8: Verify signature (first pass to extract certificate)
    ASSERT_EQ(TrustedCertificateStore_Create(&trust_store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store, nullptr);

    ASSERT_EQ(DigitalSignatureExtensions_Verify(digital_signature, signed_document, trust_store,
              nullptr, &verify_result), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

    // Extract signer certificate and add to trust store
    BufferHandle* signer_cert = nullptr;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCertificate(verify_result, &signer_cert),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(signer_cert, nullptr);

    ASSERT_EQ(TrustedCertificateStore_AddCertificateFromDER(trust_store, signer_cert),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Buffer_Release(signer_cert), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(SignatureVerificationResult_Release(verify_result), VANILLAPDF_ERROR_SUCCESS);
    verify_result = nullptr;

    // Step 9: Verify signature again with certificate in trust store
    ASSERT_EQ(DigitalSignatureExtensions_Verify(digital_signature, signed_document, trust_store,
              nullptr, &verify_result), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

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
    BufferHandle* common_name_buffer = nullptr;
    ASSERT_EQ(SignatureVerificationResult_GetSignerCommonName(verify_result, &common_name_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(common_name_buffer, nullptr);

    string_type cn_data = nullptr;
    size_type cn_len = 0;
    ASSERT_EQ(Buffer_GetData(common_name_buffer, &cn_data, &cn_len), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_STREQ(cn_data, "Unit test signer");

    // Validate results
    EXPECT_EQ(status, SignatureStatus_Valid);
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_doc_intact, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_cert_trusted, VANILLAPDF_RV_TRUE);

    // Cleanup
    if (common_name_buffer) Buffer_Release(common_name_buffer);
    if (verify_result) SignatureVerificationResult_Release(verify_result);
    if (trust_store) TrustedCertificateStore_Release(trust_store);
    if (digital_signature) DigitalSignature_Release(digital_signature);
    if (sig_field) SignatureField_Release(sig_field);
    if (field) Field_Release(field);
    if (fields) FieldCollection_Release(fields);
    if (acro_form) InteractiveForm_Release(acro_form);
    if (catalog) Catalog_Release(catalog);
    if (signed_document) Document_Release(signed_document);
    if (signed_file) File_Release(signed_file);
    if (signature_settings) DocumentSignatureSettings_Release(signature_settings);
    if (signing_time) Date_Release(signing_time);
    if (signing_key) SigningKey_Release(signing_key);
    if (pkcs12_key) PKCS12Key_Release(pkcs12_key);
    if (pkcs12_buffer) Buffer_Release(pkcs12_buffer);
    if (signed_stream) InputOutputStream_Release(signed_stream);
    if (source_stream) InputOutputStream_Release(source_stream);
}

} // namespace signature_verification
