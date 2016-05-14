#ifndef _C_DATE_H
#define _C_DATE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		DateTimezoneType_UTC = 'Z',
		DateTimezoneType_Later = '+',
		DateTimezoneType_Earlier = '-'
	} DateTimezoneType, *PDateTimezoneType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetYear(DateHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMonth(DateHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetDay(DateHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHour(DateHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinute(DateHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetSecond(DateHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetTimezone(DateHandle handle, PDateTimezoneType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetHourOffset(DateHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_GetMinuteOffset(DateHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Date_Release(DateHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DATE_H */
