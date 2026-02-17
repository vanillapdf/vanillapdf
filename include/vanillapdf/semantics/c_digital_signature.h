#ifndef _C_DIGITAL_SIGNATURE_H
#define _C_DIGITAL_SIGNATURE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_digital_signature.h
    * \brief This file contains class definitions for \ref DigitalSignatureHandle
    */

    /**
    * \class DigitalSignatureHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief Represents document's authenticated digital signature
    *
    * For more details please visit [section 12.8 - Digital Signatures](PDF32000_2008.pdf#G11.2013350).
    */

    /**
    * \memberof DigitalSignatureHandle
    * @{
    */

    /**
    * \brief
    * Information provided by the signer to enable a recipient
    * to contact the signer to verify the signature.
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetContactInfo(DigitalSignatureHandle* handle, StringObjectHandle** result);

    /**
    * \brief
    * The reason for the signing, such as (I agree...).
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetReason(DigitalSignatureHandle* handle, StringObjectHandle** result);

    /**
    * \brief
    * The CPU host name or physical location of the signing.
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetLocation(DigitalSignatureHandle* handle, StringObjectHandle** result);

    /**
    * \brief
    * The time of signing.
    *
    * Depending on the signature handler, this may be a normal unverified
    * computer time or a time generated in a verifiable way from a secure time server.
    *
    * This value should be used only when the time of signing is not available in the signature.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetSigningTime(DigitalSignatureHandle* handle, DateHandle** result);

    /**
    * \brief
    * The name of the person or authority signing the document.
    *
    * This value should be used only when it is not possible to extract the name from the signature.
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetName(DigitalSignatureHandle* handle, StringObjectHandle** result);

    /**
    * \brief
    * The version of the signature handler that was used to create the signature.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetRevision(DigitalSignatureHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief
    * An array of byte strings that shall represent the X.509 certificate chain
    * used when signing and verifying signatures that use public-key cryptography,
    * or a byte string if the chain has only one entry.
    *
    * The signing certificate shall appear first in the array.
    * It shall be used to verify the signature value in Contents,
    * and the other certificates shall be used to verify
    * the authenticity of the signing certificate.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetCertificate(DigitalSignatureHandle* handle, StringObjectHandle** result);

    /**
    * \brief
    * The signature value.
    *
    * When ByteRange is present, the value shall be a \ref HexadecimalStringObjectHandle
    * representing the value of the byte range digest.
    *
    * For public-key signatures, Contents should be either a DER-encoded PKCS#1
    * binary data object or a DER-encoded PKCS#7 binary data object.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetContents(DigitalSignatureHandle* handle, HexadecimalStringObjectHandle** result);

    /**
    * \brief
    * Exact byte range for the digest calculation.
    *
    * Multiple discontiguous byte ranges shall be used to describe
    * a digest that does not include the signature value (\ref DigitalSignature_GetContents) itself.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetByteRange(DigitalSignatureHandle* handle, ByteRangeCollectionHandle** result);

    /**
    * \brief Get the contact info as a \ref PdfTextStringHandle (encoding-aware).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetContactInfoText(DigitalSignatureHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Get the reason as a \ref PdfTextStringHandle (encoding-aware).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetReasonText(DigitalSignatureHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Get the location as a \ref PdfTextStringHandle (encoding-aware).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetLocationText(DigitalSignatureHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Get the signer name as a \ref PdfTextStringHandle (encoding-aware).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetNameText(DigitalSignatureHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_ToUnknown(DigitalSignatureHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref DigitalSignatureHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_FromUnknown(IUnknownHandle* handle, DigitalSignatureHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_Release(DigitalSignatureHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DIGITAL_SIGNATURE_H */
