#ifndef _C_HELPER_H
#define _C_HELPER_H

#include "exception.h"
#include "log.h"

#include "c_values.h"

#define RETURN_ERROR_IF_NULL(var, error_code) do { if (nullptr == var) return error_code; } while(0)
#define RETURN_ERROR_PARAM_VALUE_IF_NULL(var) RETURN_ERROR_IF_NULL(var, GOTCHANG_PDF_ERROR_PARAMETER_VALUE)

#define C_INTERFACE_EXCEPTION_HANDLERS \
	catch (gotchangpdf::syntax::Exception& e) \
	{ \
		LOG_ERROR << "Caught exception on interface: " << e.what(); \
		return GOTCHANG_PDF_ERROR_GENERAL; \
	} \
	catch (std::exception& e) \
	{ \
		LOG_ERROR << "Caught std exception on interface: " << e.what(); \
		return GOTCHANG_PDF_ERROR_GENERAL; \
	} \
	catch (...) \
	{ \
		LOG_ERROR << "Caught unknown exception on interface"; \
		return GOTCHANG_PDF_ERROR_GENERAL; \
	}

#define LOG_WEAK_FILE_SCOPE(file) \
	auto __locked_file__ = file.lock(); \
	if (!__locked_file__) \
		return GOTCHANG_PDF_ERROR_FILE_DISPOSED; \
	\
	LOG_SCOPE(__locked_file__->GetFilename()); \

template <typename SourceT, typename DestT, typename HandleT>
error_type SafeObjectConvert(SourceT* from, HandleT* result) noexcept
{
	try
	{
		DestT* converted = dynamic_cast<DestT*>(from);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<HandleT>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

template <typename T, typename HandleT>
error_type ObjectRelease(HandleT handle) noexcept
{
	T* obj = reinterpret_cast<T*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}


#endif /* _C_HELPER_H */
