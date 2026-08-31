#include "precompiled.h"

#include "contents/content_stream_operations.h"
#include "contents/content_stream_operators.h"

#include "syntax/utils/object_utils.h"

#include "utils/exceptions.h"
#include "syntax/exceptions/syntax_exceptions.h"

#include "utils/streams/stream_utils.h"

namespace vanillapdf {
namespace contents {

using namespace syntax;

OperationBeginText::OperationBeginText(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator BT expects no operands, but {} were found", operands.size());
    }
}

OperationEndText::OperationEndText(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator ET expects no operands, but {} were found", operands.size());
    }
}

OperationSaveGraphicsState::OperationSaveGraphicsState(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator q expects no operands, but {} were found", operands.size());
    }
}

OperationRestoreGraphicsState::OperationRestoreGraphicsState(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Q expects no operands, but {} were found", operands.size());
    }
}

OperationSetStrokingColorSpaceRGB::OperationSetStrokingColorSpaceRGB(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 3);
    if (operands.size() != 3) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator RG expects 3 operands, but {} were found", operands.size());
    }

    auto red = operands.at(0);
    auto green = operands.at(1);
    auto blue = operands.at(2);

    if (!ObjectUtils::IsType<RealObjectPtr>(red)) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator RG expects a Real red component, but found {}", Object::TypeName(red->GetObjectType()));
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(green)) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator RG expects a Real green component, but found {}", Object::TypeName(green->GetObjectType()));
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(blue)) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator RG expects a Real blue component, but found {}", Object::TypeName(blue->GetObjectType()));
    }

    m_red = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(red);
    m_green = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(green);
    m_blue = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(blue);
}

OperationSetNonstrokingColorSpaceRGB::OperationSetNonstrokingColorSpaceRGB(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 3);
    if (operands.size() != 3) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator rg expects 3 operands, but {} were found", operands.size());
    }

    auto red = operands.at(0);
    auto green = operands.at(1);
    auto blue = operands.at(2);

    if (!ObjectUtils::IsType<RealObjectPtr>(red)) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator rg expects a Real red component, but found {}", Object::TypeName(red->GetObjectType()));
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(green)) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator rg expects a Real green component, but found {}", Object::TypeName(green->GetObjectType()));
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(blue)) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator rg expects a Real blue component, but found {}", Object::TypeName(blue->GetObjectType()));
    }

    m_red = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(red);
    m_green = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(green);
    m_blue = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(blue);
}

OperationTextFont::OperationTextFont(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 2);
    if (operands.size() != 2) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Tf expects 2 operands, but {} were found", operands.size());
    }

    auto name = operands.at(0);
    auto scale = operands.at(1);
    if (!ObjectUtils::IsType<NameObjectPtr>(name)) {
        assert(!"Text font operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Tf expects a Name font, but found {}", Object::TypeName(name->GetObjectType()));
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(scale)) {
        assert(!"Text font operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Tf expects a Real font size, but found {}", Object::TypeName(scale->GetObjectType()));
    }

    m_font = syntax::ObjectUtils::ConvertTo<NameObjectPtr>(name);
    m_scale = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(scale);
}

OperationTextTranslate::OperationTextTranslate(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 2);
    if (operands.size() != 2) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Td expects 2 operands, but {} were found", operands.size());
    }

    auto x = operands.at(0);
    auto y = operands.at(1);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(x)) {
        assert(!"Text font operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Td expects an Integer x offset, but found {}", Object::TypeName(x->GetObjectType()));
    }

    if (!ObjectUtils::IsType<IntegerObjectPtr>(y)) {
        assert(!"Text font operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Td expects an Integer y offset, but found {}", Object::TypeName(y->GetObjectType()));
    }

    m_x = syntax::ObjectUtils::ConvertTo<IntegerObjectPtr>(x);
    m_y = syntax::ObjectUtils::ConvertTo<IntegerObjectPtr>(y);
}

OperationBeginInlineImageObject::OperationBeginInlineImageObject(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator BI expects no operands, but {} were found", operands.size());
    }
}

OperationBeginInlineImageData::OperationBeginInlineImageData(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator ID expects no operands, but {} were found", operands.size());
    }
}

OperationEndInlineImageObject::OperationEndInlineImageObject(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator EI expects no operands, but {} were found", operands.size());
    }
}

