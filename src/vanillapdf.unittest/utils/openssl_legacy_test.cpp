#include "unittest.h"
#include "handle_guard.h"
#include "test_certificates.h"

namespace openssl_legacy {

// Test_Legacy_RC2.pfx protects its certificates with pbeWithSHA1And40BitRC2-CBC,
// the OpenSSL 1.x default. RC2 is provided only by the OpenSSL legacy provider,
// which the library loads when built with VANILLAPDF_ENABLE_OPENSSL_LEGACY_PROVIDER.
TEST(OpenSSLLegacy, PKCS12KeyProtectedWithRC2) {
    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(TEST_LEGACY_RC2_CERTIFICATE),
        TEST_LEGACY_RC2_CERTIFICATE_SIZE, pkcs12_buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    auto result = PKCS12Key_CreateFromBuffer(pkcs12_buffer, "test", pkcs12_key.out());

#if defined(VANILLAPDF_TEST_OPENSSL_LEGACY)
    EXPECT_EQ(result, VANILLAPDF_ERROR_SUCCESS);
#else
    // Reported as unsupported, distinguishable from a wrong password or a damaged file
    EXPECT_EQ(result, VANILLAPDF_ERROR_NOT_SUPPORTED);
#endif
}

TEST(OpenSSLLegacy, PKCS12KeyWrongPasswordIsNotUnsupported) {
    HandleGuard<BufferHandle, Buffer_Release> pkcs12_buffer;
    ASSERT_EQ(Buffer_CreateFromData(reinterpret_cast<string_type>(TEST_LEGACY_RC2_CERTIFICATE),
        TEST_LEGACY_RC2_CERTIFICATE_SIZE, pkcs12_buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    // The MAC check fails before any RC2 decryption is attempted
    HandleGuard<PKCS12KeyHandle, PKCS12Key_Release> pkcs12_key;
    auto result = PKCS12Key_CreateFromBuffer(pkcs12_buffer, "wrong", pkcs12_key.out());
    EXPECT_NE(result, VANILLAPDF_ERROR_SUCCESS);
    EXPECT_NE(result, VANILLAPDF_ERROR_NOT_SUPPORTED);
}

} // namespace openssl_legacy
