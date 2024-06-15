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
	* \ingroup group_utils
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
		LoggingSeverity_Undefined = 0,

		/**
		* \brief Most verbose setting includes all available informations
		*/
		LoggingSeverity_Trace,

		/**
		* \brief
		* Very detailed level with most of the necessary information required during debugging.
		*/
		LoggingSeverity_Debug,

		/**
		* \brief Include detailed, but much less verbose output
		*/
		LoggingSeverity_Info,

		/**
		* \brief Include only potentially dangerous information
		*/
		LoggingSeverity_Warning,

		/**
		* \brief Include only program and input errors
		*/
		LoggingSeverity_Error,

		/**
		* \brief
		* Include only fatal problems, that disallow program to
		* continue operating and will be shut down.
		*/
		LoggingSeverity_Critical,

		/**
		* \brief Logging is turned off entirely.
		*/
		LoggingSeverity_Off
	} LoggingSeverity;

	/**
	* \memberof Logging
	* @{
	*/

	/**
	* \brief This function is invoked for every log entry when custom callback logger is used
	*/
	typedef void(CALLING_CONVENTION *Sink_Log_Function)(void* user_data, LoggingSeverity level, string_type payload, size_type length);

	/**
	* \brief This function is invoked when the data are about to be flushed to the persistent storage
	*/
	typedef void(CALLING_CONVENTION *Sink_Flush_Function)(void* user_data);

	/**
	* \brief Set a logger, that will invoke sink_callback and sink_flush_callback passing user_data
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetCallbackLogger(
		Sink_Log_Function sink_callback,
		Sink_Flush_Function sink_flush_callback,
		void* user_data);

	/**
	* \brief Set a logger using rotating file sink based on size
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetRotatingFileLogger(
		string_type filename,
		integer_type max_file_size,
		integer_type max_files);

	/**
	* \brief Set logging pattern to be used for each logging entry
	*
	* # Pattern flags
	* 
	* Pattern flags are in the form of %flag and resembles the strftime function:
	*
	* | Flag  | Meaning                                          | Example                                       |
	* |-------|--------------------------------------------------|-----------------------------------------------|
	* | %v    | The actual text to log                           | "some user text"                              |
	* | %t    | Thread id                                        | "1232"                                        |
	* | %P    | Process id                                       | "3456"                                        |
	* | %n    | Logger's name                                    | "some logger name"                            |
	* | %l    | The log level of the message                     | "debug", "info", etc                          |
	* | %L    | Short log level of the message                   | "D", "I", etc                                 |
	* | %a    | Abbreviated weekday name                         | "Thu"                                         |
	* | %A    | Full weekday name                                | "Thursday"                                    |
	* | %b    | Abbreviated month name                           | "Aug"                                         |
	* | %B    | Full month name                                  | "August"                                      |
	* | %c    | Date and time representation                     | "Thu Aug 23 15:35:46 2014"                    |
	* | %C    | Year in 2 digits                                 | "14"                                          |
	* | %Y    | Year in 4 digits                                 | "2014"                                        |
	* | %D or %x | Short MM/DD/YY date                          | "08/23/14"                                    |
	* | %m    | Month 01-12                                      | "11"                                          |
	* | %d    | Day of month 01-31                               | "29"                                          |
	* | %H    | Hours in 24 format 00-23                         | "23"                                          |
	* | %I    | Hours in 12 format 01-12                         | "11"                                          |
	* | %M    | Minutes 00-59                                    | "59"                                          |
	* | %S    | Seconds 00-59                                    | "58"                                          |
	* | %e    | Millisecond part of the current second 000-999   | "678"                                         |
	* | %f    | Microsecond part of the current second 000000-999999 | "056789"                                 |
	* | %F    | Nanosecond part of the current second 000000000-999999999 | "256789123"                           |
	* | %p    | AM/PM                                            | "AM"                                          |
	* | %r    | 12 hour clock                                    | "02:55:02 PM"                                 |
	* | %R    | 24-hour HH:MM time, equivalent to %H:%M          | "23:55"                                       |
	* | %T or %X | ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S | "23:55:59"                         |
	* | %z    | ISO 8601 offset from UTC in timezone ([+/-]HH:MM)| "+02:00"                                      |
	* | %E    | Seconds since the epoch                          | "1528834770"                                  |
	* | %%    | The % sign                                       | "%"                                           |
	* | %+    | spdlog's default format                          | "[2014-10-31 23:46:59.678] [mylogger] [info] Some message" |
	* | %^    | start color range (can be used only once)        | "[mylogger] [info(green)] Some message"       |
	* | %$    | end color range (for example %^[+++]%$ %v) (can be used only once) | "[+++] Some message"                   |
	* | %@    | Source file and line (use SPDLOG_TRACE(..), SPDLOG_INFO(...) etc. instead of spdlog::trace(...)) Same as %g:%# | /some/dir/my_file.cpp:123 |
	* | %s    | Basename of the source file (use SPDLOG_TRACE(..), SPDLOG_INFO(...) etc.) | "my_file.cpp"                          |
	* | %g    | Full or relative path of the source file as appears in spdlog::source_loc (use SPDLOG_TRACE(..), SPDLOG_INFO(...) etc.) | "/some/dir/my_file.cpp"       |
	* | %#    | Source line (use SPDLOG_TRACE(..), SPDLOG_INFO(...) etc.) | "123"                                  |
	* | %!    | Source function (use SPDLOG_TRACE(..), SPDLOG_INFO(...) etc. see tweakme for pretty-print) | "my_func"                        |
	* | %o    | Elapsed time in milliseconds since previous message | "456"                                  |
	* | %i    | Elapsed time in microseconds since previous message | "456"                                  |
	* | %u    | Elapsed time in nanoseconds since previous message | "11456"                                |
	* | %O    | Elapsed time in seconds since previous message   | "4"                                           |
	*
	* # Aligning
	*
	* Each pattern flag can be aligned by prepending a width number(up to 64).
	*
	* Use - (left align) or = (center align) to control the align side:
	*
	* | Align            | Meaning                               | Example | Result    |
	* |------------------|---------------------------------------|---------|-----------|
	* | %\<width\>\<flag\>   | Right align                           | %8l     | "    info"|
	* | %-\<width\>\<flag\>  | Left align                            | %-8l    | "info    "|
	* | %=\<width\>\<flag\>  | Center align                          | %=8l    | "  info  "|
	*
	* Optionally add ! to truncate the result if its size exceeds the specified width:
	*
	* | Align            | Meaning                               | Example | Result    |
	* |------------------|---------------------------------------|---------|-----------|
	* | %\<width\>!\<flag\>  | Right align or truncate               | %3!l    | "inf"     |
	* | %-\<width\>!\<flag\> | Left align or truncate                | %-2!l   | "in"      |
	* | %=\<width\>!\<flag\> | Center align or truncate              | %=1!l   | "i"       |
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetPattern(string_type pattern);


	/**
	* \brief Get actual logging severity
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_GetSeverity(LoggingSeverity* level);

	/**
	* \brief Set new logging severity
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetSeverity(LoggingSeverity level);

	/**
	* \brief Gracefully terminate the entire logging ecosystem
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Logging_Shutdown();

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_LOGGING_H */
