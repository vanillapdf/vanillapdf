#ifndef _C_DATE_H
#define _C_DATE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_date.h
	* This file contains class definitions for DateHandle
	*/

	/**
	* \class DateHandle
	* \extends IUnknownHandle
	* \ingroup CommonDataStructures
	* \brief Represents a reference to a particular day represented within a calendar system
	*/

	/**
	* \brief Relationship of local time to Universal Time (UT).
	*/
	typedef enum {
		/**
		* \brief Signifies that local time is equal to UT.
		*/
		DateTimezoneType_UTC = 0,

		/**
		* \brief Signifies that local time is later than UT.
		*/
		DateTimezoneType_Later,

		/**
		* \brief Signifies that local time is earlier than UT.
		*/
		DateTimezoneType_Earlier
	} DateTimezoneType;

	/**
	* \memberof DateHandle
	* @{
	*/

	/**
	* \brief Get year represented by this object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetYear(DateHandle handle, integer_type* result);

	/**
	* \brief Get month in specified year represented by this object. Range of values (1-12)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMonth(DateHandle handle, integer_type* result);

	/**
	* \brief Get day index in month represented by this object. Range of values (1-31)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetDay(DateHandle handle, integer_type* result);

	/**
	* \brief Get hours in day represented by this object. Range of values (0-23)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHour(DateHandle handle, integer_type* result);

	/**
	* \brief Get minutes represented by this object. Range of values (0-59)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinute(DateHandle handle, integer_type* result);

	/**
	* \brief Get seconds represented by this object. Range of values (0-59)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetSecond(DateHandle handle, integer_type* result);

	/**
	* \brief \copybrief DateTimezoneType
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetTimezone(DateHandle handle, DateTimezoneType* result);

	/**
	* \brief Get hour offset represented by this object. Range of values (0-23)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHourOffset(DateHandle handle, integer_type* result);

	/**
	* \brief Get minute offset represented by this object. Range of values (0-59)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinuteOffset(DateHandle handle, integer_type* result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_Release(DateHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DATE_H */
