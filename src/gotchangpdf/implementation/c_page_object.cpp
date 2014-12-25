#include "page_object.h"
#include "c_page_object.h"

using namespace gotchangpdf::documents;

GOTCHANG_PDF_API void CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	PageObject* obj = reinterpret_cast<PageObject*>(handle);
	obj->Release();
}