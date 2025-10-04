#ifndef _SIGNATURE_VERIFICATION_OPTIONS_H
#define _SIGNATURE_VERIFICATION_OPTIONS_H

#include "semantics/utils/semantics_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/trusted_certificate_store.h"
#include "semantics/objects/date.h"

namespace vanillapdf {
namespace semantics {

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
    * \brief Set reference time for verification
    * \param time Time to use for verification (nullptr = current time)
    *
    * Useful for checking if signature was valid at a specific point in time.
    */
    void SetVerificationTime(DatePtr time);

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

    /**
    * \brief Get verification time
    * \return Verification time or nullptr if using current time
    */
    DatePtr GetVerificationTime() const;

private:
    TrustedCertificateStorePtr m_trusted_store;
    VerificationFlags m_flags;
    DatePtr m_verification_time;
};

} // semantics
} // vanillapdf

#endif /* _SIGNATURE_VERIFICATION_OPTIONS_H */
