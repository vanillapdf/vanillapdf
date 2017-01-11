#include "precompiled.h"

#include "gotchangpdf/c_null_object.h"
#include "c_helper.h"

using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION NullObject_Release(NullObjectHandle handle)
{
	return ObjectRelease<NullObject, NullObjectHandle>(handle);
}
