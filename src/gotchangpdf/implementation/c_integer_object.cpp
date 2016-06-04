#include "precompiled.h"
#include "file.h"

#include "c_integer_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_GetValue(IntegerHandle handle, out_biguint_type result)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetValue();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_SetValue(IntegerHandle handle, biguint_type value)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->SetValue(value);
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_Release(IntegerHandle handle)
{
	return ObjectRelease<IntegerObject, IntegerHandle>(handle);
}
