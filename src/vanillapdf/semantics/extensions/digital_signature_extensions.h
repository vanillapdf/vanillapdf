#ifndef _DIGITAL_SIGNATURE_EXTENSIONS_H
#define _DIGITAL_SIGNATURE_EXTENSIONS_H

#include "semantics/utils/semantics_fwd.h"
#include "utils/utils_fwd.h"
#include "utils/signature_verifier.h"

namespace vanillapdf {
namespace semantics {

/**
* \class DigitalSignatureExtensions
* \brief Extension methods for DigitalSignature objects
*
* This class provides extension methods (similar to C# extension methods)
* that add functionality to DigitalSignature without modifying the class itself.
* These methods are implemented as static methods that take the object as the first parameter.
*/
class DigitalSignatureExtensions {
public:
    /**
    * \brief Verify the digital signature in a PDF document
    * \param signature The digital signature object to verify
    * \param document The PDF document containing the signature
    * \param trusted_store Certificate store for chain validation (required)
    * \param settings Verification behavior settings (optional, uses defaults if empty)
    * \return Detailed verification result
    *
    * This extension method:
    * 1. Extracts the ByteRange from the signature
    * 2. Reads the signed bytes from the document file using ByteRange
    * 3. Extracts the signature Contents (PKCS#7 blob)
    * 4. Calls SignatureVerifier::Verify with the extracted data
    * 5. Returns comprehensive verification result
    *
    * \note The signature must contain valid ByteRange and Contents entries.
    *       The document must be the same document from which the signature was obtained.
    *       If settings is empty, default settings are used (all flags disabled).
    */
    static SignatureVerificationResultPtr Verify(
        DigitalSignaturePtr signature,
        DocumentPtr document,
        TrustedCertificateStorePtr trusted_store,
        SignatureVerificationSettingsPtr settings
    );

private:
    DigitalSignatureExtensions() = delete;
    ~DigitalSignatureExtensions() = delete;
};

} // semantics
} // vanillapdf

#endif /* _DIGITAL_SIGNATURE_EXTENSIONS_H */
