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
	m_red->SetInitialized();

	m_green->Subscribe(this);
	m_green->SetInitialized();

	m_blue->Subscribe(this);
	m_blue->SetInitialized();
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
	m_red->SetInitialized();

	m_green->Subscribe(this);
	m_green->SetInitialized();

	m_blue->Subscribe(this);
	m_blue->SetInitialized();
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

	if (!ObjectUtils::IsType<RealObjectPtr>(scale)) {
		assert(!"Text font operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	m_font = syntax::ObjectUtils::ConvertTo<NameObjectPtr>(name);
	m_scale = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(scale);

	m_font->Subscribe(this);
	m_font->SetInitialized();

	m_scale->Subscribe(this);
	m_scale->SetInitialized();
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
	m_x->SetInitialized();

	m_y->Subscribe(this);
	m_y->SetInitialized();
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
	_str->SetInitialized();
}

OperationTextShowArray::OperationTextShowArray(const std::vector<ObjectPtr>& operands) {
	if (1 != operands.size()) {
		assert(!"Text show array operation has invalid arguments");
		throw GeneralException("Unexpected number of arguments");
	}

	auto item = operands.at(0);
	if (!ObjectUtils::IsType<MixedArrayObjectPtr>(item)) {
		assert(!"Text show array operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	m_items = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(item);
	m_items->Subscribe(this);
	m_items->SetInitialized();
}

OperationTransformationMatrix::OperationTransformationMatrix(const std::vector<ObjectPtr>& operands) {
	if (6 != operands.size()) {
		assert(!"Transformation matrix operation has invalid arguments");
		throw GeneralException("Unexpected number of arguments");
	}

	auto a = operands.at(0);
	if (!ObjectUtils::IsType<IntegerObjectPtr>(a)) {
		assert(!"Transformation matrix operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	auto b = operands.at(1);
	if (!ObjectUtils::IsType<IntegerObjectPtr>(b)) {
		assert(!"Transformation matrix operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	auto c = operands.at(2);
	if (!ObjectUtils::IsType<IntegerObjectPtr>(c)) {
		assert(!"Transformation matrix operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	auto d = operands.at(3);
	if (!ObjectUtils::IsType<IntegerObjectPtr>(d)) {
		assert(!"Transformation matrix operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	auto e = operands.at(4);
	if (!ObjectUtils::IsType<IntegerObjectPtr>(e)) {
		assert(!"Transformation matrix operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	auto f = operands.at(5);
	if (!ObjectUtils::IsType<IntegerObjectPtr>(f)) {
		assert(!"Transformation matrix operation has invalid arguments");
		throw GeneralException("Unexpected argument type");
	}

	m_a = ObjectUtils::ConvertTo<IntegerObjectPtr>(a);
	m_b = ObjectUtils::ConvertTo<IntegerObjectPtr>(b);
	m_c = ObjectUtils::ConvertTo<IntegerObjectPtr>(c);
	m_d = ObjectUtils::ConvertTo<IntegerObjectPtr>(d);
	m_e = ObjectUtils::ConvertTo<IntegerObjectPtr>(e);
	m_f = ObjectUtils::ConvertTo<IntegerObjectPtr>(f);
	
	m_a->Subscribe(this);
	m_a->SetInitialized();

	m_b->Subscribe(this);
	m_b->SetInitialized();

	m_c->Subscribe(this);
	m_c->SetInitialized();

	m_d->Subscribe(this);
	m_d->SetInitialized();

	m_e->Subscribe(this);
	m_e->SetInitialized();

	m_f->Subscribe(this);
	m_f->SetInitialized();
}

OperationTransformationMatrix::~OperationTransformationMatrix() {
	m_a->Unsubscribe(this);
	m_b->Unsubscribe(this);
	m_c->Unsubscribe(this);
	m_d->Unsubscribe(this);
	m_e->Unsubscribe(this);
	m_f->Unsubscribe(this);
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

std::string OperationTransformationMatrix::ToPdf() const {

	std::stringstream ss;

	TransformationMatrixOperatorPtr op;
	ss << m_a->ToPdf();
	ss << " ";
	ss << m_b->ToPdf();
	ss << " ";
	ss << m_c->ToPdf();
	ss << " ";
	ss << m_d->ToPdf();
	ss << " ";
	ss << m_e->ToPdf();
	ss << " ";
	ss << m_f->ToPdf();
	ss << " ";
	ss << op->Value();

	return ss.str();
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
