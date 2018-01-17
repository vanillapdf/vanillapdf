#ifndef _C_VALUES_H
#define _C_VALUES_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_types.h"

/**
* \file c_values.h
* \brief This file contains types and constants used as parameters or return values.
*/

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \addtogroup group_boolean_types
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
	VANILLAPDF_API extern const boolean_type VANILLAPDF_RV_FALSE;

	/**
	* \brief
	* Represents the boolean __true__ value.
	*
	* It shall respond to true value as defined in c++ standard.
	*/
	VANILLAPDF_API extern const boolean_type VANILLAPDF_RV_TRUE;

	/** @} */ /* group_boolean_types */

	/**
	* \addtogroup group_error_types
	* @{
	*/

	// global error states

	/**
	* \brief
	* Indicates that the operation completed successfully.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_SUCCESS;

	/**
	* \brief
	* An invalid parameter value to function call was passed.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_PARAMETER_VALUE;

	/**
	* \brief
	* Operation is currently not supported.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_NOT_SUPPORTED;

	/**
	* \brief
	* Operation was cancelled by user.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_USER_CANCELLED;

	/**
	* \brief
	* Error in compressed data.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_ZLIB_DATA;

	/**
	* \brief
	* Presented license file is not valid.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_INVALID_LICENSE;

	/**
	* \brief
	* An unknown error has occurred.
	*
	* Try to turn on \ref Logging for extended informations.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_GENERAL;

	// syntax errors

	/**
	* \brief
	* An invalid object type was passed to function.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_CONVERSION;

	/**
	* \brief
	* The source file was already disposed.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_FILE_DISPOSED;

	/**
	* \brief
	* The source file was not yet initialized.
	*
	* \see \ref File_Initialize
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_FILE_NOT_INITIALIZED;

	/**
	* \brief
	* A dependent object was not found.
	*
	* In order to complete a task, library needed to find and object,
	* which was unavailable. I believe, this should not happen.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_OBJECT_MISSING;

	/**
	* \brief
	* Invalid protection password or key.
	*
	* The source file was encrypted, while the entered password did not match.
	*/
	VANILLAPDF_API extern const error_type VANILLAPDF_ERROR_INVALID_PASSWORD;

	// semantic errors

	/** @} */ /* group_error_types */

#ifdef __cplusplus
};
#endif

#endif /* _C_VALUES_H */
