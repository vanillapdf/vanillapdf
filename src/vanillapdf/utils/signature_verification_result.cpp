#include "precompiled.h"

#include "utils/signature_verification_result.h"

namespace vanillapdf {

SignatureVerificationResult::SignatureVerificationResult()
    : m_status(SignatureVerificationStatus::Unknown)
    , m_signature_valid(false)
    , m_document_intact(false)
    , m_certificate_trusted(false)
    , m_certificate_valid_at_signing(false) {
}

SignatureVerificationStatus SignatureVerificationResult::GetStatus() const {
    return m_status;
}

std::string SignatureVerificationResult::GetMessage() const {
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

bool SignatureVerificationResult::IsCertificateValidAtSigningTime() const {
    return m_certificate_valid_at_signing;
}

BufferPtr SignatureVerificationResult::GetSignerCertificate() const {
    return m_signer_certificate;
}

types::size_type SignatureVerificationResult::GetCertificateChainCount() const {
    return m_certificate_chain.size();
}

BufferPtr SignatureVerificationResult::GetCertificateChainAt(types::size_type index) const {
    if (index >= m_certificate_chain.size()) {
        LOG_ERROR_AND_THROW_GENERAL("Certificate chain index out of range: {}", index);
    }
    return m_certificate_chain[index];
}

std::string SignatureVerificationResult::GetSignerCommonName() const {
    return m_signer_common_name;
}

// Setters

void SignatureVerificationResult::SetStatus(SignatureVerificationStatus status) {
    m_status = status;
}

void SignatureVerificationResult::SetMessage(const std::string& message) {
    m_message = message;
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

void SignatureVerificationResult::SetCertificateValidAtSigningTime(bool valid) {
    m_certificate_valid_at_signing = valid;
}

void SignatureVerificationResult::SetSignerCertificate(BufferPtr cert) {
    m_signer_certificate = cert;
}

void SignatureVerificationResult::AddCertificateToChain(BufferPtr cert) {
    m_certificate_chain.push_back(cert);
}

void SignatureVerificationResult::SetSignerCommonName(const std::string& name) {
    m_signer_common_name = name;
}

} // vanillapdf
