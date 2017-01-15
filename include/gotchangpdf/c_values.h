#ifndef _VALUES_H
#define _VALUES_H

#include "c_export.h"

#include <stdint.h>
#include <stddef.h>

/**
* \file c_values.h
* This file contains types and constants used as parameters or return values.
*/

/**
* \ingroup ErrorTypes
* \brief
* This is return value type of all API functions.
*/
typedef int32_t error_type;

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
* \brief 32-bit unsigned integer
*/
typedef uint32_t uinteger_type;

/**
* \ingroup Types
* \brief Offset type compatible with standard IO
*/
typedef int64_t offset_type;

/**
* \ingroup Types
* \brief Size type defined in standard library
*/
typedef size_t size_type;

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

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \addtogroup BooleanTypes
	* @{
	*/

	/**
	* \brief
	* Represents the boolean __false__ value.
	*
	* It shall respond to false value as defined in c++ standard.
	* However, I __recommend__ using this constant instead of not operator
	* or any other constant, that may seem to work.
	*/
	GOTCHANG_PDF_API extern const boolean_type GOTCHANG_PDF_RV_FALSE;

	/**
	* \brief
	* Represents the boolean __true__ value.
	*
	* It shall respond to true value as defined in c++ standard.
	*/
	GOTCHANG_PDF_API extern const boolean_type GOTCHANG_PDF_RV_TRUE;

	/** @} */

	/**
	* \addtogroup ErrorTypes
	* @{
	*/

	// global error states

	/**
	* \brief
	* Indicates that the operation completed successfully.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_SUCCES;

	/**
	* \brief
	* An invalid parameter value to function call was passed.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

	/**
	* \brief
	* Operation is currently not supported.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_NOT_SUPPORTED;

	/**
	* \brief
	* An unknown error has occurred.
	*
	* Try to turn on ::Logging for extended informations.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_GENERAL;

	// syntax errors

	/**
	* \brief
	* An invalid object type was passed to function.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_CONVERSION;

	/**
	* \brief
	* The source file was already disposed.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_FILE_DISPOSED;

	/**
	* \brief
	* The source file was not yet initialized.
	*
	* \see File_Initialize
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED;

	/**
	* \brief
	* A dependent object was not found.
	*
	* In order to complete a task, library needed to find and object,
	* which was unavailable. I believe, this should not happen.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_OBJECT_MISSING;

	/**
	* \brief
	* Invalid protection password or key.
	*
	* The source file was encrypted, while the entered password did not match.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_INVALID_PASSWORD;

	// semantic errors

	/**
	* \brief
	* In case an entry is marked as optional,
	* this is the return value to indicate that it is missing.
	*/
	GOTCHANG_PDF_API extern const error_type GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _VALUES_H */
