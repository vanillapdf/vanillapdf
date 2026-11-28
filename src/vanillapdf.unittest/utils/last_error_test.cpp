#include "unittest.h"
#include "handle_guard.h"

#include <cstring>
#include <string>
#include <thread>

namespace utils {

namespace {

error_type GetLastErrorCode() {
    error_type result = VANILLAPDF_ERROR_SUCCESS;
    EXPECT_EQ(Errors_GetLastError(&result), VANILLAPDF_ERROR_SUCCESS);
    return result;
}

std::string GetLastErrorMessage() {
    size_type message_length = 0;
    EXPECT_EQ(Errors_GetLastErrorMessageLength(&message_length), VANILLAPDF_ERROR_SUCCESS);

    std::string message(message_length, '\0');
    EXPECT_EQ(Errors_GetLastErrorMessage(message.data(), message_length), VANILLAPDF_ERROR_SUCCESS);

    // The length includes the terminating NUL character
    if (message_length > 0) {
        message.resize(message_length - 1);
    }

    return message;
}

// Stores VANILLAPDF_ERROR_IO from an exception as the last error of the thread
void SetExceptionLastError() {
    HandleGuard<FileHandle, File_Release> file;
    ASSERT_EQ(File_Open("last-error-test-file-does-not-exist.pdf", file.out()), VANILLAPDF_ERROR_IO);
    ASSERT_EQ(GetLastErrorCode(), VANILLAPDF_ERROR_IO);
}

} // namespace

TEST(LastError, NullParameterAfterException) {
    SetExceptionLastError();

    bigint_type value = 0;
    ASSERT_EQ(IntegerObject_GetIntegerValue(nullptr, &value), VANILLAPDF_ERROR_PARAMETER_VALUE);

    EXPECT_EQ(GetLastErrorCode(), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(LastError, NullParameterOnNewThread) {
    error_type returned = VANILLAPDF_ERROR_SUCCESS;
    error_type last_error = VANILLAPDF_ERROR_SUCCESS;

    std::thread worker([&returned, &last_error]() {
        bigint_type value = 0;
        returned = IntegerObject_GetIntegerValue(nullptr, &value);
        last_error = GetLastErrorCode();
    });

    worker.join();

    EXPECT_EQ(returned, VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(last_error, VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(LastError, DirectReturnAfterException) {
    const char data[] = "abc";

    HandleGuard<BufferHandle, Buffer_Release> buffer;
    ASSERT_EQ(Buffer_CreateFromData(data, std::strlen(data), buffer.out()), VANILLAPDF_ERROR_SUCCESS);

    SetExceptionLastError();

    char destination[1] = {0};
    ASSERT_EQ(Buffer_CopyTo(buffer, destination, sizeof(destination)), VANILLAPDF_ERROR_INSUFFICIENT_SPACE);

    EXPECT_EQ(GetLastErrorCode(), VANILLAPDF_ERROR_INSUFFICIENT_SPACE);
}

TEST(LastError, ObjectMissingAfterException) {
    HandleGuard<DocumentSignatureSettingsHandle, DocumentSignatureSettings_Release> settings;
    ASSERT_EQ(DocumentSignatureSettings_Create(settings.out()), VANILLAPDF_ERROR_SUCCESS);

    SetExceptionLastError();

    HandleGuard<LiteralStringObjectHandle, LiteralStringObject_Release> name;
    ASSERT_EQ(DocumentSignatureSettings_GetName(settings, name.out()), VANILLAPDF_ERROR_OBJECT_MISSING);

    EXPECT_EQ(GetLastErrorCode(), VANILLAPDF_ERROR_OBJECT_MISSING);
}

TEST(LastError, TruncatedMessageIsTerminated) {

    // The exception message contains the path, so it is longer than the message buffer
    std::string path(300, 'a');
    path.append(".pdf");

    HandleGuard<FileHandle, File_Release> file;
    ASSERT_EQ(File_Open(path.c_str(), file.out()), VANILLAPDF_ERROR_IO);

    size_type message_length = 0;
    ASSERT_EQ(Errors_GetLastErrorMessageLength(&message_length), VANILLAPDF_ERROR_SUCCESS);

    // 255 characters and the terminating NUL character
    ASSERT_EQ(message_length, 256u);

    std::string message(512, 'X');
    ASSERT_EQ(Errors_GetLastErrorMessage(message.data(), message.size()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(message[message_length - 1], '\0');
    EXPECT_EQ(std::strlen(message.c_str()), message_length - 1);
}

TEST(LastError, TrivialFunctionKeepsLastError) {
    SetExceptionLastError();
    auto message = GetLastErrorMessage();

    HandleGuard<ObjectHandle, Object_Release> object;
    ASSERT_EQ(IntegerObject_ToObject(nullptr, object.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);

    EXPECT_EQ(GetLastErrorCode(), VANILLAPDF_ERROR_IO);
    EXPECT_EQ(GetLastErrorMessage(), message);
}

TEST(LastError, ErrorsFunctionKeepsLastError) {
    SetExceptionLastError();
    auto message = GetLastErrorMessage();

    char too_small[1] = {0};
    ASSERT_EQ(Errors_GetLastErrorMessage(too_small, sizeof(too_small)), VANILLAPDF_ERROR_GENERAL);

    EXPECT_EQ(GetLastErrorCode(), VANILLAPDF_ERROR_IO);
    EXPECT_EQ(GetLastErrorMessage(), message);
}

} // namespace utils
