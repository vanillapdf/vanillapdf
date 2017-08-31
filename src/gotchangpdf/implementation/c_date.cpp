#include "precompiled.h"
#include "semantics/objects/date.h"

#include "gotchangpdf/c_date.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetYear(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetYear();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMonth(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetMonth();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetDay(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetDay();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHour(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetHour();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinute(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetMinute();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetSecond(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetSecond();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetTimezone(DateHandle handle, TimezoneType* result)
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
		return GOTCHANG_PDF_ERROR_GENERAL;
	}
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHourOffset(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetHourOffset();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinuteOffset(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetMinuteOffset();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_Release(DateHandle handle)
{
	return ObjectRelease<Date, DateHandle>(handle);
}