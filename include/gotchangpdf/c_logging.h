#ifndef _C_LOGGING_H
#define _C_LOGGING_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_logging.h
	* All functions that control library logging configuration
	*/

	/**
	* \brief Available severity settings
	*/
	typedef enum
	{
		/**
		* \brief Most verbose setting includes all available informations
		*/
		LoggingSeverity_Debug = 0,

		/**
		* \brief Include detailed, but much less verbose output
		*/
		LoggingSeverity_Info,

		/**
		* \brief Include only potentially dangerous informations
		*/
		LoggingSeverity_Warning,

		/**
		* \brief Include only program and input errors
		*/
		LoggingSeverity_Error,

		/**
		* \brief Include only fatal problems, that disallow program to
		* continue operating and will be shut down
		*/
		LoggingSeverity_Fatal
	} LoggingSeverity;

	/**
	* \brief Pointer to LoggingSeverity
	*/
	typedef LoggingSeverity *PLoggingSeverity;

	/**
	* \class Logging
	* \ingroup GotchangPDF
	* \brief Library logging facility
	* @{
	*/

	/**
	* \brief Determine if logging is enabled in current library instance
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_GetEnabled(out_boolean_type result);

	/**
	* \brief Enable logging for current library instance
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_SetEnabled(void);

	/**
	* \brief Disable logging for current library instance
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_SetDisabled(void);

	/**
	* \brief Get actual logging severity
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_GetSeverity(PLoggingSeverity level);

	/**
	* \brief Set new logging severity
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_SetSeverity(LoggingSeverity level);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_LOGGING_H */
