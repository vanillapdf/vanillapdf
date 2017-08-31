#include "precompiled.h"
#include "utils/library_info.h"

#include "gotchangpdf/c_library_info.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMajor(integer_type* result)
{
	*result = LibraryInfo::MajorVersion();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMinor(integer_type* result)
{
	*result = LibraryInfo::MinorVersion();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionPatch(integer_type* result)
{
	*result = LibraryInfo::PatchVersion();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetAuthor(string_type* result)
{
	*result = LibraryInfo::Author();
	return GOTCHANG_PDF_ERROR_SUCCESS;
}
