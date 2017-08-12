#ifndef _C_MESSAGE_DIGEST_ALGORITHM_H
#define _C_MESSAGE_DIGEST_ALGORITHM_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_message_digest_algorithm.h
	* This file contains all supported digest algorithms
	*/

	/**
	* \brief Supported digest algorithms used as hash functions
	*/
	typedef enum {
		/**
		* \brief Default unset value, triggers error when used
		*/
		MessageDigestAlgorithmType_None = 0,

		/**
		* \brief "null" message digest that does nothing: i.e. the hash it returns is of zero length.
		*/
		MessageDigestAlgorithmType_MDNULL,
		MessageDigestAlgorithmType_MD2,
		MessageDigestAlgorithmType_MD4,
		MessageDigestAlgorithmType_MD5,
		MessageDigestAlgorithmType_SHA,
		MessageDigestAlgorithmType_SHA1,
		MessageDigestAlgorithmType_DSS,
		MessageDigestAlgorithmType_DSS1,
		MessageDigestAlgorithmType_ECDSA,
		MessageDigestAlgorithmType_SHA224,
		MessageDigestAlgorithmType_SHA256,
		MessageDigestAlgorithmType_SHA384,
		MessageDigestAlgorithmType_SHA512,
		MessageDigestAlgorithmType_MDC2,
		MessageDigestAlgorithmType_RIPEMD160,
		MessageDigestAlgorithmType_WHIRLPOOL
	} MessageDigestAlgorithmType;

#ifdef __cplusplus
};
#endif

#endif /* _C_MESSAGE_DIGEST_ALGORITHM_H */
