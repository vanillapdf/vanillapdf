#include "precompiled.h"

#include "semantics/utils/pdf_text_string.h"
#include "utils/buffer.h"
#include "utils/text_string_encoding.h"

#include "syntax/objects/string_object.h"

namespace vanillapdf {
namespace semantics {

PdfTextString::PdfTextString(syntax::StringObjectPtr obj)
    : m_obj(obj) {
}

PdfTextStringPtr PdfTextString::CreateFromStringObject(syntax::StringObjectPtr obj) {
    return make_deferred<PdfTextString>(obj);
}

PdfTextStringPtr PdfTextString::CreateFromRaw(std::string_view data) {
    auto str_obj = syntax::LiteralStringObject::CreateFromDecoded(data);
    syntax::StringObjectPtr base = str_obj;
    return make_deferred<PdfTextString>(base);
}

PdfTextStringPtr PdfTextString::CreateFromUtf8(std::string_view data) {
    auto input = make_deferred_container<Buffer>(data.begin(), data.end());
    auto raw = Utf8ToTextString(*input);
    auto str_obj = syntax::LiteralStringObject::CreateFromDecoded(raw);
    syntax::StringObjectPtr base = str_obj;
    return make_deferred<PdfTextString>(base);
}

PdfTextStringPtr PdfTextString::CreateFromUtf16(std::string_view data) {
    // Prepend UTF-16BE BOM (0xFE 0xFF)
    std::string with_bom;
    with_bom.reserve(2 + data.size());
    with_bom.push_back(static_cast<char>(0xFE));
    with_bom.push_back(static_cast<char>(0xFF));
    with_bom.append(data.data(), data.size());

    auto str_obj = syntax::LiteralStringObject::CreateFromDecoded(std::string_view(with_bom));
    syntax::StringObjectPtr base = str_obj;
    return make_deferred<PdfTextString>(base);
}

TextStringEncoding PdfTextString::GetEncoding() const {
    auto value = m_obj->GetValue();
    return DetectTextStringEncoding(*value);
}

BufferPtr PdfTextString::GetStringRaw() const {
    return m_obj->GetValue();
}

BufferPtr PdfTextString::GetStringUtf8() const {
    auto value = m_obj->GetValue();
    return TextStringToUtf8(*value);
}

BufferPtr PdfTextString::GetStringUtf16() const {
    auto value = m_obj->GetValue();
    return TextStringToUtf16BE(*value);
}

void PdfTextString::SetStringRaw(std::string_view data) {
    m_obj->SetValue(data);
}

void PdfTextString::SetStringUtf8(std::string_view data) {
    auto input = make_deferred_container<Buffer>(data.begin(), data.end());
    auto raw = Utf8ToTextString(*input);
    m_obj->SetValue(raw);
}

void PdfTextString::SetStringUtf16(std::string_view data) {
    // Prepend UTF-16BE BOM (0xFE 0xFF)
    std::string with_bom;
    with_bom.reserve(2 + data.size());
    with_bom.push_back(static_cast<char>(0xFE));
    with_bom.push_back(static_cast<char>(0xFF));
    with_bom.append(data.data(), data.size());

    m_obj->SetValue(std::string_view(with_bom));
}

syntax::StringObjectPtr PdfTextString::GetStringObject() const {
    return m_obj;
}

void PdfTextString::SetStringObject(syntax::StringObjectPtr obj) {
    m_obj = obj;
}

} // semantics
} // vanillapdf
