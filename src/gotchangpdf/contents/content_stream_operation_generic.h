#ifndef _CONTENT_STREAM_OPERATION_GENERIC_H
#define _CONTENT_STREAM_OPERATION_GENERIC_H

#include "syntax/utils/syntax_fwd.h"
#include "contents/content_stream_operation_base.h"
#include "contents/content_stream_operator_base.h"
#include "utils/constants.h"

#include <vector>

namespace gotchangpdf {
namespace contents {

class OperationGeneric : public OperationBase {
public:
	OperationGeneric() = default;
	OperationGeneric(CustomSizeVector<syntax::ObjectPtr> operands, OperatorBasePtr oper);

	OperatorBasePtr GetOperator() const;
	CustomSizeVector<syntax::ObjectPtr> GetOperands() const;

	types::size_type GetOperandsSize() const;
	syntax::ObjectPtr GetOperandAt(types::size_type at) const;

	virtual InstructionBase::Type GetInstructionType(void) const noexcept override;
	virtual OperationBase::Type GetOperationType(void) const noexcept override;
	virtual std::string ToPdf() const override;

private:
	OperatorBasePtr _operator;
	CustomSizeVector<syntax::ObjectPtr> _operands;
};

typedef CustomSizeVector<OperationGenericPtr> GenericOperationCollection;

inline InstructionBase::Type OperationGeneric::GetInstructionType(void) const noexcept {
	return InstructionBase::Type::Operation;
}

inline OperationBase::Type OperationGeneric::GetOperationType(void) const noexcept {
	return OperationBase::Type::Generic;
}

} // contents
} // gotchangpdf

#endif /* _CONTENT_STREAM_OPERATION_GENERIC_H */