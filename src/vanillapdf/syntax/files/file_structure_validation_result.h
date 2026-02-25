#ifndef _FILE_STRUCTURE_VALIDATION_RESULT_H
#define _FILE_STRUCTURE_VALIDATION_RESULT_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/files/file_structure_issue.h"

#include <vector>

namespace vanillapdf {
namespace syntax {

class FileStructureValidationResult : public IUnknown {
public:
    void AddIssue(FileStructureIssuePtr issue);

    types::size_type GetIssueCount() const noexcept { return m_issues.size(); }
    FileStructureIssuePtr GetIssueAt(types::size_type index) const;

    bool IsValid() const noexcept { return m_error_count == 0; }
    types::size_type GetErrorCount() const noexcept { return m_error_count; }
    types::size_type GetWarningCount() const noexcept { return m_warning_count; }

private:
    std::vector<FileStructureIssuePtr> m_issues;
    types::size_type m_error_count = 0;
    types::size_type m_warning_count = 0;
};

} // syntax
} // vanillapdf

#endif /* _FILE_STRUCTURE_VALIDATION_RESULT_H */
