#include "unittest.h"

namespace date {

TEST(Date, CreateEmptyAndSetValues) {
    DateHandle* date = nullptr;
    integer_type year = 0;
    integer_type month = 0;
    integer_type day = 0;

    // Create empty date
    ASSERT_EQ(Date_CreateEmpty(&date), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(date, nullptr);

    // Set values
    ASSERT_EQ(Date_SetYear(date, 2025), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetMonth(date, 6), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetDay(date, 15), VANILLAPDF_ERROR_SUCCESS);

    // Get and verify values
    ASSERT_EQ(Date_GetYear(date, &year), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(year, 2025);

    ASSERT_EQ(Date_GetMonth(date, &month), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(month, 6);

    ASSERT_EQ(Date_GetDay(date, &day), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(day, 15);

    // Cleanup
    ASSERT_EQ(Date_Release(date), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Date, CreateCurrentAndModify) {
    DateHandle* date = nullptr;
    integer_type year = 0;
    integer_type month = 0;

    // Create current date
    ASSERT_EQ(Date_CreateCurrent(&date), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(date, nullptr);

    // Modify year
    ASSERT_EQ(Date_SetYear(date, 1999), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_GetYear(date, &year), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(year, 1999);

    // Modify month
    ASSERT_EQ(Date_SetMonth(date, 12), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_GetMonth(date, &month), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(month, 12);

    // Cleanup
    ASSERT_EQ(Date_Release(date), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Date, SetAllTimeComponents) {
    DateHandle* date = nullptr;
    integer_type hour = 0;
    integer_type minute = 0;
    integer_type second = 0;

    // Create current date
    ASSERT_EQ(Date_CreateCurrent(&date), VANILLAPDF_ERROR_SUCCESS);

    // Set time components
    ASSERT_EQ(Date_SetHour(date, 14), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetMinute(date, 30), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetSecond(date, 45), VANILLAPDF_ERROR_SUCCESS);

    // Get and verify
    ASSERT_EQ(Date_GetHour(date, &hour), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(hour, 14);

    ASSERT_EQ(Date_GetMinute(date, &minute), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(minute, 30);

    ASSERT_EQ(Date_GetSecond(date, &second), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(second, 45);

    // Cleanup
    ASSERT_EQ(Date_Release(date), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Date, SetTimezoneAndOffsets) {
    DateHandle* date = nullptr;
    TimezoneType timezone = TimezoneType_Undefined;
    integer_type hour_offset = 0;
    integer_type minute_offset = 0;

    // Create current date
    ASSERT_EQ(Date_CreateCurrent(&date), VANILLAPDF_ERROR_SUCCESS);

    // Set timezone to later (positive offset)
    ASSERT_EQ(Date_SetTimezone(date, TimezoneType_Later), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetHourOffset(date, 5), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetMinuteOffset(date, 30), VANILLAPDF_ERROR_SUCCESS);

    // Get and verify
    ASSERT_EQ(Date_GetTimezone(date, &timezone), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(timezone, TimezoneType_Later);

    ASSERT_EQ(Date_GetHourOffset(date, &hour_offset), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(hour_offset, 5);

    ASSERT_EQ(Date_GetMinuteOffset(date, &minute_offset), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(minute_offset, 30);

    // Cleanup
    ASSERT_EQ(Date_Release(date), VANILLAPDF_ERROR_SUCCESS);
}

TEST(Date, ToAndFromUnknown) {
    DateHandle* date = nullptr;
    IUnknownHandle* unknown = nullptr;
    DateHandle* converted = nullptr;
    integer_type year = 0;

    // Create and set date
    ASSERT_EQ(Date_CreateCurrent(&date), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_SetYear(date, 2020), VANILLAPDF_ERROR_SUCCESS);

    // Convert to unknown
    ASSERT_EQ(Date_ToUnknown(date, &unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown, nullptr);

    // Convert back
    ASSERT_EQ(Date_FromUnknown(unknown, &converted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(converted, nullptr);

    // Verify year preserved
    ASSERT_EQ(Date_GetYear(converted, &year), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(year, 2020);

    // Cleanup
    ASSERT_EQ(Date_Release(converted), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(IUnknown_Release(unknown), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Date_Release(date), VANILLAPDF_ERROR_SUCCESS);
}

} // namespace date
