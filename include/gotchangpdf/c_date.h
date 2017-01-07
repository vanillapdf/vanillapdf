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

	typedef enum
	{
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
	* \brief Pointer to DateTimezoneType
	*/
	typedef DateTimezoneType *PDateTimezoneType;

	/**
	* \memberof DateHandle
	* @{
	*/

	/**
	* \brief Get year represented by this object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetYear(DateHandle handle, out_integer_type result);

	/**
	* \brief Get month in specified year represented by this object. Range of values (1-12)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMonth(DateHandle handle, out_integer_type result);

	/**
	* \brief Get day index in month represented by this object. Range of values (1-31)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetDay(DateHandle handle, out_integer_type result);

	/**
	* \brief Get hours in day represented by this object. Range of values (0-23)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHour(DateHandle handle, out_integer_type result);

	/**
	* \brief Get minutes represented by this object. Range of values (0-59)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinute(DateHandle handle, out_integer_type result);

	/**
	* \brief Get seconds represented by this object. Range of values (0-59)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetSecond(DateHandle handle, out_integer_type result);

	/**
	* \brief
	* Get timezone represented by this object.
	* Relationship of local time to Universal Time (UT).
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetTimezone(DateHandle handle, PDateTimezoneType result);

	/**
	* \brief Get hour offset represented by this object. Range of values (0-23)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHourOffset(DateHandle handle, out_integer_type result);

	/**
	* \brief Get minute offset represented by this object. Range of values (0-59)
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinuteOffset(DateHandle handle, out_integer_type result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_Release(DateHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DATE_H */
