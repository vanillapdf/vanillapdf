#ifndef _FILE_STRUCTURE_ISSUE_H
#define _FILE_STRUCTURE_ISSUE_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/unknown_interface.h"

namespace vanillapdf {
namespace syntax {

enum class FileStructureIssueSeverity {
    Undefined = 0,
    Info,
    Warning,
    Error
};

enum class FileStructureIssueCode {
    Undefined = 0,

    // Header checks (100s)
    HeaderMissing = 100,
    HeaderVersionUndefined = 101,

    // Trailer checks (200s)
    TrailerMissingSize = 200,
    TrailerMissingRoot = 201,
    TrailerSizeMismatch = 202,
    TrailerMissing = 203,

    // Xref entry checks (300s)
    XrefUsedEntryBadOffset = 300,
    XrefUsedEntryOffsetExceedsFileSize = 301,
    XrefCompressedEntryMissingObjectStream = 302,
    XrefFreeEntryInvalidNextFree = 303,

    // Object reference checks (400s)
    TrailerRootReferenceNotInXref = 400,
    TrailerInfoReferenceNotInXref = 401,

    // Xref chain checks (500s)
    XrefChainEmpty = 500
};

class FileStructureIssue : public IUnknown {
public:
    FileStructureIssue(
        FileStructureIssueSeverity severity,
        FileStructureIssueCode code,
        BufferPtr message,
        types::big_uint object_number,
        types::ushort generation_number);

    FileStructureIssueSeverity GetSeverity() const noexcept { return m_severity; }
    FileStructureIssueCode GetCode() const noexcept { return m_code; }
    BufferPtr GetMessage() const noexcept { return m_message; }
    types::big_uint GetObjectNumber() const noexcept { return m_object_number; }
    types::ushort GetGenerationNumber() const noexcept { return m_generation_number; }

private:
    FileStructureIssueSeverity m_severity = FileStructureIssueSeverity::Undefined;
    FileStructureIssueCode m_code = FileStructureIssueCode::Undefined;
    BufferPtr m_message;
    types::big_uint m_object_number = 0;
    types::ushort m_generation_number = 0;
};

} // syntax
} // vanillapdf

#endif /* _FILE_STRUCTURE_ISSUE_H */
