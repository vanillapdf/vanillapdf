#ifndef _C_DIGITAL_SIGNATURE_EXTENSIONS_H
#define _C_DIGITAL_SIGNATURE_EXTENSIONS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"
#include "vanillapdf/utils/c_signature_verifier.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_digital_signature_extensions.h
    * \brief Extension methods for DigitalSignature objects
    */

    /**
    * \brief Verify the digital signature in a PDF document
    * \param signature The digital signature object to verify
    * \param document The PDF document containing the signature
    * \param trusted_store Certificate store for chain validation (required)
    * \param settings Verification settings (optional, can be NULL for defaults)
    * \param result Output verification result
    *
    * This extension method:
    * 1. Extracts the ByteRange from the signature
    * 2. Reads the signed bytes from the document file using ByteRange
    * 3. Extracts the signature Contents (PKCS#7 blob)
    * 4. Calls SignatureVerifier_Verify with the extracted data
    * 5. Returns comprehensive verification result
    *
    * \note The signature must contain valid ByteRange and Contents entries.
    *       The document must be the same document from which the signature was obtained.
    *       If settings is NULL, default settings are used (all flags disabled).
    *
    * \see SignatureVerifier_Verify for low-level signature verification
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignatureExtensions_Verify(
        DigitalSignatureHandle* signature,
        DocumentHandle* document,
        TrustedCertificateStoreHandle* trusted_store,
        SignatureVerificationSettingsHandle* settings,
        SignatureVerificationResultHandle** result
    );

#ifdef __cplusplus
};
#endif

#endif /* _C_DIGITAL_SIGNATURE_EXTENSIONS_H */