OperationTextShow::OperationTextShow(const std::vector<ObjectPtr>& operands) {
    if (1 != operands.size()) {
        assert(!"Text show operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Tj expects 1 operand, but {} were found", operands.size());
    }

    auto item = operands.at(0);
    if (!ObjectUtils::IsType<StringObjectPtr>(item)) {
        assert(!"Text show operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator Tj expects a String operand, but found {}", Object::TypeName(item->GetObjectType()));
    }

    _str = ObjectUtils::ConvertTo<StringObjectPtr>(item);
}

OperationTextShowArray::OperationTextShowArray(const std::vector<ObjectPtr>& operands) {
    if (1 != operands.size()) {
        assert(!"Text show array operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator TJ expects 1 operand, but {} were found", operands.size());
    }

    auto item = operands.at(0);
    if (!ObjectUtils::IsType<MixedArrayObjectPtr>(item)) {
        assert(!"Text show array operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator TJ expects an Array operand, but found {}", Object::TypeName(item->GetObjectType()));
    }

    m_items = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(item);
}

OperationTransformationMatrix::OperationTransformationMatrix(const std::vector<ObjectPtr>& operands) {
    if (6 != operands.size()) {
        assert(!"Transformation matrix operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator cm expects 6 operands, but {} were found", operands.size());
    }

    auto a = operands.at(0);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(a)) {
        assert(!"Transformation matrix operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator cm expects an Integer operand a, but found {}", Object::TypeName(a->GetObjectType()));
    }

    auto b = operands.at(1);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(b)) {
        assert(!"Transformation matrix operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator cm expects an Integer operand b, but found {}", Object::TypeName(b->GetObjectType()));
    }

    auto c = operands.at(2);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(c)) {
        assert(!"Transformation matrix operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator cm expects an Integer operand c, but found {}", Object::TypeName(c->GetObjectType()));
    }

    auto d = operands.at(3);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(d)) {
        assert(!"Transformation matrix operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator cm expects an Integer operand d, but found {}", Object::TypeName(d->GetObjectType()));
    }

    auto e = operands.at(4);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(e)) {
        assert(!"Transformation matrix operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator cm expects an Integer operand e, but found {}", Object::TypeName(e->GetObjectType()));
    }

    auto f = operands.at(5);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(f)) {
        assert(!"Transformation matrix operation has invalid arguments");
        LOG_ERROR_AND_THROW(syntax::ParseException, "Operator cm expects an Integer operand f, but found {}", Object::TypeName(f->GetObjectType()));
    }

    m_a = ObjectUtils::ConvertTo<IntegerObjectPtr>(a);
    m_b = ObjectUtils::ConvertTo<IntegerObjectPtr>(b);
    m_c = ObjectUtils::ConvertTo<IntegerObjectPtr>(c);
    m_d = ObjectUtils::ConvertTo<IntegerObjectPtr>(d);
    m_e = ObjectUtils::ConvertTo<IntegerObjectPtr>(e);
    m_f = ObjectUtils::ConvertTo<IntegerObjectPtr>(f);
}

std::string OperationTransformationMatrix::ToPdf() const {

    auto stream = StreamUtils::InputOutputStreamFromMemory();

    TransformationMatrixOperatorPtr op;
    stream->Write(m_a->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_b->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_c->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_d->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_e->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_f->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(op->Value()->ToStringView());

    return stream->ToString();
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
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    SetStrokingColorSpaceRGBOperatorPtr op;
    stream->Write(m_red->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_green->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_blue->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(op->Value()->ToStringView());

    return stream->ToString();
}

std::string OperationSetNonstrokingColorSpaceRGB::ToPdf() const {
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    SetNonstrokingColorSpaceRGBOperatorPtr op;
    stream->Write(m_red->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_green->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_blue->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(op->Value()->ToStringView());

    return stream->ToString();
}

std::string OperationTextTranslate::ToPdf() const {
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    TextTranslateOperatorPtr op;
    stream->Write(m_x->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_y->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(op->Value()->ToStringView());

    return stream->ToString();
}

std::string OperationTextShow::ToPdf() const {
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    TextShowOperatorPtr op;
    stream->Write(_str->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(op->Value()->ToStringView());

    return stream->ToString();
}

std::string OperationTextShowArray::ToPdf() const {
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    TextShowArrayOperatorPtr op;
    stream->Write(m_items->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(op->Value()->ToStringView());

    return stream->ToString();
}

std::string OperationTextFont::ToPdf() const {
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    TextFontOperatorPtr op;
    stream->Write(m_font->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(m_scale->ToPdf());
    stream->Write(WhiteSpace::SPACE);
    stream->Write(op->Value()->ToStringView());

    return stream->ToString();
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
