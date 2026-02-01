#include "unittest.h"

#include "vanillapdf/c_vanillapdf_api.h"

namespace digital_signature_objects {

// DigitalSignature ToUnknown/FromUnknown null checks

TEST(DigitalSignature, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(DigitalSignature_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(DigitalSignature, FromUnknown_NullChecks) {
    DigitalSignatureHandle* sig = nullptr;

    EXPECT_EQ(DigitalSignature_FromUnknown(nullptr, &sig), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// Field ToUnknown/FromUnknown null checks

TEST(Field, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(Field_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Field, FromUnknown_NullChecks) {
    FieldHandle* field = nullptr;

    EXPECT_EQ(Field_FromUnknown(nullptr, &field), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// OutlineItemColor ToUnknown/FromUnknown null checks

TEST(OutlineItemColor, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(OutlineItemColor_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(OutlineItemColor, FromUnknown_NullChecks) {
    OutlineItemColorHandle* color = nullptr;

    EXPECT_EQ(OutlineItemColor_FromUnknown(nullptr, &color), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// OutlineItemFlags ToUnknown/FromUnknown null checks

TEST(OutlineItemFlags, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(OutlineItemFlags_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(OutlineItemFlags, FromUnknown_NullChecks) {
    OutlineItemFlagsHandle* flags = nullptr;

    EXPECT_EQ(OutlineItemFlags_FromUnknown(nullptr, &flags), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// EncryptionKey ToUnknown/FromUnknown null checks

TEST(EncryptionKey, ToUnknown_NullChecks) {
    IUnknownHandle* unknown = nullptr;

    EXPECT_EQ(EncryptionKey_ToUnknown(nullptr, &unknown), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(EncryptionKey, FromUnknown_NullChecks) {
    EncryptionKeyHandle* key = nullptr;

    EXPECT_EQ(EncryptionKey_FromUnknown(nullptr, &key), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} // namespace digital_signature_objects
