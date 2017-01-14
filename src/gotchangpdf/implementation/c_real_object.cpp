#include "precompiled.h"
#include "syntax/files/file.h"

#include "gotchangpdf/c_real_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_GetValue(RealObjectHandle handle, real_type* result)
{
	RealObject* obj = reinterpret_cast<RealObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetValue();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_SetValue(RealObjectHandle handle, real_type value)
{
	RealObject* obj = reinterpret_cast<RealObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetValue(value);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_Release(RealObjectHandle handle)
{
	return ObjectRelease<RealObject, RealObjectHandle>(handle);
}
