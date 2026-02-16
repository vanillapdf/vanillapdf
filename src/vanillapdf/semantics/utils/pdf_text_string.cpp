#include "precompiled.h"

#include "semantics/utils/pdf_text_string.h"
#include "utils/buffer.h"
#include "utils/text_string_encoding.h"

#include "syntax/objects/string_object.h"

namespace vanillapdf {
namespace semantics {

PdfTextString::PdfTextString(BufferPtr raw_data)
    : m_raw_data(std::move(raw_data))
    , m_encoding(DetectTextStringEncoding(*m_raw_data)) {
}

PdfTextStringPtr PdfTextString::CreateFromStringObject(syntax::StringObjectPtr obj) {
    auto value = obj->GetValue();
    return make_deferred<PdfTextString>(std::move(value));
}

PdfTextStringPtr PdfTextString::CreateFromRawBytes(BufferPtr raw_data) {
    return make_deferred<PdfTextString>(std::move(raw_data));
}

PdfTextStringPtr PdfTextString::CreateFromUtf8(const std::string& utf8_text) {
    auto utf8_buffer = make_deferred_container<Buffer>(utf8_text.begin(), utf8_text.end());
    auto raw = Utf8ToTextString(*utf8_buffer);
    return make_deferred<PdfTextString>(std::move(raw));
}

TextStringEncoding PdfTextString::GetEncoding() const {
    return m_encoding;
}

BufferPtr PdfTextString::GetStringRaw() const {
    return m_raw_data;
}

BufferPtr PdfTextString::GetStringUtf8() const {
    return TextStringToUtf8(*m_raw_data);
}

BufferPtr PdfTextString::GetStringUtf16() const {
    return TextStringToUtf16BE(*m_raw_data);
}

} // semantics
} // vanillapdf
