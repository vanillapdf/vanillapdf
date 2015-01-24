#include "precompiled.h"

#include "c_integer_object.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_Value(IntegerHandle handle, out_integer_type result)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->Value();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_Release(IntegerHandle handle)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
