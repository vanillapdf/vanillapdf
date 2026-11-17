#include <stdio.h>
#include <vanillapdf/c_vanillapdf_api.h>

/* Store the result of a call and jump to the cleanup when it failed */
#define CHECK(call) \
    do { \
        rc = (call); \
        if (rc != VANILLAPDF_ERROR_SUCCESS) { \
            goto cleanup; \
        } \
    } while (0)

int main(int argc, char* argv[]) {
    FileHandle* file = NULL;
    FileStructureValidationResultHandle* result = NULL;
    FileStructureIssueHandle* issue = NULL;
    BufferHandle* message = NULL;
    boolean_type valid = VANILLAPDF_RV_FALSE;
    size_type issue_count = 0;
    error_type rc = VANILLAPDF_ERROR_SUCCESS;
    size_type i = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input.pdf>\n", argv[0]);
        return 1;
    }

    /* Validation works on the file layer: cross-reference tables, trailers and streams */
    CHECK(File_Open(argv[1], &file));
    CHECK(File_Initialize(file));

    CHECK(FileStructureValidator_Validate(file, &result));
    CHECK(FileStructureValidationResult_IsValid(result, &valid));
    CHECK(FileStructureValidationResult_GetIssueCount(result, &issue_count));

    for (i = 0; i < issue_count; i++) {
        FileStructureIssueSeverityType severity = FileStructureIssueSeverity_Undefined;
        string_type text = NULL;
        size_type text_size = 0;

        CHECK(FileStructureValidationResult_GetIssueAt(result, i, &issue));
        CHECK(FileStructureIssue_GetSeverity(issue, &severity));
        CHECK(FileStructureIssue_GetMessage(issue, &message));
        CHECK(Buffer_GetData(message, &text, &text_size));

        printf("%s: %.*s\n", severity == FileStructureIssueSeverity_Error ? "Error" : "Warning", (int) text_size, text);

        Buffer_Release(message);
        FileStructureIssue_Release(issue);

        message = NULL;
        issue = NULL;
    }

    printf("%s: %s\n", argv[1], valid ? "valid" : "not valid");

cleanup:
    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        char error_message[256] = { 0 };
        Errors_GetLastErrorMessage(error_message, sizeof(error_message));
        fprintf(stderr, "Error: %s\n", error_message);
    }

    if (message) Buffer_Release(message);
    if (issue) FileStructureIssue_Release(issue);
    if (result) FileStructureValidationResult_Release(result);
    if (file) File_Release(file);

    if (rc != VANILLAPDF_ERROR_SUCCESS) {
        return 1;
    }

    /* Exit code 2 tells a script that the file has structural errors */
    return valid ? 0 : 2;
}
