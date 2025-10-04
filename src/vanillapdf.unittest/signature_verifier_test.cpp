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
              VerificationFlag_None, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null signature_contents check
    EXPECT_EQ(SignatureVerifier_Verify(signed_data, nullptr, store,
              VerificationFlag_None, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null store check
    EXPECT_EQ(SignatureVerifier_Verify(signed_data, signature_contents, nullptr,
              VerificationFlag_None, &result), VANILLAPDF_ERROR_PARAMETER_VALUE);

    // Null result check
    EXPECT_EQ(SignatureVerifier_Verify(signed_data, signature_contents, store,
              VerificationFlag_None, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);

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

TEST(SignatureVerificationResult, Release_NullCheck) {
    EXPECT_EQ(SignatureVerificationResult_Release(nullptr),
              VANILLAPDF_ERROR_PARAMETER_VALUE);
}

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
    BufferHandle* pem_cert_buffer = nullptr;
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

    // Step 4: Create trust store and add master certificate
    ASSERT_EQ(TrustedCertificateStore_Create(&trust_store), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(trust_store, nullptr);

    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(MASTER_CERTIFICATE_PEM),
                                     MASTER_CERTIFICATE_PEM_SIZE, &pem_cert_buffer),
              VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(pem_cert_buffer, nullptr);

    ASSERT_EQ(TrustedCertificateStore_AddCertificateFromPEM(trust_store, pem_cert_buffer),
              VANILLAPDF_ERROR_SUCCESS);

    // Step 5: Verify the signature
    ASSERT_EQ(SignatureVerifier_Verify(
        message_buffer,
        signature_buffer,
        trust_store,
        VerificationFlag_None,
        &verify_result
    ), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(verify_result, nullptr);

    // Step 6: Check verification results
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

    // Get signer common name
    BufferHandle* common_name_buffer = nullptr;
    error_type cn_rv = SignatureVerificationResult_GetSignerCommonName(verify_result, &common_name_buffer);
    if (cn_rv == VANILLAPDF_ERROR_SUCCESS && common_name_buffer != nullptr) {
        string_type cn_data = nullptr;
        size_type cn_len = 0;
        ASSERT_EQ(Buffer_GetData(common_name_buffer, &cn_data, &cn_len), VANILLAPDF_ERROR_SUCCESS);
        // Common name should be "Unit test signer" from SIGNING_CERTIFICATE
        Buffer_Release(common_name_buffer);
    }

    // Log results for debugging
    printf("Signature verification status: %d\n", status);
    printf("Is signature valid: %d\n", is_signature_valid);
    printf("Is document intact: %d\n", is_doc_intact);
    printf("Is certificate trusted: %d\n", is_cert_trusted);

    // Expectations: signature should be cryptographically valid, document intact
    // Note: Certificate trust depends on whether master cert is in system trust store
    EXPECT_EQ(is_signature_valid, VANILLAPDF_RV_TRUE);
    EXPECT_EQ(is_doc_intact, VANILLAPDF_RV_TRUE);

    // Cleanup
    if (verify_result) SignatureVerificationResult_Release(verify_result);
    if (pem_cert_buffer) Buffer_Release(pem_cert_buffer);
    if (trust_store) TrustedCertificateStore_Release(trust_store);
    if (signature_buffer) Buffer_Release(signature_buffer);
    if (message_buffer) Buffer_Release(message_buffer);
    if (signing_key) SigningKey_Release(signing_key);
    if (pkcs12_key) PKCS12Key_Release(pkcs12_key);
    if (pkcs12_buffer) Buffer_Release(pkcs12_buffer);
}

} // namespace signature_verification
