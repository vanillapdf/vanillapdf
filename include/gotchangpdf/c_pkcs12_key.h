#ifndef _C_PKCS12_KEY_H
#define _C_PKCS12_KEY_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_pkcs12_key.h
	* This file contains class definitions for PKCS12KeyHandle
	*/

	/**
	* \class PKCS12KeyHandle
	* \extends IUnknownHandle
	* \ingroup Files
	* \brief TODO
	*/

	/**
	* \memberof PKCS12KeyHandle
	* @{
	*/

	/**
	* \brief Uses PKCS#12 standard format to provide encryption keys
	* \param path a string path to file containing the PKCS#12 data structure
	* \param password a protection password to the data structure. If there is no password use NULL.
	* \param result a pointer to library allocated EncryptionKeyHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PKCS12Key_CreateFromFile(string_type path, string_type password, PKCS12KeyHandle* result);

	/**
	* \brief Uses PKCS#12 standard format to provide encryption keys
	* \param data a buffer containing PKCS#12 structure with the decryption keys
	* \param password a protection password to the data structure. If there is no password use NULL.
	* \param result a pointer to library allocated EncryptionKeyHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PKCS12Key_CreateFromBuffer(BufferHandle data, string_type password, PKCS12KeyHandle* result);

	/**
	* \brief Reinterpret current object as EncryptionKeyHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PKCS12Key_ToEncryptionKey(PKCS12KeyHandle handle, EncryptionKeyHandle* result);

	/**
	* \brief Reinterpret current object as SigningKeyHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PKCS12Key_ToSigningKey(PKCS12KeyHandle handle, SigningKeyHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PKCS12Key_Release(PKCS12KeyHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PKCS12_KEY_H */
