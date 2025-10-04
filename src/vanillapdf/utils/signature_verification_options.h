#ifndef _SIGNATURE_VERIFICATION_OPTIONS_H
#define _SIGNATURE_VERIFICATION_OPTIONS_H

#include "utils/utils_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/trusted_certificate_store.h"

namespace vanillapdf {

/**
* \enum VerificationFlags
* \brief Flags controlling signature verification behavior
*/
enum class VerificationFlags {
    None = 0,
    CheckRevocation = 1 << 0,          ///< Check CRL/OCSP for certificate revocation
    RequireTrustedRoot = 1 << 1,       ///< Require certificate chain to a trusted root
    AllowExpiredCerts = 1 << 2,        ///< Allow expired certificates
    CheckSigningTime = 1 << 3          ///< Validate certificate was valid at signing time
};

// Bitwise operators for VerificationFlags
inline VerificationFlags operator|(VerificationFlags a, VerificationFlags b) {
    return static_cast<VerificationFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline VerificationFlags operator&(VerificationFlags a, VerificationFlags b) {
    return static_cast<VerificationFlags>(static_cast<int>(a) & static_cast<int>(b));
}

inline bool operator!(VerificationFlags a) {
    return static_cast<int>(a) == 0;
}

/**
* \class SignatureVerificationOptions
* \brief Configuration options for signature verification
*
* Provides control over how signature verification is performed,
* including which certificates to trust and what validation checks to apply.
*/
class SignatureVerificationOptions : public IUnknown {
public:
    SignatureVerificationOptions();
    ~SignatureVerificationOptions() = default;

    /**
    * \brief Set trusted certificate store
    * \param store Certificate store containing trusted certificates
    *
    * If not set, system default certificates will be used.
    */
    void SetTrustedCertificates(TrustedCertificateStorePtr store);

    /**
    * \brief Set verification flags
    * \param flags Bitwise OR of VerificationFlags values
    */
    void SetFlags(VerificationFlags flags);

    /**
    * \brief Get trusted certificate store
    * \return Certificate store or nullptr if not set (will use system defaults)
    */
    TrustedCertificateStorePtr GetTrustedCertificates() const;

    /**
    * \brief Get verification flags
    * \return Current verification flags
    */
    VerificationFlags GetFlags() const;

private:
    TrustedCertificateStorePtr m_trusted_store;
    VerificationFlags m_flags;
};

} // vanillapdf

#endif /* _SIGNATURE_VERIFICATION_OPTIONS_H */
