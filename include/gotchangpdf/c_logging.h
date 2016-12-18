#ifndef _C_LOGGING_H
#define _C_LOGGING_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		LoggingSeverity_Debug = 0,
		LoggingSeverity_Info,
		LoggingSeverity_Warning,
		LoggingSeverity_Error,
		LoggingSeverity_Fatal
	} LoggingSeverity, *PLoggingSeverity;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_GetEnabled(out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_SetEnabled(void);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_SetDisabled(void);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_GetSeverity(PLoggingSeverity level);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_SetSeverity(LoggingSeverity level);

#ifdef __cplusplus
};
#endif

#endif /* _C_LOGGING_H */
