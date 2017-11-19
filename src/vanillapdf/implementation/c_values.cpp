#include "precompiled.h"
#include "vanillapdf/c_values.h"

#include "utils/exceptions.h"

// boolean values
VANILLAPDF_API const boolean_type VANILLAPDF_RV_FALSE	= false;
VANILLAPDF_API const boolean_type VANILLAPDF_RV_TRUE	= true;

// global error states
VANILLAPDF_API const error_type VANILLAPDF_ERROR_SUCCESS				= static_cast<error_type>(vanillapdf::ExceptionBase::Type::Success);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_PARAMETER_VALUE		= static_cast<error_type>(vanillapdf::ExceptionBase::Type::InvalidParameter);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_NOT_SUPPORTED			= static_cast<error_type>(vanillapdf::ExceptionBase::Type::NotSupported);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_USER_CANCELLED			= static_cast<error_type>(vanillapdf::ExceptionBase::Type::UserCancelled);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_ZLIB_DATA				= static_cast<error_type>(vanillapdf::ExceptionBase::Type::ZlibDataError);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_INVALID_LICENSE		= static_cast<error_type>(vanillapdf::ExceptionBase::Type::InvalidLicense);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_GENERAL				= static_cast<error_type>(vanillapdf::ExceptionBase::Type::General);

// syntax errors
VANILLAPDF_API const error_type VANILLAPDF_ERROR_CONVERSION				= static_cast<error_type>(vanillapdf::ExceptionBase::Type::Conversion);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_FILE_DISPOSED			= static_cast<error_type>(vanillapdf::ExceptionBase::Type::FileDisposed);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_FILE_NOT_INITIALIZED	= static_cast<error_type>(vanillapdf::ExceptionBase::Type::FileNotInitialized);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_OBJECT_MISSING			= static_cast<error_type>(vanillapdf::ExceptionBase::Type::ObjectMissing);
VANILLAPDF_API const error_type VANILLAPDF_ERROR_INVALID_PASSWORD		= static_cast<error_type>(vanillapdf::ExceptionBase::Type::InvalidPassword);

// semantic errors