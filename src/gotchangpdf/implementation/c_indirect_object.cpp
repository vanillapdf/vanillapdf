#include "indirect_object.h"
#include "c_indirect_object.h"

#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectObject_GetOffset(IndirectHandle handle, out_offset_type result)
{
	IndirectObject* obj = reinterpret_cast<IndirectObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		*result = obj->GetOffset();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectObject_GetObject(IndirectHandle handle, PObjectHandle result)
{
	IndirectObject* obj = reinterpret_cast<IndirectObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		DirectObject item = obj->GetObject();
		ObjectBaseAddRefVisitor visitor;
		auto base = item.apply_visitor(visitor);

		*result = reinterpret_cast<ObjectHandle>(base);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectObject_Release(IndirectHandle handle)
{
	IndirectObject* obj = reinterpret_cast<IndirectObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
