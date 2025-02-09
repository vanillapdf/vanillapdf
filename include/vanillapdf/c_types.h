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
* \brief This is return value type of all API functions.
* \ingroup group_types
*/
typedef uint32_t error_type;

/**
* \brief Boolean type supported in C
* \ingroup group_types
*/
typedef int8_t boolean_type;

/**
* \addtogroup group_types
* @{
*/

/**
* \brief Floating point values
*/
typedef double real_type;

/**
* \brief 16-bit unsigned integer
*/
typedef uint16_t ushort_type;

/**
* \brief 32-bit signed integer
*/
typedef int32_t integer_type;

/**
* \brief Offset type compatible with standard IO
*/
typedef int64_t offset_type;

/**
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
* \brief 64-bit signed integer type
*/
typedef int64_t bigint_type;

/**
* \brief 64-bit unsigned integer type
*/
typedef uint64_t biguint_type;

/**
* \brief C-Style string
*/
typedef const char* string_type;

/**
* \brief Writeable byte array
*/
typedef char* byte_array_type;

/** @} */ /* group_types */

#endif /* _C_TYPES_H */
