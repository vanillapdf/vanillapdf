#include "precompiled.h"
#include "page_object.h"
#include "contents.h"
#include "file.h"

#include "c_page_object.h"
#include "c_helper.h"

using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle handle, PContentsHandle result)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		ContentsPtr contents;
		auto contains = obj->Contents(contents);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = contents.AddRefGet();
		*result = reinterpret_cast<ContentsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	return ObjectRelease<PageObject, PageObjectHandle>(handle);
}
