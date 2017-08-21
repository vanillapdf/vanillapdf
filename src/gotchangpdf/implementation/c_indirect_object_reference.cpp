#include "precompiled.h"

#include "syntax/objects/indirect_object_reference.h"

#include "gotchangpdf/c_indirect_object_reference.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectObjectReferenceHandle handle, ObjectHandle* result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		auto direct = obj->GetReferencedObject();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_Release(IndirectObjectReferenceHandle handle)
{
	return ObjectRelease<IndirectObjectReference, IndirectObjectReferenceHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObjectNumber(IndirectObjectReferenceHandle handle, biguint_type* result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetReferencedObjectNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedGenerationNumber(IndirectObjectReferenceHandle handle, ushort_type* result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetReferencedGenerationNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
