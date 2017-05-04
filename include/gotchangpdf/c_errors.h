#ifndef _C_ERRORS_H
#define _C_ERRORS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_errors.h
	* All functions that control last error occurred
	*/

	/**
	* \class Errors
	* \ingroup Utils
	* \brief
	* This class contains additional informations about last error occurred
	*/

	/**
	* \memberof Errors
	* @{
	*/

	/**
	* \brief Obtain the last error code that occurred
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Errors_GetLastError(error_type* result);

	/**
	* \brief Obtain length of the additional error message
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessageLength(size_type* size);

	/**
	* \brief Obtain the additional text message about the last error
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessage(char* data, size_type size);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_ERRORS_H */
