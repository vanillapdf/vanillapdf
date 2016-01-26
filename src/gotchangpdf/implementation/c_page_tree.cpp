#include "precompiled.h"
#include "page_tree.h"
#include "file.h"

#include "c_page_tree.h"
#include "c_helper.h"

using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle handle, out_integer_type result)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->PageCount();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPage(PageTreeHandle handle, integer_type at, PPageObjectHandle result)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto page = obj->Page(at);
		auto ptr = page.AddRefGet();
		*result = reinterpret_cast<PageObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_Release(PageTreeHandle handle)
{
	return ObjectRelease<PageTree, PageTreeHandle>(handle);
}