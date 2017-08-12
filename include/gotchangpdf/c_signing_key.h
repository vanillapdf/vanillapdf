#ifndef _C_SIGNING_KEY_H
#define _C_SIGNING_KEY_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

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
	* \ingroup Files
	* \brief TODO
	*/

	/**
	* \memberof SigningKeyHandle
	* @{
	*/

	typedef error_type (*SigningKeyInitializeFunction)(MessageDigestAlgorithmType algorithm);
	typedef error_type (*SigningKeyUpdateFunction)(const struct BufferHandleTag* data);
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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION SigningKey_CreateCustom(
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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION SigningKey_Release(SigningKeyHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_SIGNING_KEY_H */
