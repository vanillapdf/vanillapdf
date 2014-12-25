#include "integer_object.h"
#include "c_integer_object.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API int CALLING_CONVENTION IntegerObject_Value(IntegerObjectHandle handle)
{
	gotchangpdf::IntegerObject* obj = reinterpret_cast<gotchangpdf::IntegerObject*>(handle);
	return obj->Value();
}

GOTCHANG_PDF_API void CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle handle)
{
	gotchangpdf::IntegerObject* obj = reinterpret_cast<gotchangpdf::IntegerObject*>(handle);
	obj->Release();
}
