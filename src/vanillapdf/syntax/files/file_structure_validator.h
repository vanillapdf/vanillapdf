#ifndef _FILE_STRUCTURE_VALIDATOR_H
#define _FILE_STRUCTURE_VALIDATOR_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/file_structure_validation_result.h"

namespace vanillapdf {
namespace syntax {

class FileStructureValidator {
public:
    static FileStructureValidationResultPtr Validate(FilePtr file);

private:
    static void ValidateHeader(FilePtr file, FileStructureValidationResultPtr result);
    static void ValidateXrefChain(FilePtr file, FileStructureValidationResultPtr result);
    static void ValidateTrailerDictionaries(FilePtr file, FileStructureValidationResultPtr result);
    static void ValidateXrefEntries(FilePtr file, types::stream_size file_size, FileStructureValidationResultPtr result);
    static void ValidateTrailerReferences(FilePtr file, FileStructureValidationResultPtr result);

    static void AddIssue(
        FileStructureValidationResultPtr result,
        FileStructureIssueSeverity severity,
        FileStructureIssueCode code,
        const std::string& message);

    static void AddIssue(
        FileStructureValidationResultPtr result,
        FileStructureIssueSeverity severity,
        FileStructureIssueCode code,
        const std::string& message,
        types::big_uint object_number,
        types::ushort generation_number);
};

} // syntax
} // vanillapdf

#endif /* _FILE_STRUCTURE_VALIDATOR_H */
