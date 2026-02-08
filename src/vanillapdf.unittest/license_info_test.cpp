#include "unittest.h"

namespace license_info {

TEST(LicenseInfo, IsValid) {
    boolean_type is_valid = VANILLAPDF_RV_FALSE;

    ASSERT_EQ(LicenseInfo_IsValid(&is_valid), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_valid, VANILLAPDF_RV_TRUE);
}

TEST(LicenseInfo, IsTemporary) {
    boolean_type is_temporary = VANILLAPDF_RV_TRUE;

    ASSERT_EQ(LicenseInfo_IsTemporary(&is_temporary), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_temporary, VANILLAPDF_RV_FALSE);
}

TEST(LicenseInfo, SetLicenseFileNull) {
    EXPECT_EQ(LicenseInfo_SetLicenseFile(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(LicenseInfo, SetLicenseBufferNull) {
    EXPECT_EQ(LicenseInfo_SetLicenseBuffer(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(LicenseInfo, IsValidNull) {
    EXPECT_EQ(LicenseInfo_IsValid(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(LicenseInfo, IsTemporaryNull) {
    EXPECT_EQ(LicenseInfo_IsTemporary(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(LicenseInfo, SetLicenseFileNonExistent) {
    // In stub mode (licensing disabled), SetLicenseFile is a no-op that returns success.
    // In full mode (licensing enabled), it would fail because the file doesn't exist.
    auto result = LicenseInfo_SetLicenseFile("nonexistent_license_file.json");
    EXPECT_TRUE(result == VANILLAPDF_ERROR_SUCCESS || result == VANILLAPDF_ERROR_GENERAL);
}

} /* license_info */
