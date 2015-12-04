#include "precompiled.h"
#include "file.h"

#include "c_indirect_object_reference.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectReferenceHandle handle, PObjectHandle result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	try
	{
		auto direct = obj->GetReferencedObject();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_Release(IndirectReferenceHandle handle)
{
	return ObjectRelease<IndirectObjectReference, IndirectReferenceHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObjectNumber(IndirectReferenceHandle handle, out_integer_type result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	*result = obj->GetReferencedObjectNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedGenerationNumber(IndirectReferenceHandle handle, out_integer_type result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	*result = obj->GetReferencedGenerationNumber();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
