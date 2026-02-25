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
        throw syntax::ParseException("Unexpected number of arguments");
    }
}

OperationEndText::OperationEndText(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        throw syntax::ParseException("Unexpected number of arguments");
    }
}

OperationSaveGraphicsState::OperationSaveGraphicsState(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        throw syntax::ParseException("Unexpected number of arguments");
    }
}

OperationRestoreGraphicsState::OperationRestoreGraphicsState(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        throw syntax::ParseException("Unexpected number of arguments");
    }
}

OperationSetStrokingColorSpaceRGB::OperationSetStrokingColorSpaceRGB(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 3);
    if (operands.size() != 3) {
        throw syntax::ParseException("Unexpected number of arguments");
    }

    auto red = operands.at(0);
    auto green = operands.at(1);
    auto blue = operands.at(2);

    if (!ObjectUtils::IsType<RealObjectPtr>(red)) {
        throw syntax::ParseException("Unexpected argument type");
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(green)) {
        throw syntax::ParseException("Unexpected argument type");
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(blue)) {
        throw syntax::ParseException("Unexpected argument type");
    }

    m_red = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(red);
    m_green = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(green);
    m_blue = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(blue);
}

OperationSetNonstrokingColorSpaceRGB::OperationSetNonstrokingColorSpaceRGB(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 3);
    if (operands.size() != 3) {
        throw syntax::ParseException("Unexpected number of arguments");
    }

    auto red = operands.at(0);
    auto green = operands.at(1);
    auto blue = operands.at(2);

    if (!ObjectUtils::IsType<RealObjectPtr>(red)) {
        throw syntax::ParseException("Unexpected argument type");
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(green)) {
        throw syntax::ParseException("Unexpected argument type");
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(blue)) {
        throw syntax::ParseException("Unexpected argument type");
    }

    m_red = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(red);
    m_green = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(green);
    m_blue = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(blue);
}

OperationTextFont::OperationTextFont(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 2);
    if (operands.size() != 2) {
        throw syntax::ParseException("Unexpected number of arguments");
    }

    auto name = operands.at(0);
    auto scale = operands.at(1);
    if (!ObjectUtils::IsType<NameObjectPtr>(name)) {
        assert(!"Text font operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    if (!ObjectUtils::IsType<RealObjectPtr>(scale)) {
        assert(!"Text font operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    m_font = syntax::ObjectUtils::ConvertTo<NameObjectPtr>(name);
    m_scale = syntax::ObjectUtils::ConvertTo<RealObjectPtr>(scale);
}

OperationTextTranslate::OperationTextTranslate(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 2);
    if (operands.size() != 2) {
        throw syntax::ParseException("Unexpected number of arguments");
    }

    auto x = operands.at(0);
    auto y = operands.at(1);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(x)) {
        assert(!"Text font operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    if (!ObjectUtils::IsType<IntegerObjectPtr>(y)) {
        assert(!"Text font operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    m_x = syntax::ObjectUtils::ConvertTo<IntegerObjectPtr>(x);
    m_y = syntax::ObjectUtils::ConvertTo<IntegerObjectPtr>(y);
}

OperationBeginInlineImageObject::OperationBeginInlineImageObject(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        throw syntax::ParseException("Unexpected number of arguments");
    }
}

OperationBeginInlineImageData::OperationBeginInlineImageData(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        throw syntax::ParseException("Unexpected number of arguments");
    }
}

OperationEndInlineImageObject::OperationEndInlineImageObject(const std::vector<ObjectPtr>& operands) {
    assert(operands.size() == 0);
    if (operands.size() != 0) {
        throw syntax::ParseException("Unexpected number of arguments");
    }
}

OperationTextShow::OperationTextShow(const std::vector<ObjectPtr>& operands) {
    if (1 != operands.size()) {
        assert(!"Text show operation has invalid arguments");
        throw syntax::ParseException("Unexpected number of arguments");
    }

    auto item = operands.at(0);
    if (!ObjectUtils::IsType<StringObjectPtr>(item)) {
        assert(!"Text show operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    _str = ObjectUtils::ConvertTo<StringObjectPtr>(item);
}

OperationTextShowArray::OperationTextShowArray(const std::vector<ObjectPtr>& operands) {
    if (1 != operands.size()) {
        assert(!"Text show array operation has invalid arguments");
        throw syntax::ParseException("Unexpected number of arguments");
    }

    auto item = operands.at(0);
    if (!ObjectUtils::IsType<MixedArrayObjectPtr>(item)) {
        assert(!"Text show array operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    m_items = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(item);
}

OperationTransformationMatrix::OperationTransformationMatrix(const std::vector<ObjectPtr>& operands) {
    if (6 != operands.size()) {
        assert(!"Transformation matrix operation has invalid arguments");
        throw syntax::ParseException("Unexpected number of arguments");
    }

    auto a = operands.at(0);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(a)) {
        assert(!"Transformation matrix operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    auto b = operands.at(1);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(b)) {
        assert(!"Transformation matrix operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    auto c = operands.at(2);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(c)) {
        assert(!"Transformation matrix operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    auto d = operands.at(3);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(d)) {
        assert(!"Transformation matrix operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    auto e = operands.at(4);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(e)) {
        assert(!"Transformation matrix operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
    }

    auto f = operands.at(5);
    if (!ObjectUtils::IsType<IntegerObjectPtr>(f)) {
        assert(!"Transformation matrix operation has invalid arguments");
        throw syntax::ParseException("Unexpected argument type");
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
