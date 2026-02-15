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
    stream->Write(bt->Value()->ToStringView());
    stream->Write(WhiteSpace::LINE_FEED);

    for (auto op : _operations) {
        stream->Write(op->ToPdf());
        stream->Write(WhiteSpace::LINE_FEED);
    }

    EndTextOperatorPtr et;
    stream->Write(et->Value()->ToStringView());

    return stream->ToString();
}

std::string InlineImageObject::ToPdf() const {
    auto stream = StreamUtils::InputOutputStreamFromMemory();

    BeginInlineImageObjectOperatorPtr bi;
    stream->Write(bi->Value()->ToStringView());
    stream->Write(WhiteSpace::LINE_FEED);

    // Image dictionary
    for (auto item : m_dictionary) {
        stream->Write(item.first->ToPdf());
        stream->Write(WhiteSpace::SPACE);
        stream->Write(item.second->ToPdf());
        stream->Write(WhiteSpace::LINE_FEED);
    }

    BeginInlineImageDataOperatorPtr id;
    stream->Write(id->Value()->ToStringView());
    stream->Write(WhiteSpace::LINE_FEED);
    stream->Write(m_data->ToStringView());
    stream->Write(WhiteSpace::LINE_FEED);

    EndInlineImageObjectOperatorPtr ei;
    stream->Write(ei->Value()->ToStringView());

    return stream->ToString();
}

} // contents
} // vanillapdf
