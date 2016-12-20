#include "precompiled.h"
#include "font.h"

#include "gotchangpdf/c_font_map.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION FontMap_Contains(FontMapHandle handle, NameObjectHandle name_handle, out_boolean_type result)
{
	FontMap* obj = reinterpret_cast<FontMap*>(handle);
	NameObject* name = reinterpret_cast<NameObject*>(name_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->Contains(name);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FontMap_Find(FontMapHandle handle, NameObjectHandle name_handle, PFontHandle result)
{
	FontMap* obj = reinterpret_cast<FontMap*>(handle);
	NameObject* name = reinterpret_cast<NameObject*>(name_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->Find(name);
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<FontHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FontMap_Release(FontMapHandle handle)
{
	return ObjectRelease<FontMap, FontMapHandle>(handle);
}