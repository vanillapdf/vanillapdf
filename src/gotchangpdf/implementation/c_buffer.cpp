#include "precompiled.h"

#include "c_buffer.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_GetSize(BufferHandle handle, out_size_type result)
{
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_GetData(BufferHandle handle, out_string_type result)
{
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->data();
	return GOTCHANG_PDF_ERROR_SUCCES;

}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_Release(BufferHandle handle)
{
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->Release();
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
