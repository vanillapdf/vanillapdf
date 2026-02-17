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

PdfTextString::PdfTextString(BufferPtr raw_data, TextStringEncoding encoding)
    : m_raw_data(std::move(raw_data))
    , m_encoding(encoding) {
}

PdfTextStringPtr PdfTextString::CreateFromStringObject(syntax::StringObjectPtr obj) {
    auto value = obj->GetValue();
    auto encoding = DetectTextStringEncoding(*value);
    return make_deferred<PdfTextString>(std::move(value), encoding);
}

PdfTextStringPtr PdfTextString::CreateFromRaw(BufferPtr bytes) {
    return make_deferred<PdfTextString>(std::move(bytes));
}

PdfTextStringPtr PdfTextString::CreateFromUtf8(BufferPtr utf8_bytes) {
    auto raw = Utf8ToTextString(*utf8_bytes);
    auto encoding = DetectTextStringEncoding(*raw);
    return make_deferred<PdfTextString>(std::move(raw), encoding);
}

PdfTextStringPtr PdfTextString::CreateFromUtf16(BufferPtr utf16be_bytes) {
    // Prepend UTF-16BE BOM (0xFE 0xFF) so raw bytes are a valid PDF text string
    std::string result;
    result.reserve(2 + utf16be_bytes->size());
    result.push_back(static_cast<char>(0xFE));
    result.push_back(static_cast<char>(0xFF));
    result.append(utf16be_bytes->data(), utf16be_bytes->size());
    auto with_bom = make_deferred_container<Buffer>(result.begin(), result.end());
    return make_deferred<PdfTextString>(std::move(with_bom), TextStringEncoding::UTF16BE);
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
