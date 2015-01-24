#include "precompiled.h"
#include "page_object.h"

#include "c_page_object.h"
#include "c_helper.h"

using namespace gotchangpdf::documents;

GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
