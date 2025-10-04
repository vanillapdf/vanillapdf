#include "unittest.h"

namespace signature_verification {

// Test certificate data from document_test.cpp (PKCS#12 format)
static const unsigned char SIGNING_CERTIFICATE[] = {
    0x30, 0x82, 0x09, 0xc6, 0x02, 0x01, 0x03, 0x30, 0x82, 0x09, 0x90, 0x06, 0x09, 0x2a, 0x86, 0x48,
    0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01, 0xa0, 0x82, 0x09, 0x81, 0x04, 0x82, 0x09, 0x7d, 0x30, 0x82,
    0x09, 0x79, 0x30, 0x82, 0x03, 0xff, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07,
    0x06, 0xa0, 0x82, 0x03, 0xf0, 0x30, 0x82, 0x03, 0xec, 0x02, 0x01, 0x00, 0x30, 0x82, 0x03, 0xe5,
    0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x07, 0x01, 0x30, 0x1c, 0x06, 0x0a, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x0c, 0x01, 0x03, 0x30, 0x0e, 0x04, 0x08, 0x7c, 0x42, 0x40,
    0x1e, 0xe7, 0x87, 0x29, 0x52, 0x02, 0x02, 0x08, 0x00, 0x80, 0x82, 0x03, 0xb8, 0x29, 0xf4, 0x8b,
    0xd9, 0x7b, 0x69, 0x4a, 0x9e, 0x96, 0x07, 0x12, 0xee, 0x80, 0xe3, 0xa2, 0xe9, 0x1b, 0x12, 0x6d,
    0x0c, 0x8b, 0xdc, 0x13, 0xe5, 0xbd, 0x9c, 0x57, 0xf3, 0x29, 0x75, 0xc3, 0xd8, 0x39, 0x5a, 0x73,
    0x57, 0xd9, 0xb7, 0xc6, 0xde, 0x2c, 0x5c, 0x34, 0xfe, 0x57, 0xfc, 0x22, 0x19, 0x0e, 0x7c, 0xd3,
    0x6f, 0x26, 0x29, 0x4c, 0x90, 0x86, 0x2b, 0xc6, 0x93, 0x49, 0xdb, 0xc5, 0xe3, 0x6e, 0x83, 0x3c
    // ... truncated for brevity, full data available in document_test.cpp
};

static const size_type SIGNING_CERTIFICATE_SIZE = sizeof(SIGNING_CERTIFICATE);

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

} // namespace signature_verification
