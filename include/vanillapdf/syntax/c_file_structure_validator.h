#ifndef _C_FILE_STRUCTURE_VALIDATOR_H
#define _C_FILE_STRUCTURE_VALIDATOR_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_file_structure_validator.h
    * \brief This file contains APIs for PDF file structure validation.
    */

    /**
    * \class FileStructureIssueHandle
    * \extends IUnknownHandle
    * \ingroup group_files
    * \brief Represents a single structural issue found during validation.
    */

    /**
    * \class FileStructureValidationResultHandle
    * \extends IUnknownHandle
    * \ingroup group_files
    * \brief Container for all issues found during file structure validation.
    */

    /**
    * \brief Severity level of a file structure issue.
    * \ingroup group_files
    */
    typedef enum {
        FileStructureIssueSeverity_Undefined = 0, /**< Severity not set (uninitialized) */
        FileStructureIssueSeverity_Info,           /**< Informational observation */
        FileStructureIssueSeverity_Warning,        /**< Non-critical issue */
        FileStructureIssueSeverity_Error           /**< Critical structural error */
    } FileStructureIssueSeverityType;

    /**
    * \brief Code identifying the specific type of structural issue.
    * \ingroup group_files
    */
    typedef enum {
        FileStructureIssueCode_Undefined = 0,                             /**< Code not set (uninitialized) */

        /* Header checks (100s) */
        FileStructureIssueCode_HeaderMissing = 100,                       /**< File header is missing */
        FileStructureIssueCode_HeaderVersionUndefined = 101,              /**< Header version is undefined */

        /* Trailer checks (200s) */
        FileStructureIssueCode_TrailerMissingSize = 200,                  /**< Trailer missing /Size entry */
        FileStructureIssueCode_TrailerMissingRoot = 201,                  /**< Trailer missing /Root entry */
        FileStructureIssueCode_TrailerSizeMismatch = 202,                 /**< Trailer /Size does not match xref */
        FileStructureIssueCode_TrailerMissing = 203,                      /**< Trailer dictionary is missing entirely */

        /* Xref entry checks (300s) */
        FileStructureIssueCode_XrefUsedEntryBadOffset = 300,              /**< Used entry has invalid offset */
        FileStructureIssueCode_XrefUsedEntryOffsetExceedsFileSize = 301,  /**< Used entry offset exceeds file size */
        FileStructureIssueCode_XrefCompressedEntryMissingObjectStream = 302, /**< Compressed entry references missing object stream */
        FileStructureIssueCode_XrefFreeEntryInvalidNextFree = 303,        /**< Free entry references invalid next free object */

        /* Object reference checks (400s) */
        FileStructureIssueCode_TrailerRootReferenceNotInXref = 400,       /**< /Root target not in xref */
        FileStructureIssueCode_TrailerInfoReferenceNotInXref = 401,       /**< /Info target not in xref */

        /* Xref chain checks (500s) */
        FileStructureIssueCode_XrefChainEmpty = 500                       /**< Xref chain has no sections */
    } FileStructureIssueCodeType;

    /**
    * \class FileStructureValidator
    * \ingroup group_files
    * \brief Static utility class for validating PDF file structure.
    *
    * \note This validator operates on the in-memory structures produced by
    * \ref File_Initialize. It requires the file to be successfully initialized
    * before validation can run.
    *
    * In practice, initialization is resilient: when the primary xref parsing
    * fails, the parser falls back to a full-file object scan (\c FindAllObjects),
    * which recovers most malformed files. Only files that fail even the fallback
    * (e.g. those using unsupported features such as unknown encryption) cannot
    * be initialized and therefore cannot be validated. For those cases the
    * initialization error itself is the diagnostic signal.
    */

    /**
    * \memberof FileStructureValidator
    * @{
    */

    /**
    * \brief Validate the structure of an initialized PDF file.
    * \param handle An initialized file handle
    * \param result Output validation result
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidator_Validate(
        FileHandle* handle,
        FileStructureValidationResultHandle** result
    );

    /** @} */

    /**
    * \memberof FileStructureValidationResultHandle
    * @{
    */

    /**
    * \brief Get the total number of issues found.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_GetIssueCount(
        FileStructureValidationResultHandle* handle,
        size_type* result
    );

    /**
    * \brief Get an issue at the specified index.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_GetIssueAt(
        FileStructureValidationResultHandle* handle,
        size_type index,
        FileStructureIssueHandle** result
    );

    /**
    * \brief Check if the file is structurally valid (no errors).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_IsValid(
        FileStructureValidationResultHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Get the number of error-severity issues.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_GetErrorCount(
        FileStructureValidationResultHandle* handle,
        size_type* result
    );

    /**
    * \brief Get the number of warning-severity issues.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_GetWarningCount(
        FileStructureValidationResultHandle* handle,
        size_type* result
    );

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_ToUnknown(
        FileStructureValidationResultHandle* handle,
        IUnknownHandle** result
    );

    /**
    * \brief Convert \ref IUnknownHandle to \ref FileStructureValidationResultHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_FromUnknown(
        IUnknownHandle* handle,
        FileStructureValidationResultHandle** result
    );

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureValidationResult_Release(
        FileStructureValidationResultHandle* handle
    );

    /** @} */

    /**
    * \memberof FileStructureIssueHandle
    * @{
    */

    /**
    * \brief Get the severity of the issue.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetSeverity(
        FileStructureIssueHandle* handle,
        FileStructureIssueSeverityType* result
    );

    /**
    * \brief Get the issue code.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetCode(
        FileStructureIssueHandle* handle,
        FileStructureIssueCodeType* result
    );

    /**
    * \brief Get the human-readable issue message.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetMessage(
        FileStructureIssueHandle* handle,
        BufferHandle** result
    );

    /**
    * \brief Get the object number associated with this issue (0 if not applicable).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetObjectNumber(
        FileStructureIssueHandle* handle,
        biguint_type* result
    );

    /**
    * \brief Get the generation number associated with this issue (0 if not applicable).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_GetGenerationNumber(
        FileStructureIssueHandle* handle,
        ushort_type* result
    );

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_ToUnknown(
        FileStructureIssueHandle* handle,
        IUnknownHandle** result
    );

    /**
    * \brief Convert \ref IUnknownHandle to \ref FileStructureIssueHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_FromUnknown(
        IUnknownHandle* handle,
        FileStructureIssueHandle** result
    );

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FileStructureIssue_Release(
        FileStructureIssueHandle* handle
    );

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_STRUCTURE_VALIDATOR_H */
