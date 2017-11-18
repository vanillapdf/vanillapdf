#ifndef _C_SIGNING_KEY_H
#define _C_SIGNING_KEY_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#include "c_message_digest_algorithm.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_signing_key.h
	* This file contains class definitions for SigningKeyHandle
	*/

	/**
	* \class SigningKeyHandle
	* \extends IUnknownHandle
	* \ingroup Utils
	* \brief Used for document signing
	*
	* Standard routine for message signing is Initialize, (single or multiple) Update, Final.
	*/

	/**
	* \memberof SigningKeyHandle
	* @{
	*/

	/**
	* \brief Initialize signing engine using selected digest algorithm
	*
	* This function should reset all previous settings and prepare
	* for future ::SigningKeyUpdateFunction to be called.
	*/
	typedef error_type (*SigningKeyInitializeFunction)(MessageDigestAlgorithmType algorithm);

	/**
	* \brief Insert data into engine for digest calculation
	*
	* This function can be called multiple times,
	* if there are more data for signing that would fit in a single buffer.
	*/
	typedef error_type (*SigningKeyUpdateFunction)(const struct BufferHandleTag* data);

	/**
	* \brief Finish the digest calculation and return signed hash
	*
	* This function should allocate buffer, filled with data.
	* The data should be hash value according to \p algorithm in ::SigningKeyInitializeFunction.
	*
	* Hash should be signed and the result should be either a DER-encoded PKCS#1 binary data object or a DER-encoded PKCS#7 binary data object.
	* \see ::DigitalSignatureHandle
	*/
	typedef error_type (*SigningKeyFinalFunction)(BufferHandle* result);

	/**
	* \brief Cleanup all dependencies after the signing process has finished
	*
	* This function is called when the last reference to the
	* SigningKeyHandle has been disposed.
	*/
	typedef void (*SigningKeyCleanupFunction)(void);

	/**
	* \brief Creates a custom SigningKeyHandle to provide custom sign operation
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SigningKey_CreateCustom(
		SigningKeyInitializeFunction sign_init,
		SigningKeyUpdateFunction sign_update,
		SigningKeyFinalFunction sign_final,
		SigningKeyCleanupFunction sign_cleanup,
		SigningKeyHandle* result
	);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SigningKey_Release(SigningKeyHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_SIGNING_KEY_H */
