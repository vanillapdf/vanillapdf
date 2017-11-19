#include "precompiled.h"
#include "semantics/objects/date.h"

#include "vanillapdf/semantics/c_date.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetYear(DateHandle* handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetYear();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMonth(DateHandle* handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetMonth();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetDay(DateHandle* handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetDay();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetHour(DateHandle* handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetHour();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMinute(DateHandle* handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetMinute();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetSecond(DateHandle* handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetSecond();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetTimezone(DateHandle* handle, TimezoneType* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetTimezone())
	{
	case Timezone::UTC:
		*result = TimezoneType_UTC; break;
	case Timezone::Later:
		*result = TimezoneType_Later; break;
	case Timezone::Earlier:
		*result = TimezoneType_Earlier; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetHourOffset(DateHandle* handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetHourOffset();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMinuteOffset(DateHandle* handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetMinuteOffset();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_Release(DateHandle* handle)
{
	return ObjectRelease<Date, DateHandle*>(handle);
}