#ifndef _CONTENT_STREAM_OPERATION_GENERIC_H
#define _CONTENT_STREAM_OPERATION_GENERIC_H

#include "syntax/utils/syntax_fwd.h"
#include "contents/content_stream_operation_base.h"
#include "contents/content_stream_operator_base.h"
#include "utils/constants.h"

#include <vector>

namespace vanillapdf {
namespace contents {

class OperationGeneric : public OperationBase {
public:
    OperationGeneric(const std::vector<syntax::ObjectPtr>& operands, OperatorBasePtr oper);

    OperatorBasePtr GetOperator() const;
    std::vector<syntax::ObjectPtr> GetOperands() const;

    types::size_type GetOperandsSize() const;
    syntax::ObjectPtr GetOperandAt(types::size_type at) const;

    virtual OperationBase::Type GetOperationType(void) const noexcept override;
    virtual std::string ToPdf() const override;

private:
    OperatorBasePtr _operator;
    std::vector<syntax::ObjectPtr> _operands;
};

typedef std::vector<OperationGenericPtr> GenericOperationCollection;

inline OperationBase::Type OperationGeneric::GetOperationType(void) const noexcept {
    return OperationBase::Type::Generic;
}

} // contents
} // vanillapdf

#endif /* _CONTENT_STREAM_OPERATION_GENERIC_H */