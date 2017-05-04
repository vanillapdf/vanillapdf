#include "precompiled.h"
#include "utils/errors.h"

#include "gotchangpdf/c_errors.h"
#include "c_helper.h"

#include <cstring>

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Errors_GetLastError(error_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = Errors::GetLastError();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessageLength(size_type* size) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(size);

	try {
		std::string message = Errors::GetLastErrorMessage();
		*size = message.size();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Errors_GetLastErrorMessage(char* data, size_type size) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try {
		std::string message = Errors::GetLastErrorMessage();

		if (size < message.size()) {
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		std::memcpy(data, message.data(), message.size());

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}
