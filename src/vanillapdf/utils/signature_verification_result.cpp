#include "precompiled.h"

#include "utils/signature_verification_result.h"

namespace vanillapdf {

SignatureVerificationResult::SignatureVerificationResult()
    : m_status(SignatureVerificationStatus::Unknown)
    , m_signature_valid(false)
    , m_document_intact(false)
    , m_certificate_trusted(false) {
}

SignatureVerificationStatus SignatureVerificationResult::GetStatus() const {
    return m_status;
}

BufferPtr SignatureVerificationResult::GetMessage() const {
    return m_message;
}

bool SignatureVerificationResult::IsSignatureValid() const {
    return m_signature_valid;
}

bool SignatureVerificationResult::IsDocumentIntact() const {
    return m_document_intact;
}

bool SignatureVerificationResult::IsCertificateTrusted() const {
    return m_certificate_trusted;
}

BufferPtr SignatureVerificationResult::GetSignerCertificate() const {
    return m_signer_certificate;
}

types::size_type SignatureVerificationResult::GetCertificateChainCount() const {
    return m_certificate_chain.size();
}

BufferPtr SignatureVerificationResult::GetCertificateChainAt(types::size_type index) const {
    if (index >= m_certificate_chain.size()) {
        LOG_ERROR_AND_THROW(CryptoErrorException,"Certificate chain index out of range: {}", index);
    }
    return m_certificate_chain[index];
}

BufferPtr SignatureVerificationResult::GetSignerCommonName() const {
    return m_signer_common_name;
}

// Setters

void SignatureVerificationResult::SetStatus(SignatureVerificationStatus status) {
    m_status = status;
}

void SignatureVerificationResult::SetMessage(BufferPtr message) {
    m_message = message;
}

void SignatureVerificationResult::SetMessage(std::string_view message) {
    m_message = Buffer::CreateFromString(message);
}

void SignatureVerificationResult::SetSignatureValid(bool valid) {
    m_signature_valid = valid;
}

void SignatureVerificationResult::SetDocumentIntact(bool intact) {
    m_document_intact = intact;
}

void SignatureVerificationResult::SetCertificateTrusted(bool trusted) {
    m_certificate_trusted = trusted;
}

void SignatureVerificationResult::SetSignerCertificate(BufferPtr cert) {
    m_signer_certificate = cert;
}

void SignatureVerificationResult::AddCertificateToChain(BufferPtr cert) {
    m_certificate_chain.push_back(cert);
}

void SignatureVerificationResult::SetSignerCommonName(BufferPtr name) {
    m_signer_common_name = name;
}

void SignatureVerificationResult::SetSignerCommonName(std::string_view name) {
    m_signer_common_name = Buffer::CreateFromString(name);
}

} // vanillapdf
