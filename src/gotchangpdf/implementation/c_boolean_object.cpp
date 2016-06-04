#include "precompiled.h"
#include "file.h"

#include "c_boolean_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_GetValue(BooleanHandle handle, out_boolean_type result)
{
	BooleanObject* obj = reinterpret_cast<BooleanObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetValue();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_SetValue(BooleanHandle handle, boolean_type value)
{
	BooleanObject* obj = reinterpret_cast<BooleanObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	if (value == GOTCHANG_PDF_TRUE)
		obj->SetValue(true);
	else if (value == GOTCHANG_PDF_FALSE)
		obj->SetValue(false);
	else
		return GOTCHANG_PDF_ERROR_GENERAL;

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_Release(BooleanHandle handle)
{
	return ObjectRelease<BooleanObject, BooleanHandle>(handle);
}
