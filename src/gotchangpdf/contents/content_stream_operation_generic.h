#ifndef _CONTENT_STREAM_OPERATION_GENERIC_H
#define _CONTENT_STREAM_OPERATION_GENERIC_H

#include "syntax/utils/syntax_fwd.h"
#include "contents/content_stream_operation_base.h"
#include "contents/content_stream_operators.h"
#include "utils/constants.h"

#include <vector>

namespace gotchangpdf {
namespace contents {

class OperationGeneric : public OperationBase {
public:
	OperationGeneric() = default;
	OperationGeneric(std::vector<syntax::ObjectPtr> operands, OperatorBasePtr oper);

	OperatorBasePtr GetOperator() const;
	std::vector<syntax::ObjectPtr> GetOperands() const;

	types::size_type GetOperandsSize() const;
	syntax::ObjectPtr GetOperandAt(types::size_type at) const;

	virtual InstructionBase::Type GetInstructionType(void) const noexcept override;
	virtual OperationBase::Type GetOperationType(void) const noexcept override;
	virtual std::string ToPdf() const override;

private:
	OperatorBasePtr _operator;
	std::vector<syntax::ObjectPtr> _operands;
};

typedef std::vector<OperationGenericPtr> GenericOperationCollection;

inline InstructionBase::Type OperationGeneric::GetInstructionType(void) const noexcept {
	return InstructionBase::Type::Operation;
}

inline OperationBase::Type OperationGeneric::GetOperationType(void) const noexcept {
	return OperationBase::Type::Generic;
}

} // contents
} // gotchangpdf

#endif /* _CONTENT_STREAM_OPERATION_GENERIC_H */