#include "precompiled.h"
#include "utils/errors.h"

#include "vanillapdf/utils/c_errors.h"
#include "implementation/c_helper.h"

#include <cstring>

using namespace vanillapdf;

namespace {

#if (defined(STRINGIFY) || defined(CHECK_CODE))
	#error "Helper macros already defined"
#endif // (defined(STRINGIFY) || defined(CHECK_CODE))


#define STRINGIFY(name) #name

#define CHECK_CODE(code, constant) \
	if (code == constant) { \
		static const char value[] = STRINGIFY(constant); \
		data = value; \
		length = sizeof(value); \
		return true; \
	}

	static bool GetValueName(error_type code, string_type& data, size_type& length) {

		// global
		CHECK_CODE(code, VANILLAPDF_ERROR_SUCCESS);
		CHECK_CODE(code, VANILLAPDF_ERROR_PARAMETER_VALUE);
		CHECK_CODE(code, VANILLAPDF_ERROR_NOT_SUPPORTED);
		CHECK_CODE(code, VANILLAPDF_ERROR_USER_CANCELLED);
		CHECK_CODE(code, VANILLAPDF_ERROR_ZLIB_DATA);
		CHECK_CODE(code, VANILLAPDF_ERROR_INVALID_LICENSE);
		CHECK_CODE(code, VANILLAPDF_ERROR_GENERAL);

		// syntax
		CHECK_CODE(code, VANILLAPDF_ERROR_CONVERSION);
		CHECK_CODE(code, VANILLAPDF_ERROR_FILE_DISPOSED);
		CHECK_CODE(code, VANILLAPDF_ERROR_FILE_NOT_INITIALIZED);
		CHECK_CODE(code, VANILLAPDF_ERROR_OBJECT_MISSING);
		CHECK_CODE(code, VANILLAPDF_ERROR_INVALID_PASSWORD);

		return false;
	}
}

VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastError(error_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = Errors::GetLastError();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetPrintableErrorTextLength(error_type value, size_type* size) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(size);

	string_type value_data = nullptr;
	size_type value_data_size = 0;

	bool success = GetValueName(value, value_data, value_data_size);

	if (!success || value_data == nullptr || value_data_size == 0) {
		return VANILLAPDF_ERROR_GENERAL;
	}

	*size = value_data_size;
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetPrintableErrorText(error_type value, char* data, size_type size) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	string_type value_data = nullptr;
	size_type value_data_size = 0;

	bool success = GetValueName(value, value_data, value_data_size);

	if (!success || value_data == nullptr || value_data_size == 0) {
		LOG_ERROR_GLOBAL << "Errors_GetPrintableErrorText: Failed to get error code";
		return VANILLAPDF_ERROR_GENERAL;
	}

	if (size < value_data_size) {
		LOG_ERROR_GLOBAL << "Errors_GetPrintableErrorText: Buffer too small";
		return VANILLAPDF_ERROR_GENERAL;
	}

	LOG_DEBUG_GLOBAL << "Errors_GetPrintableErrorText (" << value << "): " << value_data;
	std::memcpy(data, value_data, value_data_size);
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessageLength(size_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = Errors::GetLastErrorMessageLength();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessage(char* data, size_type size) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	auto message = Errors::GetLastErrorMessage();
	auto message_size = Errors::GetLastErrorMessageLength();

	// Terminate if the buffer is small
	if (size < message_size) {
		return VANILLAPDF_ERROR_GENERAL;
	}

	std::memcpy(data, message, message_size);
	return VANILLAPDF_ERROR_SUCCESS;
}
