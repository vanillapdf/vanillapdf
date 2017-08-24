#include "precompiled.h"

#include "contents/content_stream_operations.h"
#include "contents/content_stream_operators.h"

#include "syntax/utils/object_utils.h"

#include "utils/exceptions.h"

#include <sstream>

namespace gotchangpdf {
namespace contents {

using namespace syntax;

OperationBeginText::OperationBeginText(const CustomSizeVector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0)
		throw GeneralException("Unexpected number of arguments");
}

OperationEndText::OperationEndText(const CustomSizeVector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0)
		throw GeneralException("Unexpected number of arguments");
}

OperationTextFont::OperationTextFont(const CustomSizeVector<ObjectPtr>& operands) {
	assert(operands.size() == 2);
	if (operands.size() != 2)
		throw GeneralException("Unexpected number of arguments");

	auto name = operands.at(0);
	auto scale = operands.at(1);
	if (!ObjectUtils::IsType<NameObjectPtr>(name)) {
		assert(!"Text font operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	if (!ObjectUtils::IsType<IntegerObjectPtr>(scale)) {
		assert(!"Text font operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	m_font = syntax::ObjectUtils::ConvertTo<NameObjectPtr>(name);
	m_scale = syntax::ObjectUtils::ConvertTo<IntegerObjectPtr>(scale);
	m_font->Subscribe(this);
	m_scale->Subscribe(this);
}

OperationBeginInlineImageObject::OperationBeginInlineImageObject(const CustomSizeVector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0)
		throw GeneralException("Unexpected number of arguments");
}

OperationBeginInlineImageData::OperationBeginInlineImageData(const CustomSizeVector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0)
		throw GeneralException("Unexpected number of arguments");
}

OperationEndInlineImageObject::OperationEndInlineImageObject(const CustomSizeVector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0)
		throw GeneralException("Unexpected number of arguments");
}

OperationTextShow::OperationTextShow(const CustomSizeVector<ObjectPtr>& operands) {
	if (1 != operands.size()) {
		assert(!"Text show operation has invalid arguments");
		throw GeneralException("Unexpected number of arguments");
	}

	auto item = operands.at(0);
	if (!ObjectUtils::IsType<StringObjectPtr>(item)) {
		assert(!"Text show operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	_str = ObjectUtils::ConvertTo<StringObjectPtr>(item);
	_str->Subscribe(this);
}

OperationTextShowArray::OperationTextShowArray(const CustomSizeVector<ObjectPtr>& operands) {
	if (1 != operands.size()) {
		assert(!"Text show array operation has invalid arguments");
		throw GeneralException("Unexpected number of arguments");
	}

	auto operand = operands.at(0);
	m_items = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(operand);
	m_items->Subscribe(this);
}

OperationTextShowArray::~OperationTextShowArray() {
	m_items->Unsubscribe(this);
}

OperationTextShow::~OperationTextShow() {
	_str->Unsubscribe(this);
}

OperationTextFont::~OperationTextFont() {
	m_font->Unsubscribe(this);
	m_scale->Unsubscribe(this);
}

std::string OperationBeginText::ToPdf() const {
	BeginTextOperatorPtr op;
	BufferPtr buffer = op->Value();
	return buffer->ToString();
}

std::string OperationEndText::ToPdf() const {
	EndTextOperatorPtr op;
	BufferPtr buffer = op->Value();
	return buffer->ToString();
}

std::string OperationTextShow::ToPdf() const {
	std::stringstream ss;

	TextShowOperatorPtr op;
	ss << _str->ToPdf();
	ss << " ";
	ss << op->Value();

	return ss.str();
}

std::string OperationTextShowArray::ToPdf() const {
	std::stringstream ss;

	TextShowArrayOperatorPtr op;
	ss << m_items->ToPdf();
	ss << " ";
	ss << op->Value();

	return ss.str();
}

std::string OperationTextFont::ToPdf() const {
	std::stringstream ss;

	TextFontOperatorPtr op;
	ss << m_font->ToPdf();
	ss << " ";
	ss << op->Value();

	return ss.str();
}

std::string OperationBeginInlineImageObject::ToPdf() const {
	BeginInlineImageObjectOperatorPtr op;
	BufferPtr buffer = op->Value();
	return buffer->ToString();
}

std::string OperationBeginInlineImageData::ToPdf() const {
	BeginInlineImageDataOperatorPtr op;
	BufferPtr buffer = op->Value();
	return buffer->ToString();
}

std::string OperationEndInlineImageObject::ToPdf() const {
	EndInlineImageObjectOperatorPtr op;
	BufferPtr buffer = op->Value();
	return buffer->ToString();
}

} // contents
} // gotchangpdf
