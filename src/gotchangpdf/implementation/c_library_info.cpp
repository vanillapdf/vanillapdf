#include "precompiled.h"
#include "library_info.h"

#include "gotchangpdf/c_library_info.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMajor(out_integer_type result)
{
	*result = LibraryInfo::MajorVersion();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMinor(out_integer_type result)
{
	*result = LibraryInfo::MinorVersion();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionPatch(out_integer_type result)
{
	*result = LibraryInfo::PatchVersion();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LibraryInfo_GetAuthor(out_string_type result)
{
	*result = LibraryInfo::Author();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
