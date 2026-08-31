#include "precompiled.h"

#include "syntax/files/file_structure_validation_result.h"

namespace vanillapdf {
namespace syntax {

void FileStructureValidationResult::AddIssue(FileStructureIssuePtr issue) {
    auto severity = issue->GetSeverity();
    if (severity == FileStructureIssueSeverity::Error) {
        m_error_count += 1;
    } else if (severity == FileStructureIssueSeverity::Warning) {
        m_warning_count += 1;
    }

    m_issues.push_back(issue);
}

FileStructureIssuePtr FileStructureValidationResult::GetIssueAt(types::size_type index) const {
    if (index >= m_issues.size()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "Issue index {} is out of range, the result holds {} issue(s)", index, m_issues.size());
    }

    return m_issues[index];
}

} // syntax
} // vanillapdf
