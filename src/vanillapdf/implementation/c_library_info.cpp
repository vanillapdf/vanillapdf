#include "precompiled.h"
#include "utils/library_info.h"

#include "vanillapdf/c_library_info.h"
#include "c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMajor(integer_type* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = LibraryInfo::MajorVersion();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionMinor(integer_type* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = LibraryInfo::MinorVersion();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetVersionPatch(integer_type* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = LibraryInfo::PatchVersion();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetAuthor(string_type* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = LibraryInfo::Author();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetBuildDay(integer_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = LibraryInfo::BuildDay();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetBuildMonth(integer_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = LibraryInfo::BuildMonth();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION LibraryInfo_GetBuildYear(integer_type* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = LibraryInfo::BuildYear();
	return VANILLAPDF_ERROR_SUCCESS;
}
