#ifndef _C_LOGGING_H
#define _C_LOGGING_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_logging.h
	* \brief All functions that control library logging configuration
	*/

	/**
	* \class Logging
	* \ingroup Utils
	* \brief
	* The library can provide some additional debug and error messages.
	*
	* This class controls the logging switch
	* which enables and disables the logging entirely.
	* Additionally, the verbosity of the logging output
	* can be adjusted as well.
	*
	* The default behavior is that it creates a
	* log file for each file, where you can find some additional informations
	* based on the log level.
	*
	* This feature is __disabled__ by default.
	*/

	/**
	* \brief Available severity settings
	*/
	typedef enum {
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
	* \memberof Logging
	* @{
	*/

	/**
	* \brief Determine if logging is enabled in current library instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_IsEnabled(boolean_type* result);

	/**
	* \brief Enable logging for current library instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_Enable(void);

	/**
	* \brief Disable logging for current library instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_Disable(void);

	/**
	* \brief Get actual logging severity
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_GetSeverity(LoggingSeverity* level);

	/**
	* \brief Set new logging severity
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetSeverity(LoggingSeverity level);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_LOGGING_H */
