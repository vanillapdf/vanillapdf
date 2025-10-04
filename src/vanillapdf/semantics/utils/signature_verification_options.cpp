#include "precompiled.h"

#include "semantics/utils/signature_verification_options.h"

namespace vanillapdf {
namespace semantics {

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

DatePtr SignatureVerificationOptions::GetVerificationTime() const {
    return m_verification_time;
}

} // semantics
} // vanillapdf
