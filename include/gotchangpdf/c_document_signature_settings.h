#ifndef _C_DOCUMENT_SIGNATURE_SETTINGS_H
#define _C_DOCUMENT_SIGNATURE_SETTINGS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_message_digest_algorithm.h"
#include "c_signing_key.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_document_signature_settings.h
	* This file contains class definitions for DocumentSignatureSettingsHandle
	*/

	/**
	* \class DocumentSignatureSettingsHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief TODO
	* \see DocumentHandle
	*/

	/**
	* \memberof DocumentSignatureSettingsHandle
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetDigest(DocumentSignatureSettingsHandle handle, MessageDigestAlgorithmType* result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetDigest(DocumentSignatureSettingsHandle handle, MessageDigestAlgorithmType value);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetSigningKey(DocumentSignatureSettingsHandle handle, SigningKeyHandle* result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetSigningKey(DocumentSignatureSettingsHandle handle, SigningKeyHandle value);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetName(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle* result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetName(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle value);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetLocation(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle* result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetLocation(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle value);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetReason(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle* result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetReason(DocumentSignatureSettingsHandle handle, LiteralStringObjectHandle value);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_GetCertificate(DocumentSignatureSettingsHandle handle, HexadecimalStringObjectHandle* result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_SetCertificate(DocumentSignatureSettingsHandle handle, HexadecimalStringObjectHandle value);


	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentSignatureSettings_Release(DocumentSignatureSettingsHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_SIGNATURE_SETTINGS_H */
