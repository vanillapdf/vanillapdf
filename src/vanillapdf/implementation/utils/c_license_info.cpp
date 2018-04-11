#include "precompiled.h"
#include "utils/license_info.h"

#include "vanillapdf/utils/c_license_info.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION LicenseInfo_SetLicenseFile(string_type filename) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);

	try {
		LicenseInfo::SetLicense(filename);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION LicenseInfo_SetLicenseBuffer(BufferHandle* data) {
	Buffer* buffer = reinterpret_cast<Buffer*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try {
		LicenseInfo::SetLicense(*buffer);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION LicenseInfo_IsValid(boolean_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = LicenseInfo::IsValid();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION LicenseInfo_IsTemporary(boolean_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = LicenseInfo::IsTemporary();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}
