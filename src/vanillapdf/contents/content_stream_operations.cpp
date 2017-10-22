#include "precompiled.h"

#include "contents/content_stream_operations.h"
#include "contents/content_stream_operators.h"

#include "syntax/utils/object_utils.h"

#include "utils/exceptions.h"

#include <sstream>

namespace vanillapdf {
namespace contents {

using namespace syntax;

OperationBeginText::OperationBeginText(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0) {
		throw GeneralException("Unexpected number of arguments");
	}
}

OperationEndText::OperationEndText(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0) {
		throw GeneralException("Unexpected number of arguments");
	}
}

OperationSaveGraphicsState::OperationSaveGraphicsState(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0) {
		throw GeneralException("Unexpected number of arguments");
	}
}

OperationRestoreGraphicsState::OperationRestoreGraphicsState(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0) {
		throw GeneralException("Unexpected number of arguments");
	}
}

OperationSetStrokingColorSpaceRGB::OperationSetStrokingColorSpaceRGB(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 3);
	if (operands.size() != 3) {
		throw GeneralException("Unexpected number of arguments");
	}

	auto red = operands.at(0);
	auto green = operands.at(1);
	auto blue = operands.at(2);

	if (!ObjectUtils::IsType<RealObjectPtr>(red)) {
		throw GeneralException("Unexpected argument type");
	}

	if (!ObjectUtils::IsType<RealObjectPtr>(green)) {
		throw GeneralException("Unexpected argument type");
	}

	if (!ObjectUtils::IsType<RealObjectPtr>(blue)) {
		throw GeneralException("Unexpected argument type");
	}

	m_red = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(red);
	m_green = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(green);
	m_blue = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(blue);

	m_red->Subscribe(this);
	m_green->Subscribe(this);
	m_blue->Subscribe(this);
}

OperationSetNonstrokingColorSpaceRGB::OperationSetNonstrokingColorSpaceRGB(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 3);
	if (operands.size() != 3) {
		throw GeneralException("Unexpected number of arguments");
	}

	auto red = operands.at(0);
	auto green = operands.at(1);
	auto blue = operands.at(2);

	if (!ObjectUtils::IsType<RealObjectPtr>(red)) {
		throw GeneralException("Unexpected argument type");
	}

	if (!ObjectUtils::IsType<RealObjectPtr>(green)) {
		throw GeneralException("Unexpected argument type");
	}

	if (!ObjectUtils::IsType<RealObjectPtr>(blue)) {
		throw GeneralException("Unexpected argument type");
	}

	m_red = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(red);
	m_green = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(green);
	m_blue = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(blue);

	m_red->Subscribe(this);
	m_green->Subscribe(this);
	m_blue->Subscribe(this);
}

OperationTextFont::OperationTextFont(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 2);
	if (operands.size() != 2) {
		throw GeneralException("Unexpected number of arguments");
	}

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

OperationTextTranslate::OperationTextTranslate(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 2);
	if (operands.size() != 2) {
		throw GeneralException("Unexpected number of arguments");
	}

	auto x = operands.at(0);
	auto y = operands.at(1);
	if (!ObjectUtils::IsType<IntegerObjectPtr>(x)) {
		assert(!"Text font operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	if (!ObjectUtils::IsType<IntegerObjectPtr>(y)) {
		assert(!"Text font operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	m_x = syntax::ObjectUtils::ConvertTo<IntegerObjectPtr>(x);
	m_y = syntax::ObjectUtils::ConvertTo<IntegerObjectPtr>(y);
	m_x->Subscribe(this);
	m_y->Subscribe(this);
}

OperationBeginInlineImageObject::OperationBeginInlineImageObject(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0) {
		throw GeneralException("Unexpected number of arguments");
	}
}

OperationBeginInlineImageData::OperationBeginInlineImageData(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0) {
		throw GeneralException("Unexpected number of arguments");
	}
}

OperationEndInlineImageObject::OperationEndInlineImageObject(const std::vector<ObjectPtr>& operands) {
	assert(operands.size() == 0);
	if (operands.size() != 0) {
		throw GeneralException("Unexpected number of arguments");
	}
}

OperationTextShow::OperationTextShow(const std::vector<ObjectPtr>& operands) {
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

OperationTextShowArray::OperationTextShowArray(const std::vector<ObjectPtr>& operands) {
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

OperationTextTranslate::~OperationTextTranslate() {
	m_x->Unsubscribe(this);
	m_y->Unsubscribe(this);
}

OperationSetStrokingColorSpaceRGB::~OperationSetStrokingColorSpaceRGB() {
	m_red->Unsubscribe(this);
	m_green->Unsubscribe(this);
	m_blue->Unsubscribe(this);
}

OperationSetNonstrokingColorSpaceRGB::~OperationSetNonstrokingColorSpaceRGB() {
	m_red->Unsubscribe(this);
	m_green->Unsubscribe(this);
	m_blue->Unsubscribe(this);
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

std::string OperationSaveGraphicsState::ToPdf() const {
	SaveGraphicsStateOperatorPtr op;
	BufferPtr buffer = op->Value();
	return buffer->ToString();
}

std::string OperationRestoreGraphicsState::ToPdf() const {
	RestoreGraphicsStateOperatorPtr op;
	BufferPtr buffer = op->Value();
	return buffer->ToString();
}

std::string OperationSetStrokingColorSpaceRGB::ToPdf() const {
	std::stringstream ss;

	SetStrokingColorSpaceRGBOperatorPtr op;
	ss << m_red->ToPdf();
	ss << " ";
	ss << m_green->ToPdf();
	ss << " ";
	ss << m_blue->ToPdf();
	ss << " ";
	ss << op->Value();

	return ss.str();
}

std::string OperationSetNonstrokingColorSpaceRGB::ToPdf() const {
	std::stringstream ss;

	SetNonstrokingColorSpaceRGBOperatorPtr op;
	ss << m_red->ToPdf();
	ss << " ";
	ss << m_green->ToPdf();
	ss << " ";
	ss << m_blue->ToPdf();
	ss << " ";
	ss << op->Value();

	return ss.str();
}

std::string OperationTextTranslate::ToPdf() const {
	std::stringstream ss;

	TextTranslateOperatorPtr op;
	ss << m_x->ToPdf();
	ss << " ";
	ss << m_y->ToPdf();
	ss << " ";
	ss << op->Value();

	return ss.str();
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
	ss << m_scale->ToPdf();
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
} // vanillapdf
