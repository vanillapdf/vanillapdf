#include "precompiled.h"
#include "page_object.h"
#include "contents.h"

#include "c_page_object.h"
#include "c_helper.h"

using namespace gotchangpdf::documents;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle handle, PContentsHandle result)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	//LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		auto contents = obj->Contents();
		auto ptr = contents.AddRefGet();
		*result = reinterpret_cast<ContentsHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
