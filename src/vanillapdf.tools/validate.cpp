#include "tools.h"

namespace {

struct ValidateOptions {
    std::string file;
    std::string license_file;
};

int process_validate(const ValidateOptions& options) {
    FileGuard file;
    FileStructureValidationResultGuard result;
    boolean_type is_valid = VANILLAPDF_RV_FALSE;
    size_type issue_count = 0;
    size_type error_count = 0;
    size_type warning_count = 0;

    if (!options.license_file.empty()) {
        RETURN_ERROR_IF_NOT_SUCCESS(LicenseInfo_SetLicenseFile(options.license_file.c_str()));
    }

    printf("Validating PDF file: %s\n", options.file.c_str());

    RETURN_ERROR_IF_NOT_SUCCESS(File_Open(options.file.c_str(), file.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(File_Initialize(file));

    //! [Validate file structure]
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidator_Validate(file, result.out()));
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_IsValid(result, &is_valid));
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_GetIssueCount(result, &issue_count));
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_GetErrorCount(result, &error_count));
    RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_GetWarningCount(result, &warning_count));

    printf("Issues: %llu (%llu error(s), %llu warning(s))\n",
        (unsigned long long)issue_count,
        (unsigned long long)error_count,
        (unsigned long long)warning_count);

    for (size_type i = 0; i < issue_count; i++) {
        FileStructureIssueGuard issue;
        FileStructureIssueSeverityType severity = FileStructureIssueSeverity_Undefined;
        FileStructureIssueCodeType code = FileStructureIssueCode_Undefined;
        BufferGuard message;
        string_type data = nullptr;
        size_type size = 0;

        RETURN_ERROR_IF_NOT_SUCCESS(FileStructureValidationResult_GetIssueAt(result, i, issue.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(FileStructureIssue_GetSeverity(issue, &severity));
        RETURN_ERROR_IF_NOT_SUCCESS(FileStructureIssue_GetCode(issue, &code));
        RETURN_ERROR_IF_NOT_SUCCESS(FileStructureIssue_GetMessage(issue, message.out()));
        RETURN_ERROR_IF_NOT_SUCCESS(Buffer_GetData(message, &data, &size));

        printf("[severity=%d code=%d]: %.*s\n", (int)severity, (int)code, (int)size, data);
    }
    //! [Validate file structure]

    printf("\nResult: %s\n", VANILLAPDF_RV_TRUE == is_valid ? "VALID" : "INVALID");

    return VANILLAPDF_RV_TRUE == is_valid
        ? VANILLAPDF_TOOLS_ERROR_SUCCESS
        : VANILLAPDF_TOOLS_ERROR_FAILURE;
}

} // namespace

void register_validate(CLI::App& app, int& exit_code) {
    auto options = std::make_shared<ValidateOptions>();
    auto* command = app.add_subcommand("validate", "Validate PDF file structure");

    command->add_option("-f,--file", options->file, "PDF file to validate")->required();
    command->add_option("-l,--license", options->license_file, "License file");

    command->callback([options, &exit_code]() {
        exit_code = process_validate(*options);
    });
}
