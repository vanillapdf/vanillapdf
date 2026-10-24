#ifndef _CRYPTO_SETTINGS_H
#define _CRYPTO_SETTINGS_H

#include "utils/utils_fwd.h"

namespace vanillapdf {

/**
* \class CryptoSettings
* \brief Configuration of the cryptographic toolkit the library uses
*
* Applied once by CryptoUtils::Initialize, before the first cryptographic
* operation. Without an explicit initialization the defaults apply.
*
* Example usage:
* \code
* auto settings = make_deferred<CryptoSettings>();
* settings->SetLegacyAlgorithmsPolicy(CryptoSettings::LegacyAlgorithmsPolicy::Required);
*
* CryptoUtils::Initialize(*settings);
* \endcode
*/
class CryptoSettings : public IUnknown {
public:
    /**
    * \brief Whether the algorithms only old documents and key files use are enabled
    *
    * Legacy algorithms are RC2 (envelopes of certificate encrypted documents,
    * PKCS#12 key files) and MD2/MD4 (signatures). RC4 and MD5 of the standard
    * security handler are implemented in-house and are always available.
    */
    enum class LegacyAlgorithmsPolicy {
        Undefined = 0,

        /// Never enable them
        Disabled,

        /// Enable them when available, otherwise continue without them
        Optional,

        /// Fail the initialization when they cannot be enabled
        Required
    };

    /**
    * \brief Get the legacy algorithms policy
    * \return The policy, LegacyAlgorithmsPolicy::Optional by default
    */
    LegacyAlgorithmsPolicy GetLegacyAlgorithmsPolicy(void) const noexcept {
        return m_legacy_algorithms_policy;
    }

    /**
    * \brief Set the legacy algorithms policy
    * \param policy The policy to apply at initialization
    */
    void SetLegacyAlgorithmsPolicy(LegacyAlgorithmsPolicy policy) noexcept {
        m_legacy_algorithms_policy = policy;
    }

private:
    LegacyAlgorithmsPolicy m_legacy_algorithms_policy = LegacyAlgorithmsPolicy::Optional;
};

} // vanillapdf

#endif /* _CRYPTO_SETTINGS_H */
