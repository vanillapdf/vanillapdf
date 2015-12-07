#include "precompiled.h"
#include "file.h"

#include "c_boolean_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_Value(BooleanHandle handle, out_boolean_type result)
{
	BooleanObject* obj = reinterpret_cast<BooleanObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	*result = obj->Value();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_Release(BooleanHandle handle)
{
	return ObjectRelease<BooleanObject, BooleanHandle>(handle);
}
