#ifndef _C_SIGNING_KEY_H
#define _C_SIGNING_KEY_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#include "vanillapdf/utils/c_message_digest_algorithm.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_signing_key.h
	* \brief This file contains class definitions for \ref SigningKeyHandle
	*/

	/**
	* \class SigningKeyHandle
	* \extends IUnknownHandle
	* \ingroup group_utils
	* \brief Used for document signing
	*
	* Standard routine for message signing is Initialize, (single or multiple) Update, Final.
	* \see \ref DocumentSignatureSettingsHandle
	*/

	/**
	* \memberof SigningKeyHandle
	* @{
	*/

	/**
	* \brief Initialize signing engine using selected digest algorithm
	*
	* This function should reset all previous settings and prepare
	* for future \ref SigningKey_Update_Function to be called.
	*/
	typedef error_type (*SigningKey_Initialize_Function)(void* user_data, MessageDigestAlgorithmType algorithm);

	/**
	* \brief Insert data into engine for digest calculation
	*
	* This function can be called multiple times,
	* if there are more data for signing that would fit in a single buffer.
	*/
	typedef error_type (*SigningKey_Update_Function)(void* user_data, const BufferHandle* data);

	/**
	* \brief Finish the digest calculation and return signed hash
	*
	* This function should allocate buffer, filled with data.
	* The data should be hash value according to \p algorithm in \ref SigningKey_Initialize_Function.
	*
	* Hash should be signed and the result should be either a DER-encoded PKCS#1 binary data object or a DER-encoded PKCS#7 binary data object.
	*/
	typedef error_type (*SigningKey_Final_Function)(void* user_data, BufferHandle** result);

	/**
	* \brief Cleanup all dependencies after the signing process has finished
	*
	* This function is called when the last reference to the
	* \ref SigningKeyHandle has been disposed.
	*/
	typedef void (*SigningKey_Cleanup_Function)(void* user_data);

	/**
	* \brief Creates a custom \ref SigningKeyHandle to provide custom sign operation
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SigningKey_CreateCustom(
		SigningKey_Initialize_Function sign_init,
		SigningKey_Update_Function sign_update,
		SigningKey_Final_Function sign_final,
		SigningKey_Cleanup_Function sign_cleanup,
		void* user_data,
		SigningKeyHandle** result
	);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SigningKey_Release(SigningKeyHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_SIGNING_KEY_H */
