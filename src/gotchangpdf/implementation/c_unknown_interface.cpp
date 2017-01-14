#include "precompiled.h"
#include "utils/unknown_interface.h"

#include "gotchangpdf/c_unknown_interface.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION IUnknown_AddRef(IUnknownHandle handle)
{
	IUnknown* obj = reinterpret_cast<IUnknown*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->AddRef();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IUnknown_Release(IUnknownHandle handle)
{
	return ObjectRelease<IUnknown, IUnknownHandle>(handle);
}
