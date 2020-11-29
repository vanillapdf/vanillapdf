#ifndef _C_DATE_H
#define _C_DATE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_date.h
	* \brief This file contains class definitions for \ref DateHandle
	*/

	/**
	* \class DateHandle
	* \extends IUnknownHandle
	* \ingroup group_common_data_structures
	* \brief Represents a reference to a particular day represented within a calendar system
	*/

	/**
	* \brief Relationship of local time to Universal Time (UT).
	*/
	typedef enum {

		/**
		* \brief Undefined unitialized default value, triggers error when used
		*/
		TimezoneType_Undefined = 0,

		/**
		* \brief Signifies that local time is equal to UT.
		*/
		TimezoneType_UTC,

		/**
		* \brief Signifies that local time is later than UT.
		*/
		TimezoneType_Later,

		/**
		* \brief Signifies that local time is earlier than UT.
		*/
		TimezoneType_Earlier
	} TimezoneType;

	/**
	* \memberof DateHandle
	* @{
	*/

	/**
	* \brief Create a new blank date instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_CreateEmpty(DateHandle** result);

	/**
	* \brief Get current date in UTC format
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_CreateCurrent(DateHandle** result);

	/**
	* \brief Get year represented by this object
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetYear(DateHandle* handle, integer_type* result);

	/**
	* \brief \copybrief Date_GetYear
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetYear(DateHandle* handle, integer_type value);

	/**
	* \brief Get month in specified year represented by this object. Range of values (1-12)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMonth(DateHandle* handle, integer_type* result);

	/**
	* \brief \copybrief Date_GetMonth
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetMonth(DateHandle* handle, integer_type value);

	/**
	* \brief Get day index in month represented by this object. Range of values (1-31)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetDay(DateHandle* handle, integer_type* result);

	/**
	* \brief \copybrief Date_GetDay
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetDay(DateHandle* handle, integer_type value);

	/**
	* \brief Get hours in day represented by this object. Range of values (0-23)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetHour(DateHandle* handle, integer_type* result);

	/**
	* \brief \copybrief Date_GetHour
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetHour(DateHandle* handle, integer_type value);

	/**
	* \brief Get minutes represented by this object. Range of values (0-59)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMinute(DateHandle* handle, integer_type* result);

	/**
	* \brief \copybrief Date_GetMinute
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetMinute(DateHandle* handle, integer_type value);

	/**
	* \brief Get seconds represented by this object. Range of values (0-59)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetSecond(DateHandle* handle, integer_type* result);

	/**
	* \brief \copybrief Date_GetSecond
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetSecond(DateHandle* handle, integer_type value);

	/**
	* \brief \copybrief TimezoneType
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetTimezone(DateHandle* handle, TimezoneType* result);

	/**
	* \brief \copybrief Date_GetTimezone
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetTimezone(DateHandle* handle, TimezoneType value);

	/**
	* \brief Get hour offset represented by this object. Range of values (0-23)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetHourOffset(DateHandle* handle, integer_type* result);

	/**
	* \brief \copybrief Date_GetHourOffset
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetHourOffset(DateHandle* handle, integer_type value);

	/**
	* \brief Get minute offset represented by this object. Range of values (0-59)
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMinuteOffset(DateHandle* handle, integer_type* result);

	/**
	* \brief \copybrief Date_GetMinuteOffset
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_SetMinuteOffset(DateHandle* handle, integer_type value);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_ToUnknown(DateHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref ObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_FromUnknown(IUnknownHandle* handle, DateHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Date_Release(DateHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DATE_H */
