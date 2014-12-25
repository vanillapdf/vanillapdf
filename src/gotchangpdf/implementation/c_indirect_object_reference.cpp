#include "indirect_object_reference.h"
#include "c_indirect_object_reference.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API IndirectHandle CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectReferenceHandle handle)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	return reinterpret_cast<IndirectHandle>(AddRefGet(obj->GetReferencedObject()));
}
