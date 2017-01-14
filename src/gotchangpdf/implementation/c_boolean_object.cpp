#include "precompiled.h"
#include "syntax/files/file.h"

#include "gotchangpdf/c_boolean_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_GetValue(BooleanObjectHandle handle, boolean_type* result)
{
	BooleanObject* obj = reinterpret_cast<BooleanObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetValue();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_SetValue(BooleanObjectHandle handle, boolean_type value)
{
	BooleanObject* obj = reinterpret_cast<BooleanObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	if (value == GOTCHANG_PDF_RV_TRUE)
		obj->SetValue(true);
	else if (value == GOTCHANG_PDF_RV_FALSE)
		obj->SetValue(false);
	else
		return GOTCHANG_PDF_ERROR_GENERAL;

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_Release(BooleanObjectHandle handle)
{
	return ObjectRelease<BooleanObject, BooleanObjectHandle>(handle);
}
