#include "precompiled.h"

#include "contents/content_stream_objects.h"
#include "contents/content_stream_operators.h"

#include "syntax/exceptions/syntax_exceptions.h"
#include "syntax/filters/filter.h"
#include "syntax/utils/name_constants.h"
#include "syntax/utils/object_utils.h"

#include "utils/streams/stream_utils.h"

namespace vanillapdf {
namespace contents {

using namespace syntax;

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

BufferPtr InlineImageObject::GetSamples() const {

    // The inline image dictionary may use either the abbreviated or the full key names
    OutputContainableObjectPtr filter_found;
    bool has_filter =
        m_dictionary->TryFind(constant::Name::F, filter_found) ||
        m_dictionary->TryFind(constant::Name::Filter, filter_found);

    if (!has_filter) {
        return m_data;
    }

    ObjectPtr filter_obj = *filter_found;

    OutputContainableObjectPtr decode_parameters_found;
    bool has_decode_parameters =
        m_dictionary->TryFind(constant::Name::DP, decode_parameters_found) ||
        m_dictionary->TryFind(constant::Name::DecodeParms, decode_parameters_found);

    ObjectPtr decode_parameters_obj = NullObject::GetInstance();
    if (has_decode_parameters) {
        decode_parameters_obj = *decode_parameters_found;
    }

    if (ObjectUtils::IsType<NullObjectPtr>(filter_obj)) {
        return m_data;
    }

    if (ObjectUtils::IsType<NameObjectPtr>(filter_obj)) {
        auto filter_name = ObjectUtils::ConvertTo<NameObjectPtr>(filter_obj);
        const auto& full_filter_name = FilterBase::GetFullFilterName(filter_name);
        auto filter = FilterBase::GetFilterByName(full_filter_name);

        if (!ObjectUtils::IsType<NullObjectPtr>(decode_parameters_obj)) {
            auto decode_parameters = ObjectUtils::ConvertTo<DictionaryObjectPtr>(decode_parameters_obj);
            return filter->Decode(m_data, decode_parameters);
        }

        return filter->Decode(m_data);
    }

    if (ObjectUtils::IsType<ArrayObjectPtr<NameObjectPtr>>(filter_obj)) {
        auto filter_array = ObjectUtils::ConvertTo<ArrayObjectPtr<NameObjectPtr>>(filter_obj);

        MixedArrayObjectPtr decode_parameters_array;
        bool has_decode_parameters_array = false;
        if (!ObjectUtils::IsType<NullObjectPtr>(decode_parameters_obj)) {
            decode_parameters_array = ObjectUtils::ConvertTo<MixedArrayObjectPtr>(decode_parameters_obj);
            has_decode_parameters_array = true;
            assert(filter_array->GetSize() == decode_parameters_array->GetSize());
        }

        BufferPtr result = m_data;
        for (unsigned int i = 0; i < filter_array->GetSize(); ++i) {
            auto current_filter_name = (*filter_array)[i];
            const auto& full_filter_name = FilterBase::GetFullFilterName(current_filter_name);
            auto filter = FilterBase::GetFilterByName(full_filter_name);

            if (has_decode_parameters_array && i < decode_parameters_array->GetSize()) {
                auto current_decode_parameters = (*decode_parameters_array)[i];
                if (!ObjectUtils::IsType<NullObjectPtr>(current_decode_parameters)) {
                    auto decode_parameters = ObjectUtils::ConvertTo<DictionaryObjectPtr>(current_decode_parameters);
                    result = filter->Decode(result, decode_parameters);
                    continue;
                }
            }

            result = filter->Decode(result);
        }

        return result;
    }

    LOG_ERROR_AND_THROW(ParseException, "Inline image filter is neither name nor array of names");
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
