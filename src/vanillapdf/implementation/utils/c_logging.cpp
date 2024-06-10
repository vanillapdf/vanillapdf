#include "precompiled.h"
#include "utils/log.h"

#include "vanillapdf/utils/c_logging.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION Logging_IsEnabled(boolean_type* result)
{
	try
	{
		//*result = LogManager::GetLoggerInstance()->IsEnabled();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_Enable(void)
{
	try
	{
		//LogManager::GetLoggerInstance()->SetEnabled(true);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_Disable(void)
{
	try
	{
		//LogManager::GetLoggerInstance()->SetEnabled(false);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_GetSeverity(LoggingSeverity* level)
{
	try
	{
		auto result = spdlog::get_level();
		switch (result)
		{
		case spdlog::level::debug:
			*level = LoggingSeverity_Debug; break;
		case spdlog::level::info:
			*level = LoggingSeverity_Info; break;
		case spdlog::level::warn:
			*level = LoggingSeverity_Warning; break;
		case spdlog::level::err:
			*level = LoggingSeverity_Error; break;
		case spdlog::level::critical:
			*level = LoggingSeverity_Fatal; break;
		default:
			return VANILLAPDF_ERROR_GENERAL;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Logging_SetSeverity(LoggingSeverity level)
{
	try
	{
		switch (level)
		{
		case LoggingSeverity_Debug:
			spdlog::set_level(spdlog::level::debug); break;
		case LoggingSeverity_Info:
			spdlog::set_level(spdlog::level::info); break;
		case LoggingSeverity_Warning:
			spdlog::set_level(spdlog::level::warn); break;
		case LoggingSeverity_Error:
			spdlog::set_level(spdlog::level::err); break;
		case LoggingSeverity_Fatal:
			spdlog::set_level(spdlog::level::critical); break;
		default:
			return VANILLAPDF_ERROR_GENERAL;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}
