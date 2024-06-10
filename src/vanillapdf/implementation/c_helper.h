#ifndef _C_HELPER_H
#define _C_HELPER_H

#include "utils/log.h"
#include "utils/errors.h"
#include "utils/unknown_interface.h"

#include "vanillapdf/c_values.h"

#include <type_traits>

#define RETURN_ERROR_IF_NULL(var, error_code) do { if (nullptr == var) return error_code; } while(0)
#define RETURN_ERROR_PARAM_VALUE_IF_NULL(var) RETURN_ERROR_IF_NULL(var, VANILLAPDF_ERROR_PARAMETER_VALUE)

#define CATCH_VANILLAPDF_EXCEPTIONS \
	catch (vanillapdf::ExceptionBase& e) \
	{ \
		spdlog::error("{}", e.what()); \
		error_type code = static_cast<error_type>(e.code()); \
		vanillapdf::Errors::SetLastError(code); \
		vanillapdf::Errors::SetLastErrorMessage(e.what()); \
		return code; \
	} \
	catch (std::exception& e) \
	{ \
		spdlog::error("{}", e.what()); \
		vanillapdf::Errors::SetLastError(VANILLAPDF_ERROR_GENERAL); \
		vanillapdf::Errors::SetLastErrorMessage(e.what()); \
		return VANILLAPDF_ERROR_GENERAL; \
	} \
	catch (...) \
	{ \
		spdlog::error("Caught unknown exception"); \
		vanillapdf::Errors::SetLastError(VANILLAPDF_ERROR_GENERAL); \
		return VANILLAPDF_ERROR_GENERAL; \
	}

template <
	typename SourceT,
	typename DestT,
	typename SourceHandleT,
	typename DestHandleT,
	typename = typename std::enable_if<std::is_polymorphic<SourceT>::value>::type,
	typename = typename std::enable_if<
		std::is_convertible<SourceT *, DestT *>::value ||
		std::is_base_of<SourceT, DestT>::value
	>::type
>
error_type SafeObjectConvert(SourceHandleT* from, DestHandleT** result) {
	SourceT* obj = reinterpret_cast<SourceT*>(from);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		DestT* converted = dynamic_cast<DestT*>(obj);
		if (nullptr == converted) {
			return VANILLAPDF_ERROR_PARAMETER_VALUE;
		}

		// My friend, we need to talk and it won't be easy
		// Back in my days, we didn't have to increment the
		// reference counters when converting from one to another

		// When implementing .NET wrapper with usage of SafeHandle
		// the things get complicated. It would not be pretty
		// to increment the counters here, but not there.
		
		// Let's be nice and keep the interface consistent
		// AND
		// Increment the reference counter
		converted->AddRef();

		*result = reinterpret_cast<DestHandleT*>(converted);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

template <
	typename T,
	typename HandleT,
	typename = typename std::enable_if<std::is_base_of<vanillapdf::IUnknown, T>::value>::type
>
error_type ObjectRelease(HandleT* handle) {
	T* obj = reinterpret_cast<T*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->Release();
	return VANILLAPDF_ERROR_SUCCESS;
}


#endif /* _C_HELPER_H */
