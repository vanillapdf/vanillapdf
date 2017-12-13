#ifndef _C_TYPES_H
#define _C_TYPES_H

/**
* \file c_types.h
* \brief This file contains type definitions.
*/

#include "c_platform.h"

// C++ specific headers
#ifdef __cplusplus
	#include <cstdint>
	#include <cstddef>
#endif /* __cplusplus */

// C specific headers
#include <stdint.h>
#include <stddef.h>

/**
* \ingroup ErrorTypes
* \brief
* This is return value type of all API functions.
*/
typedef uint32_t error_type;

/**
* \ingroup BooleanTypes
* \brief
* Boolean type supported in C
*/
typedef int8_t boolean_type;

/**
* \ingroup Types
* \brief
* Floating point values
*/
typedef double real_type;

/**
* \ingroup Types
* \brief 16-bit unsigned integer
*/
typedef uint16_t ushort_type;

/**
* \ingroup Types
* \brief 32-bit signed integer
*/
typedef int32_t integer_type;

/**
* \ingroup Types
* \brief Offset type compatible with standard IO
*/
typedef int64_t offset_type;

/**
* \ingroup Types
* \brief Size type defined in standard library
*/
#if defined(ENVIRONMENT_32_BIT)
	typedef uint32_t size_type;
#elif defined(ENVIRONMENT_64_BIT)
	typedef uint64_t size_type;
#elif !defined(ENVIRONMENT_32_BIT) && !defined(ENVIRONMENT_64_BIT)
	#error Unknown environment size type
#endif /* ENVIRONMENT_32_BIT */

/**
* \ingroup Types
* \brief 64-bit signed integer type
*/
typedef int64_t bigint_type;

/**
* \ingroup Types
* \brief 64-bit unsigned integer type
*/
typedef uint64_t biguint_type;

/**
* \ingroup Types
* \brief C-Style string
*/
typedef const char* string_type;

#endif /* _C_TYPES_H */
