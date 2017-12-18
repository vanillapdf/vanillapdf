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
	*/

	/**
	* \class ByteRangeCollectionHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
	* \brief Represents a collection of \ref ByteRangeHandle
	*/

	/**
	* \class ByteRangeHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
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
	VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetContactInfo(DigitalSignatureHandle* handle, StringObjectHandle** result);

	/**
	* \brief
	* The reason for the signing, such as (I agree...).
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetReason(DigitalSignatureHandle* handle, StringObjectHandle** result);

	/**
	* \brief
	* The CPU host name or physical location of the signing.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetLocation(DigitalSignatureHandle* handle, StringObjectHandle** result);

	/**
	* \brief
	* The time of signing.
	*
	* Depending on the signature handler, this may be a normal unverified
	* computer time or a time generated in a verifiable way from a secure time server.
	*
	* This value should be used only when the time of signing is not available in the signature.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetDate(DigitalSignatureHandle* handle, DateHandle** result);

	/**
	* \brief
	* The name of the person or authority signing the document.
	*
	* This value should be used only when it is not possible to extract the name from the signature.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetName(DigitalSignatureHandle* handle, StringObjectHandle** result);

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
	* When ByteRange is present, the value shall be
	* a hexadecimal string (see 7.3.4.3, "Hexadecimal Strings")
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
	* a digest that does not include the signature value (theContents entry) itself.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_GetByteRange(DigitalSignatureHandle* handle, ByteRangeCollectionHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DigitalSignature_Release(DigitalSignatureHandle* handle);

	/** @} */

	/**
	* \memberof ByteRangeCollectionHandle
	* @{
	*/

	/**
	* \brief Get size of byte range collection
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Size(ByteRangeCollectionHandle* handle, size_type* result);

	/**
	* \brief
	* Get single byte range from array at specific position
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_At(ByteRangeCollectionHandle* handle, size_type at, ByteRangeHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ByteRangeCollection_Release(ByteRangeCollectionHandle* handle);

	/** @} */

	/**
	* \memberof ByteRangeHandle
	* @{
	*/

	/**
	* \brief Get starting offset of byte range
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_GetOffset(ByteRangeHandle* handle, IntegerObjectHandle** result);

	/**
	* \brief Get length of byte range
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_GetLength(ByteRangeHandle* handle, IntegerObjectHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ByteRange_Release(ByteRangeHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DIGITAL_SIGNATURE_H */
