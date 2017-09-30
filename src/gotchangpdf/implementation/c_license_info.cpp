#include "precompiled.h"
#include "utils/license_info.h"

#include "gotchangpdf/c_license_info.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION LicenseInfo_SetLicenseFile(string_type filename) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);

	try {
		LicenseInfo::SetLicense(filename);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LicenseInfo_SetLicenseBuffer(BufferHandle data) {
	Buffer* buffer = reinterpret_cast<Buffer*>(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try {
		LicenseInfo::SetLicense(*buffer);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LicenseInfo_IsValid(boolean_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = LicenseInfo::IsValid();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}
