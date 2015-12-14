#include "precompiled.h"
#include "file.h"

#include "c_real_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_Value(RealHandle handle, out_real_type result)
{
	RealObject* obj = reinterpret_cast<RealObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_OBJECT_SCOPE(obj);

		*result = obj->Value();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_Release(RealHandle handle)
{
	return ObjectRelease<RealObject, RealHandle>(handle);
}
