#include "precompiled.h"

#include "semantics/utils/document_signature_settings.h"

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

bool DocumentSignatureSettings::GetName(OutputPointer<LiteralStringObjectPtr>& result) const {
	if (m_name.empty()) {
		return false;
	}

	result = m_name;
	return true;
}

bool DocumentSignatureSettings::GetLocation(OutputPointer<LiteralStringObjectPtr>& result) const {
	if (m_location.empty()) {
		return false;
	}

	result = m_location;
	return true;
}

bool DocumentSignatureSettings::GetReason(OutputPointer<LiteralStringObjectPtr>& result) const {
	if (m_reason.empty()) {
		return false;
	}

	result = m_reason;
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

} // semantics
} // vanillapdf
