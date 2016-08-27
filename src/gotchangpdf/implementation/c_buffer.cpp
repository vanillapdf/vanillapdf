#include "precompiled.h"

#include "c_buffer.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_GetData(BufferHandle handle, out_string_type data, out_string_size_type size)
{
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(size);

	*data = obj->data();
	*size = obj->size();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_SetData(BufferHandle handle, string_type data, string_size_type size)
{
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		obj->assign(data, data + size);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Buffer_Release(BufferHandle handle)
{
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
