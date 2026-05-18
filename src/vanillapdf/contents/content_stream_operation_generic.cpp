#include "precompiled.h"

#include "contents/content_stream_operation_generic.h"

#include "utils/streams/stream_utils.h"

namespace vanillapdf {
namespace contents {

OperationGeneric::OperationGeneric(const std::vector<syntax::ObjectPtr>& operands, OperatorBasePtr oper)
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
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    bool first = true;
    for (auto operand : _operands) {
        if (!first) {
            stream->Write(WhiteSpace::SPACE);
        }

        stream->Write(operand->ToPdf());
        first = false;
    }

    if (!first) {
        stream->Write(WhiteSpace::SPACE);
    }

    stream->Write(_operator->Value()->ToStringView());

    return stream->ToString();
}

} // contents
} // vanillapdf
