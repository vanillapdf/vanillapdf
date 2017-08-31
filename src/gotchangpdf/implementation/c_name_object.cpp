#include "precompiled.h"

#include "syntax/objects/name_object.h"

#include "gotchangpdf/c_name_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_GetValue(const struct NameObjectHandleTag* handle, BufferHandle* result)
{
	const NameObject* obj = reinterpret_cast<const NameObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetValue();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_SetValue(NameObjectHandle handle, BufferHandle value)
{
	NameObject* obj = reinterpret_cast<NameObject*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetValue(buffer);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Release(NameObjectHandle handle)
{
	return ObjectRelease<NameObject, NameObjectHandle>(handle);
}
