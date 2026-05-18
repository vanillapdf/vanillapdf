#include "tools.h"

void print_validate_help() {
    printf("Usage: validate -f [PDF file] [options]\n");
    printf("Options:\n");
    printf("  -f [file]    PDF file to validate (required)\n");
    printf("  -l [license] License file (optional)\n");
}

int process_validate(int argc, char *argv[]) {

    int arg_counter = 0;
    string_type license_file = NULL;
    string_type pdf_file = NULL;

    FileHandle* file = NULL;
    FileStructureValidationResultHandle* result = NULL;
    boolean_type is_valid = VANILLAPDF_RV_FALSE;
    size_type issue_count = 0;
    size_type error_count = 0;
    size_type warning_count = 0;

    for (arg_counter = 0; arg_counter < argc; ++arg_counter) {

        if (strcmp(argv[arg_counter], "-f") == 0 && (arg_counter + 1 < argc)) {
            pdf_file = argv[arg_counter + 1];
            arg_counter += 1;

        } else if (strcmp(argv[arg_counter], "-l") == 0 && (arg_counter + 1 < argc)) {
            license_file = argv[arg_counter + 1];
            arg_counter += 1;

        } else {
            print_validate_help();
            return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
        }
    }

    if (pdf_file == NULL) {
        printf("Error: PDF file is required\n");
        print_validate_help();
        return VANILLAPDF_TOOLS_ERROR_INVALID_PARAMETERS;
    }

    if (license_file != NULL) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(license_file));
    }

    printf("Validating PDF file: %s\n", pdf_file);

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(pdf_file, &file));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));

    //! [Validate file structure]
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidator_Validate(file, &result));
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_IsValid(result, &is_valid));
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_GetIssueCount(result, &issue_count));
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_GetErrorCount(result, &error_count));
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_GetWarningCount(result, &warning_count));

    printf("Issues: %llu (%llu error(s), %llu warning(s))\n",
        (unsigned long long)issue_count,
        (unsigned long long)error_count,
        (unsigned long long)warning_count);

    for (size_type i = 0; i < issue_count; i++) {
        FileStructureIssueHandle* issue = NULL;
        FileStructureIssueSeverityType severity = FileStructureIssueSeverity_Undefined;
        FileStructureIssueCodeType code = FileStructureIssueCode_Undefined;
        BufferHandle* message = NULL;
        string_type data = NULL;
        size_type size = 0;

        RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_GetIssueAt(result, i, &issue));
        RETURN_ERROR_IF_NOT_SUCCESS(FileStructureIssue_GetSeverity(issue, &severity));
        RETURN_ERROR_IF_NOT_SUCCESS(FileStructureIssue_GetCode(issue, &code));
        RETURN_ERROR_IF_NOT_SUCCESS(FileStructureIssue_GetMessage(issue, &message));
        RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(message, &data, &size));

        printf("[severity=%d code=%d]: %.*s\n", (int)severity, (int)code, (int)size, data);

        if (message) Buffer_Release(message);
        if (issue) FileStructureIssue_Release(issue);
    }
    //! [Validate file structure]

    printf("\nResult: %s\n", VANILLAPDF_RV_TRUE == is_valid ? "VALID" : "INVALID");

    if (result) FileStructureValidationResult_Release(result);
    if (file) File_Release(file);

    return VANILLAPDF_RV_TRUE == is_valid
        ? VANILLAPDF_TOOLS_ERROR_SUCCESS
        : VANILLAPDF_TOOLS_ERROR_FAILURE;
}
