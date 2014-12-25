#include "array_object.h"
#include "objects.h"

#include "c_array_object.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION ArrayObject_At(ArrayHandle handle, int at)
{
	MixedArrayObject* arr = reinterpret_cast<MixedArrayObject*>(handle);
	auto item = arr->At(at);
	ObjectBaseVisitor visitor;
	auto direct = item.apply_visitor(visitor);
	auto ptr = direct.AddRefGet();
	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API int CALLING_CONVENTION ArrayObject_Size(ArrayHandle handle)
{
	MixedArrayObject* arr = reinterpret_cast<MixedArrayObject*>(handle);
	return arr->Size();
}

GOTCHANG_PDF_API void CALLING_CONVENTION ArrayObject_Release(ArrayHandle handle)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	obj->Release();
}