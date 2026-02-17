#include "precompiled.h"

#include "semantics/utils/document_signature_settings.h"
#include "semantics/utils/pdf_text_string.h"

namespace vanillapdf {
namespace semantics {

using namespace syntax;

MessageDigestAlgorithm DocumentSignatureSettings::GetDigest() const {
    return m_digest;
}

void DocumentSignatureSettings::SetDigest(MessageDigestAlgorithm digest) {
    m_digest = digest;
}

bool DocumentSignatureSettings::GetSigningKey(OutputPointer<ISigningKeyPtr>& result) const {
    if (m_key.empty()) {
        return false;
    }

    result = m_key;
    return true;
}

bool DocumentSignatureSettings::GetCertificate(OutputPointer<HexadecimalStringObjectPtr>& result) const {
    if (m_certificate.empty()) {
        return false;
    }

    result = m_certificate;
    return true;
}

bool DocumentSignatureSettings::GetName(OutputPointer<syntax::LiteralStringObjectPtr>& result) const {
    if (m_name.empty()) {
        return false;
    }

    result = m_name;
    return true;
}

bool DocumentSignatureSettings::GetLocation(OutputPointer<syntax::LiteralStringObjectPtr>& result) const {
    if (m_location.empty()) {
        return false;
    }

    result = m_location;
    return true;
}

bool DocumentSignatureSettings::GetReason(OutputPointer<syntax::LiteralStringObjectPtr>& result) const {
    if (m_reason.empty()) {
        return false;
    }

    result = m_reason;
    return true;
}

bool DocumentSignatureSettings::GetSigningTime(OutputPointer<DatePtr>& result) const {
    if (m_time.empty()) {
        return false;
    }

    result = m_time;
    return true;
}

void DocumentSignatureSettings::SetSigningKey(ISigningKeyPtr value) {
    m_key = value;
}

void DocumentSignatureSettings::SetCertificate(HexadecimalStringObjectPtr value) {
    m_certificate = value;
}

void DocumentSignatureSettings::SetName(LiteralStringObjectPtr value) {
    m_name = value;
}

void DocumentSignatureSettings::SetLocation(LiteralStringObjectPtr value) {
    m_location = value;
}

void DocumentSignatureSettings::SetReason(LiteralStringObjectPtr value) {
    m_reason = value;
}

void DocumentSignatureSettings::SetSigningTime(DatePtr value) {
    m_time = value;
}

bool DocumentSignatureSettings::GetNameText(PdfTextStringPtr& result) const {
    if (m_name.empty()) {
        return false;
    }
    LiteralStringObjectPtr str = m_name;
    result = PdfTextString::CreateFromStringObject(str);
    return true;
}

bool DocumentSignatureSettings::GetLocationText(PdfTextStringPtr& result) const {
    if (m_location.empty()) {
        return false;
    }
    LiteralStringObjectPtr str = m_location;
    result = PdfTextString::CreateFromStringObject(str);
    return true;
}

bool DocumentSignatureSettings::GetReasonText(PdfTextStringPtr& result) const {
    if (m_reason.empty()) {
        return false;
    }
    LiteralStringObjectPtr str = m_reason;
    result = PdfTextString::CreateFromStringObject(str);
    return true;
}

void DocumentSignatureSettings::SetNameText(PdfTextStringPtr value) {
    auto raw = value->GetStringRaw();
    auto str_obj = LiteralStringObject::CreateFromDecoded(raw);
    SetName(str_obj);
}

void DocumentSignatureSettings::SetLocationText(PdfTextStringPtr value) {
    auto raw = value->GetStringRaw();
    auto str_obj = LiteralStringObject::CreateFromDecoded(raw);
    SetLocation(str_obj);
}

void DocumentSignatureSettings::SetReasonText(PdfTextStringPtr value) {
    auto raw = value->GetStringRaw();
    auto str_obj = LiteralStringObject::CreateFromDecoded(raw);
    SetReason(str_obj);
}

} // semantics
} // vanillapdf
