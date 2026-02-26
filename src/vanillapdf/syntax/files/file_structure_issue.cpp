#include "precompiled.h"

#include "syntax/files/file_structure_issue.h"

namespace vanillapdf {
namespace syntax {

FileStructureIssue::FileStructureIssue(
    FileStructureIssueSeverity severity,
    FileStructureIssueCode code,
    BufferPtr message)
    : m_severity(severity)
    , m_code(code)
    , m_message(message) {
}

FileStructureIssue::FileStructureIssue(
    FileStructureIssueSeverity severity,
    FileStructureIssueCode code,
    BufferPtr message,
    types::big_uint object_number,
    types::ushort generation_number)
    : m_severity(severity)
    , m_code(code)
    , m_message(message)
    , m_object_number(object_number)
    , m_generation_number(generation_number) {
}

} // syntax
} // vanillapdf
