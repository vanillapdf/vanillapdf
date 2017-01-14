#ifndef _CONTENT_STREAM_OPERATION_GENERIC_H
#define _CONTENT_STREAM_OPERATION_GENERIC_H

#include "syntax/utils/syntax_fwd.h"
#include "contents/content_stream_operation_base.h"
#include "contents/content_stream_operators.h"
#include "utils/constants.h"

#include <vector>
#include <sstream>

namespace gotchangpdf
{
	namespace contents
	{
		class OperationGeneric : public OperationBase
		{
		public:
			OperationGeneric() = default;
			OperationGeneric(std::vector<syntax::ObjectPtr> operands, OperatorBasePtr oper) :
				_operator(oper), _operands(operands) {}
			OperatorBasePtr GetOperator() const { return _operator; }
			std::vector<syntax::ObjectPtr> GetOperands() const { return _operands; }

			types::uinteger GetOperandsSize() const { return _operands.size(); }
			syntax::ObjectPtr GetOperandAt(types::uinteger at) const { return _operands.at(at); }

			virtual InstructionBase::Type GetInstructionType(void) const noexcept override { return InstructionBase::Type::Operation; }
			virtual Type GetOperationType(void) const noexcept override { return Type::Generic; }
			virtual std::string ToPdf() const override
			{
				std::stringstream ss;

				bool first = true;
				for (auto operand : _operands) {
					if (!first) {
						ss << " ";
					}

					ss << operand->ToPdf();
					first = false;
				}

				if (!first) {
					ss << " ";
				}

				ss << _operator->Value();

				return ss.str();
			}

		private:
			OperatorBasePtr _operator;
			std::vector<syntax::ObjectPtr> _operands;
		};

		typedef std::vector<OperationGenericPtr> GenericOperationCollection;
	}
}

#endif /* _CONTENT_STREAM_OPERATION_GENERIC_H */