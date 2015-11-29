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

#endif /* _C_HELPER_H */
