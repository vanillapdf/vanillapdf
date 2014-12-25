#include "array_object.h"
#include "objects.h"

#include "c_array_object.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle handle, int at)
{
	gotchangpdf::MixedArrayObject* arr = reinterpret_cast<gotchangpdf::MixedArrayObject*>(handle);
	auto item = arr->At(at);
	ObjectBaseVisitor visitor;
	auto direct = item.apply_visitor(visitor);
	auto ptr = direct.AddRefGet();
	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API int CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle handle)
{
	gotchangpdf::MixedArrayObject* arr = reinterpret_cast<gotchangpdf::MixedArrayObject*>(handle);
	return arr->Size();
}

GOTCHANG_PDF_API void CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle handle)
{
	gotchangpdf::MixedArrayObject* obj = reinterpret_cast<gotchangpdf::MixedArrayObject*>(handle);
	obj->Release();
}