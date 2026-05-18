#ifndef _FILE_STRUCTURE_ISSUE_H
#define _FILE_STRUCTURE_ISSUE_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/unknown_interface.h"

#include <optional>

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
    // File-level issue — no object context
    FileStructureIssue(
        FileStructureIssueSeverity severity,
        FileStructureIssueCode code,
        BufferPtr message);

    // Object-scoped issue — carries object number and generation number
    FileStructureIssue(
        FileStructureIssueSeverity severity,
        FileStructureIssueCode code,
        BufferPtr message,
        types::big_uint object_number,
        types::ushort generation_number);

    FileStructureIssueSeverity GetSeverity() const noexcept { return m_severity; }
    FileStructureIssueCode GetCode() const noexcept { return m_code; }
    BufferPtr GetMessage() const noexcept { return m_message; }
    bool HasObjectContext() const noexcept { return m_object_number.has_value(); }
    std::optional<types::big_uint> GetObjectNumber() const noexcept { return m_object_number; }
    std::optional<types::ushort> GetGenerationNumber() const noexcept { return m_generation_number; }

private:
    FileStructureIssueSeverity m_severity = FileStructureIssueSeverity::Undefined;
    FileStructureIssueCode m_code = FileStructureIssueCode::Undefined;
    BufferPtr m_message;
    std::optional<types::big_uint> m_object_number;
    std::optional<types::ushort> m_generation_number;
};

} // syntax
} // vanillapdf

#endif /* _FILE_STRUCTURE_ISSUE_H */
