#include "precompiled.h"

#include "contents/content_stream_operation_generic.h"

#include <sstream>

namespace vanillapdf {
namespace contents {

OperationGeneric::OperationGeneric(std::vector<syntax::ObjectPtr> operands, OperatorBasePtr oper)
	: _operator(oper), _operands(operands) {
}

OperatorBasePtr OperationGeneric::GetOperator() const {
	return _operator;
}

std::vector<syntax::ObjectPtr> OperationGeneric::GetOperands() const {
	return _operands;
}

types::size_type OperationGeneric::GetOperandsSize() const {
	return _operands.size();
}

syntax::ObjectPtr OperationGeneric::GetOperandAt(types::size_type at) const {
	return _operands.at(at);
}

std::string OperationGeneric::ToPdf() const {
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

} // contents
} // vanillapdf
