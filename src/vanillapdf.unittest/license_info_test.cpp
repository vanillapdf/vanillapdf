#include "unittest.h"
#include "handle_guard.h"

#include <cstring>

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

TEST(LicenseInfo, SetLicenseBufferInvalidData) {
    HandleGuard<BufferHandle, Buffer_Release> buffer;
    const char* data = "not a valid license";
    ASSERT_EQ(Buffer_CreateFromData(data, static_cast<size_type>(strlen(data)), buffer.out()), VANILLAPDF_ERROR_SUCCESS);

#ifdef VANILLAPDF_ENABLE_LICENSING
    // Invalid data is not valid JSON, so parsing fails
    EXPECT_EQ(LicenseInfo_SetLicenseBuffer(buffer.get()), VANILLAPDF_ERROR_GENERAL);
#else
    // Licensing disabled, SetLicense is a no-op
    EXPECT_EQ(LicenseInfo_SetLicenseBuffer(buffer.get()), VANILLAPDF_ERROR_SUCCESS);
#endif
}

TEST(LicenseInfo, SetLicenseFileNonExistent) {
#ifdef VANILLAPDF_ENABLE_LICENSING
    // File does not exist, so opening fails
    EXPECT_EQ(LicenseInfo_SetLicenseFile("nonexistent_license_file.json"), VANILLAPDF_ERROR_GENERAL);
#else
    // Licensing disabled, SetLicense is a no-op
    EXPECT_EQ(LicenseInfo_SetLicenseFile("nonexistent_license_file.json"), VANILLAPDF_ERROR_SUCCESS);
#endif
}

} /* license_info */
