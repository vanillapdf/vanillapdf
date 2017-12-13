#ifndef _C_ENCRYPTION_H
#define _C_ENCRYPTION_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_encryption.h
	* \brief This file contains file encryption utilities
	*/

	/**
	* \class EncryptionKeyHandle
	* \extends IUnknownHandle
	* \ingroup Utils
	* \brief Represents encryption key other than password
	*/

	/**
	* \memberof EncryptionKeyHandle
	* @{
	*/

	/**
	* \brief Function pointer used to initialize dependencies in order to provide decryption
	*
	* This function currently cannot fail. In order to indicate failure on initialization
	* return error code in the decrypt function itself.
	* \see \ref EncryptionKeyDecryptFunction
	*/
	typedef void (*EncryptionKeyInitializeFunction)(void);

	/**
	* \brief Cleanup all dependencies after the decryption process has finished
	*
	* This function is called when the last reference to the
	* EncryptionKeyHandle has been disposed.
	*/
	typedef void (*EncryptionKeyCleanupFunction)(void);

	/**
	* \brief User provided function used for decrypting data
	*
	* This function is only called when \ref EncryptionKeyContainsFunction sets the result to true.
	* \param data a pointer to source buffer containing the PKCS#7 encrypted data
	* \param result an output pointer which has to be set with the decrypted data on success. Otherwise leave unchanged.
	*/
	typedef error_type (*EncryptionKeyDecryptFunction)(const BufferHandle* data, BufferHandle** result);

	/**
	* \brief Function used to determine whether the private key for certificate issued by \p issuer with \p serial is present
	* \param issuer a pointer to X509 NAME structure containning the certificate issuer
	* \param issuer a pointer to ASN.1 structure containning the certificate serial
	* \param result a pointer to boolean. Set to true, if the private key for \p issuer and \p serial is present. Otherwise set to false.
	*/
	typedef error_type (*EncryptionKeyContainsFunction)(const BufferHandle* issuer, const BufferHandle* serial, boolean_type* result);

	/**
	* \brief Creates a custom EncryptionKeyHandle to provide custom decrypt operation
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION EncryptionKey_CreateCustom(
		EncryptionKeyInitializeFunction initialize,
		EncryptionKeyCleanupFunction cleanup,
		EncryptionKeyDecryptFunction decrypt,
		EncryptionKeyContainsFunction contains,
		EncryptionKeyHandle** result
	);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION EncryptionKey_Release(EncryptionKeyHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_ENCRYPTION_H */
