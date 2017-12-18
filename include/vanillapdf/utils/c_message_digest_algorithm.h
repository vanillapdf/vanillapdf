#ifndef _C_MESSAGE_DIGEST_ALGORITHM_H
#define _C_MESSAGE_DIGEST_ALGORITHM_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_message_digest_algorithm.h
	* \brief This file contains all supported digest algorithms
	*/

	/**
	* \brief Supported digest algorithms used as hash functions
	* \ingroup group_utils
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

		/**
		* \brief The MD2 Message - Digest Algorithm is a cryptographic hash function developed by Ronald Rivest in 1989.
		*/
		MessageDigestAlgorithmType_MD2,

		/**
		* \brief The MD4 Message - Digest Algorithm is a cryptographic hash function developed by Ronald Rivest in 1990.
		*/
		MessageDigestAlgorithmType_MD4,

		/**
		* \brief MD5 is one in a series of message digest algorithms designed by Professor Ronald Rivest of MIT (Rivest, 1992).
		*/
		MessageDigestAlgorithmType_MD5,

		/**
		* \brief A 160-bit hash function which resembles the earlier MD5 algorithm.
		*/
		MessageDigestAlgorithmType_SHA1,

		/**
		* \brief SHA-2 (Secure Hash Algorithm 2) is a set of cryptographic hash functions designed by the United States National Security Agency (NSA).
		*/
		MessageDigestAlgorithmType_SHA224,

		/**
		* \brief \copybrief MessageDigestAlgorithmType_SHA224
		*/
		MessageDigestAlgorithmType_SHA256,

		/**
		* \brief \copybrief MessageDigestAlgorithmType_SHA224
		*/
		MessageDigestAlgorithmType_SHA384,

		/**
		* \brief \copybrief MessageDigestAlgorithmType_SHA224
		*/
		MessageDigestAlgorithmType_SHA512,

		/**
		* \brief MDC - 2 (Modification Detection Code 2, sometimes called Meyer - Schilling) is a cryptographic hash function.
		*/
		MessageDigestAlgorithmType_MDC2,

		/**
		* \brief
		* RIPEMD (RACE Integrity Primitives Evaluation Message Digest) is an improved,
		* 160-bit version of the original RIPEMD, and the most common version in the family.
		*/
		MessageDigestAlgorithmType_RIPEMD160,

		/**
		* \brief WHIRLPOOL is a cryptographic hash function designed by Vincent Rijmen.
		*/
		MessageDigestAlgorithmType_WHIRLPOOL
	} MessageDigestAlgorithmType;

#ifdef __cplusplus
};
#endif

#endif /* _C_MESSAGE_DIGEST_ALGORITHM_H */
