#include "name_object.h"
#include "c_name_object.h"

#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Value(NameHandle handle, out_string_type result)
{
	NameObject* obj = reinterpret_cast<NameObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->Value();
		auto str = buffer.ToString();
		*result = str.c_str();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Release(NameHandle handle)
{
	NameObject* obj = reinterpret_cast<NameObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
