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

} // namespace signature_verification
