#include "precompiled.h"
#include "c_values.h"

#include "exception.h"

// boolean values
GOTCHANG_PDF_API const boolean_type GOTCHANG_PDF_FALSE	= false;
GOTCHANG_PDF_API const boolean_type GOTCHANG_PDF_TRUE	= true;

// global error states
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_SUCCES				= static_cast<error_type>(gotchangpdf::ExceptionBase::Type::Success);
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_PARAMETER_VALUE	= static_cast<error_type>(gotchangpdf::ExceptionBase::Type::InvalidParameter);
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_NOT_SUPPORTED		= static_cast<error_type>(gotchangpdf::ExceptionBase::Type::NotSupported);
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_GENERAL			= static_cast<error_type>(gotchangpdf::ExceptionBase::Type::General);

// syntax errors
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_FILE_DISPOSED			= static_cast<error_type>(gotchangpdf::ExceptionBase::Type::FileDisposed);
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_FILE_NOT_INITIALIZED	= static_cast<error_type>(gotchangpdf::ExceptionBase::Type::FileNotInitialized);
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_OBJECT_MISSING			= static_cast<error_type>(gotchangpdf::ExceptionBase::Type::ObjectMissing);
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_INVALID_PASSWORD		= static_cast<error_type>(gotchangpdf::ExceptionBase::Type::InvalidPassword);

// semantic errors
GOTCHANG_PDF_API const error_type GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING = static_cast<error_type>(gotchangpdf::ExceptionBase::Type::OptionalEntryMissing);