#include "precompiled.h"
#include "resource_dictionary.h"

#include "gotchangpdf/c_resource_dictionary.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION ResourceDictionary_GetFontMap(ResourceDictionaryHandle handle, PFontMapHandle result)
{
	ResourceDictionary* obj = reinterpret_cast<ResourceDictionary*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputFontMapPtr font;
		bool contains = obj->Font(font);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = font.AddRefGet();
		*result = reinterpret_cast<FontMapHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ResourceDictionary_Release(ResourceDictionaryHandle handle)
{
	return ObjectRelease<ResourceDictionary, ResourceDictionaryHandle>(handle);
}