#include "precompiled.h"
#include "utils/log.h"

#include "gotchangpdf/c_logging.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_IsEnabled(boolean_type* result)
{
	*result = LogManager::GetLoggerInstance()->IsEnabled();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_Enable(void)
{
	LogManager::GetLoggerInstance()->SetEnabled(true);
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_Disable(void)
{
	LogManager::GetLoggerInstance()->SetEnabled(false);
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_GetSeverity(LoggingSeverity* level)
{
	auto result = LogManager::GetLoggerInstance()->GetSeverity();
	switch (result)
	{
	case gotchangpdf::Severity::Debug:
		*level = LoggingSeverity_Debug; break;
	case gotchangpdf::Severity::Info:
		*level = LoggingSeverity_Info; break;
	case gotchangpdf::Severity::Warning:
		*level = LoggingSeverity_Warning; break;
	case gotchangpdf::Severity::Error:
		*level = LoggingSeverity_Error; break;
	case gotchangpdf::Severity::Fatal:
		*level = LoggingSeverity_Fatal; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Logging_SetSeverity(LoggingSeverity level)
{
	Severity converted;
	switch (level)
	{
	case LoggingSeverity_Debug:
		converted = gotchangpdf::Severity::Debug; break;
	case LoggingSeverity_Info:
		converted = gotchangpdf::Severity::Info; break;
	case LoggingSeverity_Warning:
		converted = gotchangpdf::Severity::Warning; break;
	case LoggingSeverity_Error:
		converted = gotchangpdf::Severity::Error; break;
	case LoggingSeverity_Fatal:
		converted = gotchangpdf::Severity::Fatal; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	LogManager::GetLoggerInstance()->SetSeverity(converted);
	return GOTCHANG_PDF_ERROR_SUCCESS;
}
