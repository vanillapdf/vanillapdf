#include "precompiled.h"

#include "syntax/files/file.h"
#include "syntax/files/file_structure_issue.h"
#include "syntax/files/file_structure_validation_result.h"
#include "syntax/files/file_structure_validator.h"

#include "vanillapdf/syntax/c_file_structure_validator.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

// FileStructureValidator

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidator_Validate(
    FileHandle* handle,
    FileStructureValidationResultHandle** result) {
    File* file = reinterpret_cast<File*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(file);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto validation_result = FileStructureValidator::Validate(file);
        auto ptr = validation_result.AddRefGet();
        *result = reinterpret_cast<FileStructureValidationResultHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

// FileStructureValidationResult

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_GetIssueCount(
    FileStructureValidationResultHandle* handle,
    size_type* result) {
    FileStructureValidationResult* validation_result = reinterpret_cast<FileStructureValidationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(validation_result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = validation_result->GetIssueCount();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_GetIssueAt(
    FileStructureValidationResultHandle* handle,
    size_type index,
    FileStructureIssueHandle** result) {
    FileStructureValidationResult* validation_result = reinterpret_cast<FileStructureValidationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(validation_result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto issue = validation_result->GetIssueAt(index);
        auto ptr = issue.AddRefGet();
        *result = reinterpret_cast<FileStructureIssueHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_IsValid(
    FileStructureValidationResultHandle* handle,
    boolean_type* result) {
    FileStructureValidationResult* validation_result = reinterpret_cast<FileStructureValidationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(validation_result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = validation_result->IsValid() ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_GetErrorCount(
    FileStructureValidationResultHandle* handle,
    size_type* result) {
    FileStructureValidationResult* validation_result = reinterpret_cast<FileStructureValidationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(validation_result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = validation_result->GetErrorCount();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_GetWarningCount(
    FileStructureValidationResultHandle* handle,
    size_type* result) {
    FileStructureValidationResult* validation_result = reinterpret_cast<FileStructureValidationResult*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(validation_result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = validation_result->GetWarningCount();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_ToUnknown(
    FileStructureValidationResultHandle* handle,
    IUnknownHandle** result) {
    return SafeObjectConvert<FileStructureValidationResult, IUnknown, FileStructureValidationResultHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_FromUnknown(
    IUnknownHandle* handle,
    FileStructureValidationResultHandle** result) {
    return SafeObjectConvert<IUnknown, FileStructureValidationResult, IUnknownHandle, FileStructureValidationResultHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_Release(
    FileStructureValidationResultHandle* handle) {
    return ObjectRelease<FileStructureValidationResult, FileStructureValidationResultHandle>(handle);
}

// FileStructureIssue

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetSeverity(
    FileStructureIssueHandle* handle,
    FileStructureIssueSeverityType* result) {
    FileStructureIssue* issue = reinterpret_cast<FileStructureIssue*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(issue);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    switch (issue->GetSeverity()) {
    case FileStructureIssueSeverity::Info:
        *result = FileStructureIssueSeverity_Info; break;
    case FileStructureIssueSeverity::Warning:
        *result = FileStructureIssueSeverity_Warning; break;
    case FileStructureIssueSeverity::Error:
        *result = FileStructureIssueSeverity_Error; break;
    default:
        *result = FileStructureIssueSeverity_Undefined; break;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetCode(
    FileStructureIssueHandle* handle,
    FileStructureIssueCodeType* result) {
    FileStructureIssue* issue = reinterpret_cast<FileStructureIssue*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(issue);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = static_cast<FileStructureIssueCodeType>(issue->GetCode());
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetMessage(
    FileStructureIssueHandle* handle,
    BufferHandle** result) {
    FileStructureIssue* issue = reinterpret_cast<FileStructureIssue*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(issue);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto message = issue->GetMessage();
        auto ptr = message.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetObjectNumber(
    FileStructureIssueHandle* handle,
    biguint_type* result) {
    FileStructureIssue* issue = reinterpret_cast<FileStructureIssue*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(issue);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = issue->GetObjectNumber();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetGenerationNumber(
    FileStructureIssueHandle* handle,
    ushort_type* result) {
    FileStructureIssue* issue = reinterpret_cast<FileStructureIssue*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(issue);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = issue->GetGenerationNumber();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_ToUnknown(
    FileStructureIssueHandle* handle,
    IUnknownHandle** result) {
    return SafeObjectConvert<FileStructureIssue, IUnknown, FileStructureIssueHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_FromUnknown(
    IUnknownHandle* handle,
    FileStructureIssueHandle** result) {
    return SafeObjectConvert<IUnknown, FileStructureIssue, IUnknownHandle, FileStructureIssueHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_Release(
    FileStructureIssueHandle* handle) {
    return ObjectRelease<FileStructureIssue, FileStructureIssueHandle>(handle);
}
