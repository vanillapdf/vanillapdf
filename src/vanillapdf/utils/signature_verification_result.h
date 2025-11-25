#ifndef _SIGNATURE_VERIFICATION_RESULT_H
#define _SIGNATURE_VERIFICATION_RESULT_H

#include "utils/utils_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/buffer.h"

#include <string>
#include <vector>

namespace vanillapdf {

/**
* \enum SignatureVerificationStatus
* \brief Overall status of signature verification
*/
enum class SignatureVerificationStatus {
    Undefined = 0,               ///< Status not set (uninitialized)
    Valid,                       ///< Signature is cryptographically valid
    Invalid,                     ///< Signature verification failed
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
    * \return Description message as buffer (UTF-8 encoded)
    */
    BufferPtr GetMessage() const;

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
    * \brief Get signer's common name from certificate
    * \return Common name (CN) field from certificate subject as buffer (UTF-8 encoded)
    */
    BufferPtr GetSignerCommonName() const;

    // Setters for building the result (internal use)
    void SetStatus(SignatureVerificationStatus status);
    void SetMessage(BufferPtr message);
    void SetMessage(std::string_view message);  ///< Convenience overload for UTF-8 strings
    void SetSignatureValid(bool valid);
    void SetDocumentIntact(bool intact);
    void SetCertificateTrusted(bool trusted);
    void SetSignerCertificate(BufferPtr cert);
    void AddCertificateToChain(BufferPtr cert);
    void SetSignerCommonName(BufferPtr name);
    void SetSignerCommonName(std::string_view name);  ///< Convenience overload for UTF-8 strings

private:
    SignatureVerificationStatus m_status;
    BufferPtr m_message;                    ///< UTF-8 encoded message
    bool m_signature_valid;
    bool m_document_intact;
    bool m_certificate_trusted;
    BufferPtr m_signer_certificate;         ///< DER-encoded certificate
    std::vector<BufferPtr> m_certificate_chain;
    BufferPtr m_signer_common_name;         ///< UTF-8 encoded common name
};

} // vanillapdf

#endif /* _SIGNATURE_VERIFICATION_RESULT_H */
