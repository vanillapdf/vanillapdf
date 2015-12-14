#ifndef _C_HELPER_H
#define _C_HELPER_H

#include "exception.h"
#include "semantic_exceptions.h"
#include "log.h"

#include "c_values.h"

#define RETURN_ERROR_IF_NULL(var, error_code) do { if (nullptr == var) return error_code; } while(0)
#define RETURN_ERROR_PARAM_VALUE_IF_NULL(var) RETURN_ERROR_IF_NULL(var, GOTCHANG_PDF_ERROR_PARAMETER_VALUE)

#define CATCH_SCOPE_EXCEPTIONS \
	catch (...) \
	{ \
		LOG_ERROR << "Caught unknown exception"; \
		return GOTCHANG_PDF_ERROR_GENERAL; \
	}

#define CATCH_GOTCHNGPDF_EXCEPTIONS \
	catch (gotchangpdf::semantics::OptionalEntryMissingException& e) \
	{ \
		LOG_DEBUG << e.what(); \
		return static_cast<error_type>(e.code()); \
	} \
	catch (gotchangpdf::ExceptionBase& e) \
	{ \
		LOG_ERROR << e.what(); \
		return static_cast<error_type>(e.code()); \
	} \
	catch (std::exception& e) \
	{ \
		LOG_ERROR << e.what(); \
		return GOTCHANG_PDF_ERROR_GENERAL; \
	} \
	catch (...) \
	{ \
		LOG_ERROR << "Caught unknown exception"; \
		return GOTCHANG_PDF_ERROR_GENERAL; \
	}

#define LOG_WEAK_FILE_SCOPE(file) \
	auto __locked_file__ = file.lock(); \
	if (!__locked_file__) \
		return GOTCHANG_PDF_ERROR_FILE_DISPOSED; \
	\
	LOG_SCOPE(__locked_file__->GetFilename());

#define LOG_OBJECT_SCOPE(obj) \
	auto __weak_file__ = obj->GetFile(); \
	LOG_WEAK_FILE_SCOPE(__weak_file__)

#define LOG_HIGH_OBJECT_SCOPE(obj) \
	auto __low_obj__ = obj->GetObject(); \
	LOG_OBJECT_SCOPE(__low_obj__);

template <typename SourceT, typename DestT, typename SourceHandleT, typename DestHandleT>
error_type SafeObjectConvert(SourceHandleT from, DestHandleT* result) noexcept
{
	SourceT* obj = reinterpret_cast<SourceT*>(from);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_OBJECT_SCOPE(obj);

		try
		{
			DestT* converted = dynamic_cast<DestT*>(obj);
			if (nullptr == converted)
				return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

			*result = reinterpret_cast<DestHandleT>(converted);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

template <typename SourceT, typename DestT, typename SourceHandleT, typename DestHandleT>
error_type SafeObjectConvertNoScope(SourceHandleT from, DestHandleT* result) noexcept
{
	SourceT* obj = reinterpret_cast<SourceT*>(from);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		DestT* converted = dynamic_cast<DestT*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<DestHandleT>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

template <typename T, typename HandleT>
error_type ObjectRelease(HandleT handle) noexcept
{
	T* obj = reinterpret_cast<T*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		LOG_OBJECT_SCOPE(obj);
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}

template <typename T, typename HandleT>
error_type HighObjectRelease(HandleT handle) noexcept
{
	T* obj = reinterpret_cast<T*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		LOG_HIGH_OBJECT_SCOPE(obj);
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}


#endif /* _C_HELPER_H */
