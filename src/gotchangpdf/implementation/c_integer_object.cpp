#include "integer_object.h"
#include "c_integer_object.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API int CALLING_CONVENTION IntegerObject_Value(IntegerHandle handle)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	return obj->Value();
}

GOTCHANG_PDF_API void CALLING_CONVENTION IntegerObject_Release(IntegerHandle handle)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	obj->Release();
}
