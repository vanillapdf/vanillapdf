#include "precompiled.h"

#include "syntax/objects/integer_object.h"

#include "gotchangpdf/c_integer_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_GetIntegerValue(IntegerObjectHandle handle, bigint_type* result)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetIntegerValue();
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_GetUnsignedIntegerValue(IntegerObjectHandle handle, biguint_type* result)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetUnsignedIntegerValue();
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_SetIntegerValue(IntegerObjectHandle handle, bigint_type value)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetValue(value);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_SetUnsignedIntegerValue(IntegerObjectHandle handle, biguint_type value)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetValue(value);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle handle)
{
	return ObjectRelease<IntegerObject, IntegerObjectHandle>(handle);
}
