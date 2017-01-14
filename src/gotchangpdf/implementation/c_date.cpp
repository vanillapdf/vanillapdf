#include "precompiled.h"
#include "semantics/objects/date.h"

#include "gotchangpdf/c_date.h"
#include "c_helper.h"

using namespace gotchangpdf::syntax;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetYear(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Year();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMonth(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Month();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetDay(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Day();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHour(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Hour();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinute(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Minute();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetSecond(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Second();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetTimezone(DateHandle handle, DateTimezoneType* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->Timezone())
	{
	case Date::TimezoneType::UTC:
		*result = DateTimezoneType_UTC; break;
	case Date::TimezoneType::Later:
		*result = DateTimezoneType_Later; break;
	case Date::TimezoneType::Earlier:
		*result = DateTimezoneType_Earlier; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHourOffset(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->HourOffset();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinuteOffset(DateHandle handle, integer_type* result)
{
	Date* obj = reinterpret_cast<Date*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->MinuteOffset();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_Release(DateHandle handle)
{
	return ObjectRelease<Date, DateHandle>(handle);
}