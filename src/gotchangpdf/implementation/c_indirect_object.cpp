#include "indirect_object.h"
#include "c_indirect_object.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API long long CALLING_CONVENTION IndirectObject_GetOffset(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);
	return obj->GetOffset();
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION IndirectObject_GetObject(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);

	DirectObject item = obj->GetObject();
	ObjectBaseVisitor visitor;

	Object *ptr = item.apply_visitor(visitor).AddRefGet();
	//boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API void CALLING_CONVENTION IndirectObject_Release(IndirectObjectHandle handle)
{
	gotchangpdf::IndirectObject* obj = reinterpret_cast<gotchangpdf::IndirectObject*>(handle);
	obj->Release();
}
