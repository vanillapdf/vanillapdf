#include "precompiled.h"
#include "semantics/objects/font.h"

#include "vanillapdf/semantics/c_font_map.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Contains(FontMapHandle handle, const struct NameObjectHandleTag* key_handle, boolean_type* result)
{
	FontMap* obj = reinterpret_cast<FontMap*>(handle);
	const NameObject* key = reinterpret_cast<const NameObject*>(key_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->Contains(*key);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Find(FontMapHandle handle, const struct NameObjectHandleTag* key_handle, FontHandle* result)
{
	FontMap* obj = reinterpret_cast<FontMap*>(handle);
	const NameObject* key = reinterpret_cast<const NameObject*>(key_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->Find(*key);
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<FontHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Release(FontMapHandle handle)
{
	return ObjectRelease<FontMap, FontMapHandle>(handle);
}