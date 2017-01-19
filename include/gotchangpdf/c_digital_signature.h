#ifndef _C_DIGITAL_SIGNATURE_H
#define _C_DIGITAL_SIGNATURE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_digital_signature.h
	* This file contains class definitions for DigitalSignatureHandle
	*/

	/**
	* \class DigitalSignatureHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief Represents document's authenticated digital signature
	*/

	/**
	* \class ByteRangesHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief Represents a collection of ByteRangeHandle
	*/

	/**
	* \class ByteRangeHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief Represents starting byte offset and length
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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetContactInfo(DigitalSignatureHandle handle, StringObjectHandle* result);

	/**
	* \brief
	* The reason for the signing, such as (I agree...).
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetReason(DigitalSignatureHandle handle, StringObjectHandle* result);

	/**
	* \brief
	* The CPU host name or physical location of the signing.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetLocation(DigitalSignatureHandle handle, StringObjectHandle* result);

	/**
	* \brief
	* The time of signing.
	*
	* Depending on the signature handler, this may be a normal unverified
	* computer time or a time generated in a verifiable way from a secure time server.
	*
	* This value should be used only when the time of signing is not available in the signature.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetDate(DigitalSignatureHandle handle, DateHandle* result);

	/**
	* \brief
	* The name of the person or authority signing the document.
	*
	* This value should be used only when it is not possible to extract the name from the signature.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetName(DigitalSignatureHandle handle, StringObjectHandle* result);

	/**
	* \brief
	* The version of the signature handler that was used to create the signature.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetRevision(DigitalSignatureHandle handle, IntegerObjectHandle* result);

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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetCertificate(DigitalSignatureHandle handle, StringObjectHandle* result);

	/**
	* \brief
	* The signature value.
	*
	* When ByteRange is present, the value shall be
	* a hexadecimal string (see 7.3.4.3, "Hexadecimal Strings")
	* representing the value of the byte range digest.
	*
	* For public-key signatures, Contents should be either a DER-encoded PKCS#1
	* binary data object or a DER-encoded PKCS#7 binary data object.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetContents(DigitalSignatureHandle handle, HexadecimalStringObjectHandle* result);

	/**
	* \brief
	* Exact byte range for the digest calculation.
	*
	* Multiple discontiguous byte ranges shall be used to describe
	* a digest that does not include the signature value (theContents entry) itself.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_GetByteRange(DigitalSignatureHandle handle, ByteRangesHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DigitalSignature_Release(DigitalSignatureHandle handle);

	/** @} */

	/**
	* \memberof ByteRangesHandle
	* @{
	*/

	/**
	* \brief Get size of byte range collection
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ByteRanges_Size(ByteRangesHandle handle, size_type* result);

	/**
	* \brief
	* Get single byte range from array at specific position
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ByteRanges_At(ByteRangesHandle handle, size_type at, ByteRangeHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ByteRanges_Release(ByteRangesHandle handle);

	/** @} */

	/**
	* \memberof ByteRangeHandle
	* @{
	*/

	/**
	* \brief Get starting offset of byte range
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ByteRange_GetOffset(ByteRangeHandle handle, IntegerObjectHandle* result);

	/**
	* \brief Get length of byte range
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ByteRange_GetLength(ByteRangeHandle handle, IntegerObjectHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ByteRange_Release(ByteRangeHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DIGITAL_SIGNATURE_H */
