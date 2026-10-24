#include "unittest.h"
#include "handle_guard.h"
#include "test_certificates.h"

namespace openssl_legacy {

// Test_Legacy_RC2.pfx protects its certificates with pbeWithSHA1And40BitRC2-CBC,
// the OpenSSL 1.x default. RC2 is provided only by the OpenSSL legacy provider,
// which the library loads unless built without VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER
// or initialized with LegacyAlgorithmsPolicy_Disabled.
static error_type LoadLegacyKey(string_type password) {
    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    EXPECT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(TEST_LEGACY_RC2_CERTIFICATE),
        TEST_LEGACY_RC2_CERTIFICATE_SIZE, pkcs12_buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    return PKCS12Key_CreateFromBuffer(pkcs12_buffer, password, pkcs12_key.out());
}

static error_type InitializeWithPolicy(LegacyAlgorithmsPolicyType policy) {
    HandleGuard<CryptoSettingsHandle, CryptoSettings_Release> settings;
    EXPECT_EQ(CryptoSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(CryptoSettings_SetLegacyAlgorithmsPolicy(settings, policy), VANILLAPDF_ERROR_SUCCESS);
    return CryptoUtils_Initialize(settings);
}

static boolean_type IsLegacyAvailable() {
    boolean_type available = VANILLAPDF_RV_FALSE;
    EXPECT_EQ(CryptoUtils_GetLegacyAlgorithmsAvailable(&available), VANILLAPDF_ERROR_SUCCESS);
    return available;
}

// Every test starts from an uninitialized toolkit and leaves one behind,
// so the next operation initializes it with the default settings again
class LegacyAlgorithmsPolicyTest : public ::testing::Test {
protected:
    void SetUp() override {
        ASSERT_EQ(CryptoUtils_Cleanup(), VANILLAPDF_ERROR_SUCCESS);
    }

    void TearDown() override {
        ASSERT_EQ(CryptoUtils_Cleanup(), VANILLAPDF_ERROR_SUCCESS);
    }
};

TEST(CryptoSettings, DefaultPolicyIsOptional) {
    HandleGuard<CryptoSettingsHandle, CryptoSettings_Release> settings;
    ASSERT_EQ(CryptoSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);

    LegacyAlgorithmsPolicyType policy = LegacyAlgorithmsPolicy_Undefined;
    ASSERT_EQ(CryptoSettings_GetLegacyAlgorithmsPolicy(settings, &policy), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(policy, LegacyAlgorithmsPolicy_Optional);
}

TEST(CryptoSettings, SetPolicy) {
    HandleGuard<CryptoSettingsHandle, CryptoSettings_Release> settings;
    ASSERT_EQ(CryptoSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(CryptoSettings_SetLegacyAlgorithmsPolicy(settings, LegacyAlgorithmsPolicy_Required), VANILLAPDF_ERROR_SUCCESS);

    LegacyAlgorithmsPolicyType policy = LegacyAlgorithmsPolicy_Undefined;
    ASSERT_EQ(CryptoSettings_GetLegacyAlgorithmsPolicy(settings, &policy), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(policy, LegacyAlgorithmsPolicy_Required);
}

TEST(CryptoSettings, SetUndefinedPolicyIsRejected) {
    HandleGuard<CryptoSettingsHandle, CryptoSettings_Release> settings;
    ASSERT_EQ(CryptoSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(CryptoSettings_SetLegacyAlgorithmsPolicy(settings, LegacyAlgorithmsPolicy_Undefined), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST_F(LegacyAlgorithmsPolicyTest, DisabledRejectsLegacyKey) {
    ASSERT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Disabled), VANILLAPDF_ERROR_SUCCESS);

    // Reported as unsupported, distinguishable from a wrong password or a damaged file
    EXPECT_EQ(LoadLegacyKey("test"), VANILLAPDF_ERROR_NOT_SUPPORTED);
    EXPECT_EQ(IsLegacyAvailable(), VANILLAPDF_RV_FALSE);
}

TEST_F(LegacyAlgorithmsPolicyTest, OptionalLoadsLegacyKeyWhenBuiltIn) {
    ASSERT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Optional), VANILLAPDF_ERROR_SUCCESS);

#if defined(VANILLAPDF_TEST_OPENSSL_LEGACY_PROVIDER)
    EXPECT_EQ(LoadLegacyKey("test"), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(IsLegacyAvailable(), VANILLAPDF_RV_TRUE);
#else
    EXPECT_EQ(LoadLegacyKey("test"), VANILLAPDF_ERROR_NOT_SUPPORTED);
    EXPECT_EQ(IsLegacyAvailable(), VANILLAPDF_RV_FALSE);
#endif
}

TEST_F(LegacyAlgorithmsPolicyTest, RequiredLoadsLegacyKeyWhenBuiltIn) {

#if defined(VANILLAPDF_TEST_OPENSSL_LEGACY_PROVIDER)
    ASSERT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Required), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(LoadLegacyKey("test"), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(IsLegacyAvailable(), VANILLAPDF_RV_TRUE);
#else
    // A build without the legacy provider can never satisfy the requirement
    EXPECT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Required), VANILLAPDF_ERROR_NOT_SUPPORTED);
#endif
}

TEST_F(LegacyAlgorithmsPolicyTest, ReinitializeWithSamePolicy) {
    ASSERT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Disabled), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Disabled), VANILLAPDF_ERROR_SUCCESS);
}

