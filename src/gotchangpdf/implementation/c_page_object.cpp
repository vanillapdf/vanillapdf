#include "precompiled.h"
#include "page_object.h"
#include "contents.h"
#include "file.h"
#include "document.h"

#include "c_page_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle handle, PContentsHandle result)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputContentsPtr contents;
		auto contains = obj->Contents(contents);
		if (!contains) return GOTCHANG_PDF_ERROR_OPTIONAL_ENTRY_MISSING;
		auto ptr = contents.AddRefGet();
		*result = reinterpret_cast<ContentsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_CreateFromDocument(DocumentHandle handle, PPageObjectHandle result)
{
	Document* obj = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto page = PageObject::Create(obj);
		auto ptr = page.release();
		*result = reinterpret_cast<PageObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_CreateFromObject(DictionaryHandle handle, PPageObjectHandle result)
{
	syntax::DictionaryObject* obj = reinterpret_cast<syntax::DictionaryObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto page = PageObject::Create(obj);
		auto ptr = page.release();
		*result = reinterpret_cast<PageObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	return ObjectRelease<PageObject, PageObjectHandle>(handle);
}
