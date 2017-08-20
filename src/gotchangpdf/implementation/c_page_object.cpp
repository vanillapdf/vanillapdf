#include "precompiled.h"

#include "semantics/objects/page_object.h"
#include "semantics/objects/contents.h"
#include "semantics/objects/document.h"

#include "syntax/files/file.h"

#include "gotchangpdf/c_page_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle handle, ContentsHandle* result)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputContentsPtr contents;
		auto contains = obj->GetContents(contents);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = contents.AddRefGet();
		*result = reinterpret_cast<ContentsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_CreateFromDocument(DocumentHandle handle, PageObjectHandle* result)
{
	Document* obj = reinterpret_cast<Document*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto page = PageObject::Create(obj);
		auto ptr = page.release();

		// Release from unique ptr does not increase internal ref counter
		ptr->AddRef();

		*result = reinterpret_cast<PageObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_CreateFromObject(DictionaryObjectHandle handle, PageObjectHandle* result)
{
	syntax::DictionaryObject* obj = reinterpret_cast<syntax::DictionaryObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto page = PageObject::Create(obj);
		auto ptr = page.release();

		// Release from unique ptr does not increase internal ref counter
		ptr->AddRef();

		*result = reinterpret_cast<PageObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetParent(PageObjectHandle handle, PageTreeHandle* result)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto parent = obj->GetParent();
		auto ptr = parent.AddRefGet();
		*result = reinterpret_cast<PageTreeHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetResources(PageObjectHandle handle, ResourceDictionaryHandle* result)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto parent = obj->GetResources();
		auto ptr = parent.AddRefGet();
		*result = reinterpret_cast<ResourceDictionaryHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetAnnotations(PageObjectHandle handle, PageAnnotationsHandle* result)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		OutputPageAnnotationsPtr annots;
		bool contains = obj->GetAnnotations(annots);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = annots.AddRefGet();
		*result = reinterpret_cast<PageAnnotationsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetMediaBox(PageObjectHandle handle, RectangleHandle* result)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto media_box = obj->GetMediaBox();
		auto ptr = media_box.AddRefGet();
		*result = reinterpret_cast<RectangleHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	return ObjectRelease<PageObject, PageObjectHandle>(handle);
}
