#include "indirect_object_reference.h"
#include "c_indirect_object_reference.h"

#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectReferenceHandle handle, PIndirectHandle result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		*result = reinterpret_cast<IndirectHandle>(AddRefGet(obj->GetReferencedObject()));
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_Release(IndirectReferenceHandle handle)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
