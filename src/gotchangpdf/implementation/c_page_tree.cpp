#include "precompiled.h"
#include "semantics/objects/page_tree.h"
#include "syntax/files/file.h"

#include "gotchangpdf/c_page_tree.h"
#include "c_helper.h"

using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle handle, integer_type* result)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->PageCount();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPage(PageTreeHandle handle, integer_type at, PageObjectHandle* result)
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_InsertPage(PageTreeHandle handle, integer_type at, PageObjectHandle page_handle)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	PageObject* page = reinterpret_cast<PageObject*>(page_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(page);

	try
	{
		obj->Insert(page, at);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_AppendPage(PageTreeHandle handle, PageObjectHandle page_handle)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	PageObject* page = reinterpret_cast<PageObject*>(page_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(page);

	try
	{
		obj->Append(page);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_RemovePage(PageTreeHandle handle, integer_type at)
{
	PageTree* obj = reinterpret_cast<PageTree*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Remove(at);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_Release(PageTreeHandle handle)
{
	return ObjectRelease<PageTree, PageTreeHandle>(handle);
}