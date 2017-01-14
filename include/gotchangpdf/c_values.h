#ifndef _VALUES_H
#define _VALUES_H

#include "c_export.h"

#include <stdint.h>
#include <stddef.h>

/**
* \file c_values.h
* This file contains types and constants used as parameters or return values.
*/

/** \brief
* This is return value type of all API functions.
* GOTCHANG_PDF_ERROR_SUCCES indicates success.
* Any other value indicates failure.
*/
typedef int32_t error_type;

/**
* \brief
* Boolean type supported in C
*/
typedef int8_t boolean_type;

/**
* \brief
* Floating point values
*/
typedef double real_type;

/**
* \brief
* 16-bit signed integer
*/
typedef int16_t short_type;

/**
* \brief
* 16-bit unsigned integer
*/
typedef uint16_t ushort_type;

/**
* \brief
* 32-bit signed integer
*/
typedef int32_t integer_type;

/**
* \brief
* 32-bit unsigned integer
*/
typedef uint32_t uinteger_type;

/**
* \brief
* Offset type compatible with standard IO
*/
typedef int64_t offset_type;

/**
* \brief
* Size type defined in standard library
*/
typedef size_t size_type;

/**
* \brief
* 64-bit signed integer type
*/
typedef int64_t bigint_type;

/**
* \brief
* 64-bit unsigned integer type
*/
typedef uint64_t biguint_type;

/**
* \brief
* C-Style string
*/
typedef const char* string_type;

#ifdef __cplusplus
extern "C"
{
#endif

	// boolean values
	GOTCHANG_PDF_API extern const boolean_type GOTCHANG_PDF_RV_FALSE;
	GOTCHANG_PDF_API extern const boolean_type GOTCHANG_PDF_RV_TRUE;

	// global error states
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_SUCCES;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_PARAMETER_VALUE;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_NOT_SUPPORTED;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_GENERAL;

	// syntax errors
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_CONVERSION;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_FILE_DISPOSED;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_OBJECT_MISSING;
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_INVALID_PASSWORD;

	// semantic errors
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;

#ifdef __cplusplus
};
#endif

#endif /* _VALUES_H */
