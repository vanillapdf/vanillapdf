#include "precompiled.h"
#include "file.h"

#include "c_array_object.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_At(ArrayHandle handle, integer_type at, PObjectHandle result)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		auto direct = obj->At(at);
		ObjectBaseAddRefVisitor visitor;
		auto base = direct.apply_visitor(visitor);
		*result = reinterpret_cast<ObjectHandle>(base);

		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayHandle handle, out_integer_type result)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	*result = obj->Size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayHandle handle)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
