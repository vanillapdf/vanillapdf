#ifndef _CONTENT_STREAM_OPERATION_GENERIC_H
#define _CONTENT_STREAM_OPERATION_GENERIC_H

#include "syntax_fwd.h"
#include "content_stream_operation_base.h"
#include "content_stream_operators.h"
#include "constants.h"

#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		namespace contents
		{
			class OperationGeneric : public OperationBase
			{
			public:
				OperationGeneric() = default;
				OperationGeneric(std::vector<ObjectPtr> operands, OperatorBasePtr oper) :
					_operator(oper), _operands(operands) {}
				OperatorBasePtr GetOperator() const { return _operator; }
				std::vector<ObjectPtr> GetOperands() const { return _operands; }

				types::uinteger GetOperandsSize() const { return _operands.size(); }
				ObjectPtr GetOperandAt(types::uinteger at) const { return _operands.at(at); }

				virtual InstructionBase::Type GetInstructionType(void) const noexcept override { return InstructionBase::Type::Operation; }
				virtual Type GetOperationType(void) const noexcept override { return Type::Generic; }

			private:
				OperatorBasePtr _operator;
				std::vector<ObjectPtr> _operands;
			};

			typedef std::vector<OperationGenericPtr> GenericOperationCollection;
		}
	}
}

#endif /* _CONTENT_STREAM_OPERATION_GENERIC_H */