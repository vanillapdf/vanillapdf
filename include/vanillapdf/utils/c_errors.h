#ifndef _C_ERRORS_H
#define _C_ERRORS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_errors.h
	* \brief All functions that control last error occurred
	*/

	/**
	* \class Errors
	* \ingroup group_utils
	* \brief
	* This class contains additional informations about last error occurred
	*
	* The error code and message is stored in the thread-local storage and
	* does not clear until another error occurrs on the same thread.
	*/

	/**
	* \memberof Errors
	* @{
	*/

	/**
	* \brief Obtain the last error code that occurred
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastError(error_type* result);

	/**
	* \brief Get length of the name returned by \ref Errors_GetPrintableErrorText including the trailing NULL character
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetPrintableErrorTextLength(error_type value, size_type* size);

	/**
	* \brief Obtain a human readable text of the error code including the trailing NULL character
	*
	* This function returns a pointer to statically allocated data and it <b>shall not</b> be freed.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetPrintableErrorText(error_type value, char* data, size_type size);

	/**
	* \brief Obtain length of the additional error message including the trailing NULL character
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessageLength(size_type* result);

	/**
	* \brief Obtain the additional NULL terminated text message about the last error
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessage(char* data, size_type size);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_ERRORS_H */
