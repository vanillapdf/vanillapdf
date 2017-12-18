#ifndef _C_PKCS12_KEY_H
#define _C_PKCS12_KEY_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_pkcs12_key.h
	* \brief This file contains class definitions for \ref PKCS12KeyHandle
	*/

	/**
	* \class PKCS12KeyHandle
	* \implements EncryptionKeyHandle
	* \implements SigningKeyHandle
	* \ingroup group_utils
	* \brief PKCS#12 container is often used in cryptography to store asymmetric key pair
	*
	* It can used for encryption/decryption and signing as well.
	*/

	/**
	* \memberof PKCS12KeyHandle
	* @{
	*/

	/**
	* \brief Uses PKCS#12 standard format to access asymmetric keys
	* \param path a string path to file containing the PKCS#12 data structure
	* \param password a protection password to the data structure. If there is no password use NULL.
	* \param result a pointer to library allocated PKCS12KeyHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_CreateFromFile(string_type path, string_type password, PKCS12KeyHandle** result);

	/**
	* \brief Uses PKCS#12 standard format to access asymmetric keys
	* \param data a buffer containing PKCS#12 data structure
	* \param password a protection password to the data structure. If there is no password use NULL.
	* \param result a pointer to library allocated PKCS12KeyHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_CreateFromBuffer(BufferHandle* data, string_type password, PKCS12KeyHandle** result);

	/**
	* \brief Reinterpret current object as \ref EncryptionKeyHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_ToEncryptionKey(PKCS12KeyHandle* handle, EncryptionKeyHandle** result);

	/**
	* \brief Reinterpret current object as \ref SigningKeyHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_ToSigningKey(PKCS12KeyHandle* handle, SigningKeyHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PKCS12Key_Release(PKCS12KeyHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PKCS12_KEY_H */
