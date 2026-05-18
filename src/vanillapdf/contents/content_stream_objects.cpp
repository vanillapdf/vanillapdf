#include "precompiled.h"

#include "contents/content_stream_objects.h"
#include "contents/content_stream_operators.h"

#include "utils/streams/stream_utils.h"

namespace vanillapdf {
namespace contents {

TextObject::TextObject(const contents::BaseOperationCollection& ops)
    : _operations(ops) {
}

std::string TextObject::ToPdf() const {
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    BeginTextOperatorPtr bt;
    stream->WriteLine(bt->Value()->ToStringView());

    for (auto op : _operations) {
        stream->WriteLine(op->ToPdf());
    }

    EndTextOperatorPtr et;
    stream->Write(et->Value()->ToStringView());

    return stream->ToString();
}

std::string InlineImageObject::ToPdf() const {
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    BeginInlineImageObjectOperatorPtr bi;
    stream->WriteLine(bi->Value()->ToStringView());

    // Image dictionary
    for (auto item : m_dictionary) {
        stream->Write(item.first->ToPdf());
        stream->Write(WhiteSpace::SPACE);
        stream->WriteLine(item.second->ToPdf());
    }

    BeginInlineImageDataOperatorPtr id;
    stream->WriteLine(id->Value()->ToStringView());
    stream->WriteLine(m_data->ToStringView());

    EndInlineImageObjectOperatorPtr ei;
    stream->Write(ei->Value()->ToStringView());

    return stream->ToString();
}

} // contents
} // vanillapdf
