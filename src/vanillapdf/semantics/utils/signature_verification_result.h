#ifndef _SIGNATURE_VERIFICATION_RESULT_H
#define _SIGNATURE_VERIFICATION_RESULT_H

#include "semantics/utils/semantics_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/buffer.h"
#include "semantics/objects/date.h"

#include <string>
#include <vector>

namespace vanillapdf {
namespace semantics {

/**
* \enum SignatureVerificationStatus
* \brief Overall status of signature verification
*/
enum class SignatureVerificationStatus {
    Valid = 0,                    ///< Signature is cryptographically valid
    Invalid,                      ///< Signature verification failed
    CertificateExpired,          ///< Certificate has expired
    CertificateNotYetValid,      ///< Certificate not yet valid
    CertificateRevoked,          ///< Certificate has been revoked
    CertificateUntrusted,        ///< Certificate chain not trusted
    DocumentModified,            ///< Document modified after signing
    WeakAlgorithm,               ///< Weak digest algorithm (MD5, SHA1)
    MissingCertificate,          ///< Signature missing certificate
    Unknown                      ///< Unable to determine status
};

/**
* \class SignatureVerificationResult
* \brief Result of signature verification operation
*
* Contains detailed information about the verification process including
* signature validity, certificate status, and certificate chain information.
*/
class SignatureVerificationResult : public IUnknown {
public:
    SignatureVerificationResult();
    ~SignatureVerificationResult() = default;

    /**
    * \brief Get overall verification status
    * \return Verification status enum
    */
    SignatureVerificationStatus GetStatus() const;

    /**
    * \brief Get human-readable message describing the result
    * \return Description message
    */
    std::string GetMessage() const;

    /**
    * \brief Check if signature is cryptographically valid
    * \return true if signature matches the document content
    */
    bool IsSignatureValid() const;

    /**
    * \brief Check if document bytes are intact
    * \return true if ByteRange matches and document unmodified
    */
    bool IsDocumentIntact() const;

    /**
    * \brief Check if certificate chain is trusted
    * \return true if chain validates to a trusted root
    */
    bool IsCertificateTrusted() const;

    /**
    * \brief Check if certificate was valid at signing time
    * \return true if certificate was valid when signature was created
    */
    bool IsCertificateValidAtSigningTime() const;

    /**
    * \brief Get signer's certificate
    * \return DER-encoded certificate or nullptr if not available
    */
    BufferPtr GetSignerCertificate() const;

    /**
    * \brief Get number of certificates in the chain
    * \return Certificate chain count
    */
    types::size_type GetCertificateChainCount() const;

    /**
    * \brief Get certificate from chain at index
    * \param index Certificate index (0 = signer, 1+ = intermediate/root)
    * \return DER-encoded certificate
    */
    BufferPtr GetCertificateChainAt(types::size_type index) const;

    /**
    * \brief Get signing time from signature
    * \return Signing time or nullptr if not available
    */
    DatePtr GetSigningTime() const;

    /**
    * \brief Get certificate validity start date
    * \return Certificate NotBefore date
    */
    DatePtr GetCertificateNotBefore() const;

    /**
    * \brief Get certificate validity end date
    * \return Certificate NotAfter date
    */
    DatePtr GetCertificateNotAfter() const;

    /**
    * \brief Get signer's common name from certificate
    * \return Common name (CN) field from certificate subject
    */
    std::string GetSignerCommonName() const;

    // Setters for building the result (internal use)
    void SetStatus(SignatureVerificationStatus status);
    void SetMessage(const std::string& message);
    void SetSignatureValid(bool valid);
    void SetDocumentIntact(bool intact);
    void SetCertificateTrusted(bool trusted);
    void SetCertificateValidAtSigningTime(bool valid);
    void SetSignerCertificate(BufferPtr cert);
    void AddCertificateToChain(BufferPtr cert);
    void SetSigningTime(DatePtr time);
    void SetCertificateNotBefore(DatePtr date);
    void SetCertificateNotAfter(DatePtr date);
    void SetSignerCommonName(const std::string& name);

private:
    SignatureVerificationStatus m_status;
    std::string m_message;
    bool m_signature_valid;
    bool m_document_intact;
    bool m_certificate_trusted;
    bool m_certificate_valid_at_signing;
    BufferPtr m_signer_certificate;
    std::vector<BufferPtr> m_certificate_chain;
    DatePtr m_signing_time;
    DatePtr m_cert_not_before;
    DatePtr m_cert_not_after;
    std::string m_signer_common_name;
};

} // semantics
} // vanillapdf

#endif /* _SIGNATURE_VERIFICATION_RESULT_H */