// The first initialization stays in effect until cleanup,
// whichever policy a later initialization passes
TEST_F(LegacyAlgorithmsPolicyTest, ReinitializeKeepsFirstPolicy) {
    ASSERT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Disabled), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Required), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(IsLegacyAvailable(), VANILLAPDF_RV_FALSE);
    EXPECT_EQ(LoadLegacyKey("test"), VANILLAPDF_ERROR_NOT_SUPPORTED);
}

// The first cryptographic operation initializes with the default settings
TEST_F(LegacyAlgorithmsPolicyTest, ReinitializeAfterLazyInitialization) {
    boolean_type available_before = IsLegacyAvailable();
    EXPECT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Disabled), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(IsLegacyAvailable(), available_before);
}

TEST_F(LegacyAlgorithmsPolicyTest, CleanupAllowsLegacyAgain) {
    ASSERT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Disabled), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(LoadLegacyKey("test"), VANILLAPDF_ERROR_NOT_SUPPORTED);

    ASSERT_EQ(CryptoUtils_Cleanup(), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(InitializeWithPolicy(LegacyAlgorithmsPolicy_Optional), VANILLAPDF_ERROR_SUCCESS);

#if defined(VANILLAPDF_TEST_OPENSSL_LEGACY_PROVIDER)
    EXPECT_EQ(LoadLegacyKey("test"), VANILLAPDF_ERROR_SUCCESS);
#else
    EXPECT_EQ(LoadLegacyKey("test"), VANILLAPDF_ERROR_NOT_SUPPORTED);
#endif
}

TEST(OpenSSLLegacy, PKCS12KeyProtectedWithRC2) {
    auto result = LoadLegacyKey("test");

#if defined(VANILLAPDF_TEST_OPENSSL_LEGACY_PROVIDER)
    EXPECT_EQ(result, VANILLAPDF_ERROR_SUCCESS);
#else
    // Reported as unsupported, distinguishable from a wrong password or a damaged file
    EXPECT_EQ(result, VANILLAPDF_ERROR_NOT_SUPPORTED);
#endif
}

TEST(OpenSSLLegacy, PKCS12KeyWrongPasswordIsNotUnsupported) {
    // The MAC check fails before any RC2 decryption is attempted
    auto result = LoadLegacyKey("wrong");
    EXPECT_NE(result, VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(result, VANILLAPDF_ERROR_NOT_SUPPORTED);
}

} // namespace openssl_legacy
