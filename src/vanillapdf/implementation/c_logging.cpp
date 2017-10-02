#include "precompiled.h"
#include "utils/log.h"

#include "vanillapdf/c_logging.h"
#include "c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION Logging_IsEnabled(boolean_type* result)
{
	*result = LogManager::GetLoggerInstance()->IsEnabled();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_Enable(void)
{
	LogManager::GetLoggerInstance()->SetEnabled(true);
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_Disable(void)
{
	LogManager::GetLoggerInstance()->SetEnabled(false);
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_GetSeverity(LoggingSeverity* level)
{
	auto result = LogManager::GetLoggerInstance()->GetSeverity();
	switch (result)
	{
	case vanillapdf::Severity::Debug:
		*level = LoggingSeverity_Debug; break;
	case vanillapdf::Severity::Info:
		*level = LoggingSeverity_Info; break;
	case vanillapdf::Severity::Warning:
		*level = LoggingSeverity_Warning; break;
	case vanillapdf::Severity::Error:
		*level = LoggingSeverity_Error; break;
	case vanillapdf::Severity::Fatal:
		*level = LoggingSeverity_Fatal; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetSeverity(LoggingSeverity level)
{
	Severity converted;
	switch (level)
	{
	case LoggingSeverity_Debug:
		converted = vanillapdf::Severity::Debug; break;
	case LoggingSeverity_Info:
		converted = vanillapdf::Severity::Info; break;
	case LoggingSeverity_Warning:
		converted = vanillapdf::Severity::Warning; break;
	case LoggingSeverity_Error:
		converted = vanillapdf::Severity::Error; break;
	case LoggingSeverity_Fatal:
		converted = vanillapdf::Severity::Fatal; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	LogManager::GetLoggerInstance()->SetSeverity(converted);
	return VANILLAPDF_ERROR_SUCCESS;
}
