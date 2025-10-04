#include "precompiled.h"

#include "utils/signature_verification_options.h"

namespace vanillapdf {

SignatureVerificationOptions::SignatureVerificationOptions()
    : m_flags(VerificationFlags::None) {
}

void SignatureVerificationOptions::SetTrustedCertificates(TrustedCertificateStorePtr store) {
    m_trusted_store = store;
}

void SignatureVerificationOptions::SetFlags(VerificationFlags flags) {
    m_flags = flags;
}

void SignatureVerificationOptions::SetVerificationTime(DatePtr time) {
    m_verification_time = time;
}

TrustedCertificateStorePtr SignatureVerificationOptions::GetTrustedCertificates() const {
    return m_trusted_store;
}

VerificationFlags SignatureVerificationOptions::GetFlags() const {
    return m_flags;
}

} // vanillapdf
