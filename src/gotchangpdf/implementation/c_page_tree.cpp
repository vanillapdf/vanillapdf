#include "precompiled.h"
#include "page_tree.h"

#include "c_page_tree.h"

#include "c_helper.h"

using namespace gotchangpdf::documents;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle handle, out_integer_type result)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->PageCount();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
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
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_Release(PageTreeHandle handle)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}