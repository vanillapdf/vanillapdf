#ifndef _C_DOCUMENT_SIGNATURE_SETTINGS_H
#define _C_DOCUMENT_SIGNATURE_SETTINGS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#include "vanillapdf/utils/c_message_digest_algorithm.h"
#include "vanillapdf/utils/c_signing_key.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_document_signature_settings.h
	* \brief This file contains class definitions for \ref DocumentSignatureSettingsHandle
	*/

	/**
	* \class DocumentSignatureSettingsHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
	* \brief Group of settings controlling document's digital signature
	* \see \ref DigitalSignatureHandle
	*/

	/**
	* \memberof DocumentSignatureSettingsHandle
	* @{
	*/

	/**
	* \brief Message digest algorithm for calculation hash of the data to be signed
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetDigest(DocumentSignatureSettingsHandle* handle, MessageDigestAlgorithmType* result);

	/**
	* \brief \copybrief DocumentSignatureSettings_GetDigest
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetDigest(DocumentSignatureSettingsHandle* handle, MessageDigestAlgorithmType value);

	/**
	* \brief Get handle to key used for calculating document signature value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetSigningKey(DocumentSignatureSettingsHandle* handle, SigningKeyHandle** result);

	/**
	* \brief \copybrief DocumentSignatureSettings_GetSigningKey
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetSigningKey(DocumentSignatureSettingsHandle* handle, SigningKeyHandle* value);

	/**
	* \brief Set the name of the person or authority signing the document.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetName(DocumentSignatureSettingsHandle* handle, LiteralStringObjectHandle** result);

	/**
	* \brief \copybrief DocumentSignatureSettings_GetName
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetName(DocumentSignatureSettingsHandle* handle, LiteralStringObjectHandle* value);

	/**
	* \brief Get the CPU host name or physical location of the signing.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetLocation(DocumentSignatureSettingsHandle* handle, LiteralStringObjectHandle** result);

	/**
	* \brief \copybrief DocumentSignatureSettings_GetLocation
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetLocation(DocumentSignatureSettingsHandle* handle, LiteralStringObjectHandle* value);

	/**
	* \brief Set the reason for the signing, such as (I agree...).
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetReason(DocumentSignatureSettingsHandle* handle, LiteralStringObjectHandle** result);

	/**
	* \brief \copybrief DocumentSignatureSettings_GetReason
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetReason(DocumentSignatureSettingsHandle* handle, LiteralStringObjectHandle* value);

	/**
	* \brief
	* An array of byte strings that shall represent the X.509 certificate chain
	* used when signing and verifying signatures that use public-key cryptography,
	* or a byte string if the chain has only one entry.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetCertificate(DocumentSignatureSettingsHandle* handle, HexadecimalStringObjectHandle** result);

	/**
	* \brief \copybrief DocumentSignatureSettings_GetCertificate
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetCertificate(DocumentSignatureSettingsHandle* handle, HexadecimalStringObjectHandle* value);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_Release(DocumentSignatureSettingsHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_SIGNATURE_SETTINGS_H */
