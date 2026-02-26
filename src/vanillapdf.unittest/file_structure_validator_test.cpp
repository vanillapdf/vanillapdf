#include "unittest.h"
#include "handle_guard.h"

namespace file_structure_validator {

// Helper to create and save a minimal valid PDF to a memory stream, then reopen and initialize it
static void CreateValidPdf(
    HandleGuard<FileHandle, File_Release>& file,
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release>& io_stream) {

    HandleGuard<FileHandle, File_Release> create_file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> create_stream;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(create_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(create_stream, "test", create_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(create_file, document.out()), VANILLAPDF_ERROR_SUCCESS);

    // Save to a new stream
    HandleGuard<FileHandle, File_Release> save_file;
    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "saved", save_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SaveFile(document, save_file), VANILLAPDF_ERROR_SUCCESS);

    // Reopen and initialize
    ASSERT_EQ(File_OpenStream(io_stream, "saved", file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_Initialize(file), VANILLAPDF_ERROR_SUCCESS);
}

TEST(FileStructureValidator, NullFileParameter) {
    HandleGuard<FileStructureValidationResultHandle, FileStructureValidationResult_Release> result;
    EXPECT_EQ(FileStructureValidator_Validate(nullptr, result.out()), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(FileStructureValidator, NullResultParameter) {
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    CreateValidPdf(file, io_stream);

    EXPECT_EQ(FileStructureValidator_Validate(file, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(FileStructureValidator, ValidPdfProducesNoErrors) {
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    CreateValidPdf(file, io_stream);

    HandleGuard<FileStructureValidationResultHandle, FileStructureValidationResult_Release> result;
    ASSERT_EQ(FileStructureValidator_Validate(file, result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(result.get(), nullptr);

    boolean_type is_valid = VANILLAPDF_RV_FALSE;
    ASSERT_EQ(FileStructureValidationResult_IsValid(result, &is_valid), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(is_valid, VANILLAPDF_RV_TRUE);

    size_type error_count = 0;
    ASSERT_EQ(FileStructureValidationResult_GetErrorCount(result, &error_count), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(error_count, static_cast<size_type>(0));
}

TEST(FileStructureValidator, ResultIssueCount) {
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    CreateValidPdf(file, io_stream);

    HandleGuard<FileStructureValidationResultHandle, FileStructureValidationResult_Release> result;
    ASSERT_EQ(FileStructureValidator_Validate(file, result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(result.get(), nullptr);

    size_type issue_count = 0;
    size_type error_count = 0;
    size_type warning_count = 0;

    ASSERT_EQ(FileStructureValidationResult_GetIssueCount(result, &issue_count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FileStructureValidationResult_GetErrorCount(result, &error_count), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(FileStructureValidationResult_GetWarningCount(result, &warning_count), VANILLAPDF_ERROR_SUCCESS);

    // Errors + warnings should not exceed total issue count
    EXPECT_LE(error_count + warning_count, issue_count);
}

TEST(FileStructureValidator, ResultToUnknownFromUnknown) {
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    CreateValidPdf(file, io_stream);

    HandleGuard<FileStructureValidationResultHandle, FileStructureValidationResult_Release> result;
    ASSERT_EQ(FileStructureValidator_Validate(file, result.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(result.get(), nullptr);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(FileStructureValidationResult_ToUnknown(result, unknown.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(unknown.get(), nullptr);

    HandleGuard<FileStructureValidationResultHandle, FileStructureValidationResult_Release> roundtrip;
    ASSERT_EQ(FileStructureValidationResult_FromUnknown(unknown, roundtrip.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_NE(roundtrip.get(), nullptr);
}

TEST(FileStructureValidator, IssueAccessorsOnValidFile) {
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    CreateValidPdf(file, io_stream);

    HandleGuard<FileStructureValidationResultHandle, FileStructureValidationResult_Release> result;
    ASSERT_EQ(FileStructureValidator_Validate(file, result.out()), VANILLAPDF_ERROR_SUCCESS);

    size_type issue_count = 0;
    ASSERT_EQ(FileStructureValidationResult_GetIssueCount(result, &issue_count), VANILLAPDF_ERROR_SUCCESS);

    // Iterate all issues and verify accessors work
    for (size_type i = 0; i < issue_count; i += 1) {
        HandleGuard<FileStructureIssueHandle, FileStructureIssue_Release> issue;
        ASSERT_EQ(FileStructureValidationResult_GetIssueAt(result, i, issue.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(issue.get(), nullptr);

        FileStructureIssueSeverityType severity = FileStructureIssueSeverity_Undefined;
        ASSERT_EQ(FileStructureIssue_GetSeverity(issue, &severity), VANILLAPDF_ERROR_SUCCESS);
        EXPECT_NE(severity, FileStructureIssueSeverity_Undefined);

        FileStructureIssueCodeType code = FileStructureIssueCode_Undefined;
        ASSERT_EQ(FileStructureIssue_GetCode(issue, &code), VANILLAPDF_ERROR_SUCCESS);
        EXPECT_NE(code, FileStructureIssueCode_Undefined);

        HandleGuard<BufferHandle, Buffer_Release> message;
        ASSERT_EQ(FileStructureIssue_GetMessage(issue, message.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_NE(message.get(), nullptr);

        biguint_type obj_num = 0;
        ushort_type gen_num = 0;
        ASSERT_EQ(FileStructureIssue_GetObjectNumber(issue, &obj_num), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(FileStructureIssue_GetGenerationNumber(issue, &gen_num), VANILLAPDF_ERROR_SUCCESS);
    }
}

TEST(FileStructureValidator, UninitializedFileReturnsError) {
    // First create a valid PDF in memory
    HandleGuard<FileHandle, File_Release> create_file;
    HandleGuard<DocumentHandle, Document_Release> document;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> create_stream;
    HandleGuard<FileHandle, File_Release> save_file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;

    ASSERT_EQ(InputOutputStream_CreateFromMemory(create_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(create_stream, "test", create_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_CreateFile(create_file, document.out()), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(File_CreateStream(io_stream, "saved", save_file.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(Document_SaveFile(document, save_file), VANILLAPDF_ERROR_SUCCESS);

    // Now open the stream as a file but do NOT call File_Initialize
    HandleGuard<FileHandle, File_Release> file;
    ASSERT_EQ(File_OpenStream(io_stream, "saved", file.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<FileStructureValidationResultHandle, FileStructureValidationResult_Release> result;
    // File is not initialized, should return an error (FileNotInitializedException)
    EXPECT_NE(FileStructureValidator_Validate(file, result.out()), VANILLAPDF_ERROR_SUCCESS);
}

} // namespace file_structure_validator
