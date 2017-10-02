#include "precompiled.h"
#include "utils/errors.h"

#include "vanillapdf/c_errors.h"
#include "c_helper.h"

#include <cstring>

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastError(error_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = Errors::GetLastError();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessageLength(size_type* size) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(size);

	try {
		std::string message = Errors::GetLastErrorMessage();
		*size = message.size() + 1;
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessage(char* data, size_type size) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try {
		std::string message = Errors::GetLastErrorMessage();

		// Include the NULL termination character
		size_t message_size = message.size() + 1;

		// Terminate if the buffer is small
		if (size < message_size) {
			return VANILLAPDF_ERROR_GENERAL;
		}
		
		std::memcpy(data, message.c_str(), message_size);

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}
