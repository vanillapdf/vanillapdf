#include "precompiled.h"

#include "c_stream_object.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_Header(StreamHandle handle, PDictionaryHandle result)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto header = obj->GetHeader();
		auto ptr = header.AddRefGet();
		*result = reinterpret_cast<DictionaryHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_BodyRaw(StreamHandle handle, PBufferHandle result)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto header = obj->GetBody();
		auto ptr = header.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_BodyDecoded(StreamHandle handle, PBufferHandle result)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto header = obj->GetBodyDecoded();
		auto ptr = header.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_Release(StreamHandle handle)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
