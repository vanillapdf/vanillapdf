#include "unittest.h"
#include "handle_guard.h"

namespace utils {

class BufferParamTest : public ::testing::TestWithParam<std::string_view> {
};

TEST_P(BufferParamTest, CreateFromData) {
    auto data = GetParam();

    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;
    string_type check_data_ptr = nullptr;
    size_type check_data_len = 0;

    // Create buffer with data
    ASSERT_EQ(Buffer_CreateFromData(data.data(), data.size(), buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);

    // Read data information from buffer
    ASSERT_EQ(Buffer_GetData(buffer_ptr, &check_data_ptr, &check_data_len), VANILLAPDF_ERROR_SUCCESS);

    // Verify the data and length returned by Buffer_GetData
    ASSERT_EQ(check_data_len, data.size());
    for (uint32_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(check_data_ptr[i], data[i]);
    }
}

INSTANTIATE_TEST_SUITE_P(
    /* empty */,
    BufferParamTest,
    ::testing::Values(
        "",
        "Hello",
        "1234567890",
        "special_chars!@#$%^&*()",
        "\x00\x01\xFE\xFF"
    )
);

TEST(Buffer, CreateRelease) {

    HandleGuard<BufferHandle, Buffer_Release> buffer_ptr;

    ASSERT_EQ(Buffer_Create(buffer_ptr.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_ptr.get(), nullptr);
}

TEST(Buffer, NullCheck) {
    EXPECT_EQ(Buffer_Create(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(Buffer_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(Buffer, Conversion) {
    HandleGuard<BufferHandle, Buffer_Release> buffer_handle;
    HandleGuard<IUnknownHandle, IUnknown_Release> unknown_buffer_handle;

    ASSERT_EQ(Buffer_Create(buffer_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(buffer_handle.get(), nullptr);

    ASSERT_EQ(Buffer_ToUnknown(buffer_handle, unknown_buffer_handle.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown_buffer_handle.get(), nullptr);
}

void SetCheckLoggingSeverity(LoggingSeverity desired_severity) {
    LoggingSeverity logging_severity = LoggingSeverity_Undefined;

    ASSERT_EQ(Logging_SetSeverity(desired_severity), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Logging_GetSeverity(&logging_severity), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(logging_severity, desired_severity);
}

TEST(Logging, Severity) {
    SetCheckLoggingSeverity(LoggingSeverity_Trace);
    SetCheckLoggingSeverity(LoggingSeverity_Debug);
    SetCheckLoggingSeverity(LoggingSeverity_Info);
    SetCheckLoggingSeverity(LoggingSeverity_Warning);
    SetCheckLoggingSeverity(LoggingSeverity_Error);
    SetCheckLoggingSeverity(LoggingSeverity_Critical);
    SetCheckLoggingSeverity(LoggingSeverity_Off);
}

} /* utils */

namespace errors {

void CheckPrintableErrorText(error_type value, std::string_view expected) {
    size_type text_size = 0;

    // Get length of the text needed
    ASSERT_EQ(Errors_GetPrintableErrorTextLength(value, &text_size), VANILLAPDF_ERROR_SUCCESS);

    std::string error_text;
    error_text.resize(text_size);

    // Extract actual error text representation
    ASSERT_EQ(Errors_GetPrintableErrorText(value, error_text.data(), text_size), VANILLAPDF_ERROR_SUCCESS);

    // Compare the values
    EXPECT_STREQ(error_text.data(), expected.data());
}

TEST(Errors, CheckPrintableErrors) {

    // global error states
    CheckPrintableErrorText(VANILLAPDF_ERROR_SUCCESS, "VANILLAPDF_ERROR_SUCCESS");
    CheckPrintableErrorText(VANILLAPDF_ERROR_PARAMETER_VALUE, "VANILLAPDF_ERROR_PARAMETER_VALUE");
    CheckPrintableErrorText(VANILLAPDF_ERROR_NOT_SUPPORTED, "VANILLAPDF_ERROR_NOT_SUPPORTED");
    CheckPrintableErrorText(VANILLAPDF_ERROR_USER_CANCELLED, "VANILLAPDF_ERROR_USER_CANCELLED");
    CheckPrintableErrorText(VANILLAPDF_ERROR_DATA_CORRUPTION, "VANILLAPDF_ERROR_DATA_CORRUPTION");
    CheckPrintableErrorText(VANILLAPDF_ERROR_IO, "VANILLAPDF_ERROR_IO");
    CheckPrintableErrorText(VANILLAPDF_ERROR_CRYPTO, "VANILLAPDF_ERROR_CRYPTO");
    CheckPrintableErrorText(VANILLAPDF_ERROR_IMAGE_CODEC, "VANILLAPDF_ERROR_IMAGE_CODEC");
    CheckPrintableErrorText(VANILLAPDF_ERROR_INVALID_LICENSE, "VANILLAPDF_ERROR_INVALID_LICENSE");
    CheckPrintableErrorText(VANILLAPDF_ERROR_LICENSE_REQUIRED, "VANILLAPDF_ERROR_LICENSE_REQUIRED");
    CheckPrintableErrorText(VANILLAPDF_ERROR_INSUFFICIENT_SPACE, "VANILLAPDF_ERROR_INSUFFICIENT_SPACE");
    CheckPrintableErrorText(VANILLAPDF_ERROR_GENERAL, "VANILLAPDF_ERROR_GENERAL");

    // syntax errors
    CheckPrintableErrorText(VANILLAPDF_ERROR_CONVERSION, "VANILLAPDF_ERROR_CONVERSION");
    CheckPrintableErrorText(VANILLAPDF_ERROR_FILE_DISPOSED, "VANILLAPDF_ERROR_FILE_DISPOSED");
    CheckPrintableErrorText(VANILLAPDF_ERROR_FILE_NOT_INITIALIZED, "VANILLAPDF_ERROR_FILE_NOT_INITIALIZED");
    CheckPrintableErrorText(VANILLAPDF_ERROR_OBJECT_MISSING, "VANILLAPDF_ERROR_OBJECT_MISSING");
    CheckPrintableErrorText(VANILLAPDF_ERROR_PARSE_EXCEPTION, "VANILLAPDF_ERROR_PARSE_EXCEPTION");
    CheckPrintableErrorText(VANILLAPDF_ERROR_INVALID_PASSWORD, "VANILLAPDF_ERROR_INVALID_PASSWORD");
    CheckPrintableErrorText(VANILLAPDF_ERROR_DUPLICATE_KEY, "VANILLAPDF_ERROR_DUPLICATE_KEY");

    // semantic
    CheckPrintableErrorText(VANILLAPDF_ERROR_OPTIONAL_ENTRY_MISSING, "VANILLAPDF_ERROR_OPTIONAL_ENTRY_MISSING");
    CheckPrintableErrorText(VANILLAPDF_ERROR_SEMANTIC_CONTEXT, "VANILLAPDF_ERROR_SEMANTIC_CONTEXT");
}

} /* errors */
