#ifndef _C_HELPER_H
#define _C_HELPER_H

#include "utils/log.h"
#include "utils/errors.h"

#include "gotchangpdf/c_values.h"

#define RETURN_ERROR_IF_NULL(var, error_code) do { if (nullptr == var) return error_code; } while(0)
#define RETURN_ERROR_PARAM_VALUE_IF_NULL(var) RETURN_ERROR_IF_NULL(var, GOTCHANG_PDF_ERROR_PARAMETER_VALUE)

#define CATCH_GOTCHNGPDF_EXCEPTIONS \
	catch (gotchangpdf::ExceptionBase& e) \
	{ \
		LOG_ERROR_GLOBAL << e.what(); \
		error_type code = static_cast<error_type>(e.code()); \
		gotchangpdf::Errors::SetLastError(code); \
		gotchangpdf::Errors::SetLastErrorMessage(e.what()); \
		return code; \
	} \
	catch (std::exception& e) \
	{ \
		LOG_ERROR_GLOBAL << e.what(); \
		gotchangpdf::Errors::SetLastError(GOTCHANG_PDF_ERROR_GENERAL); \
		gotchangpdf::Errors::SetLastErrorMessage(e.what()); \
		return GOTCHANG_PDF_ERROR_GENERAL; \
	} \
	catch (...) \
	{ \
		LOG_ERROR_GLOBAL << "Caught unknown exception"; \
		gotchangpdf::Errors::SetLastError(GOTCHANG_PDF_ERROR_GENERAL); \
		return GOTCHANG_PDF_ERROR_GENERAL; \
	}

template <typename SourceT, typename DestT, typename SourceHandleT, typename DestHandleT>
error_type SafeObjectConvert(SourceHandleT from, DestHandleT* result) noexcept
{
	SourceT* obj = reinterpret_cast<SourceT*>(from);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		DestT* converted = dynamic_cast<DestT*>(obj);
		if (nullptr == converted) {
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;
		}

		*result = reinterpret_cast<DestHandleT>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

template <typename T, typename HandleT>
error_type ObjectRelease(HandleT handle) noexcept
{
	T* obj = reinterpret_cast<T*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}


#endif /* _C_HELPER_H */
