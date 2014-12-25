#include "indirect_object_reference.h"
#include "c_indirect_object_reference.h"

GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectObjectReferenceHandle handle)
{
	gotchangpdf::IndirectObjectReference* obj = reinterpret_cast<gotchangpdf::IndirectObjectReference*>(handle);
	return reinterpret_cast<IndirectObjectHandle>(AddRefGet(obj->GetReferencedObject()));
}
