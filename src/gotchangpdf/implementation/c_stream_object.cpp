#include "precompiled.h"

#include "file.h"
#include "gotchangpdf/c_stream_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetHeader(StreamObjectHandle handle, DictionaryObjectHandle* result)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto header = obj->GetHeader();
		auto ptr = header.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_SetHeader(StreamObjectHandle handle, DictionaryObjectHandle value)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	DictionaryObject* header = reinterpret_cast<DictionaryObject*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(header);

	try
	{
		obj->SetHeader(header);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetBody(StreamObjectHandle handle, BufferHandle* result)
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
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_GetBodyRaw(StreamObjectHandle handle, BufferHandle* result)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto body = obj->GetBodyRaw();
		auto ptr = body.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_SetBody(StreamObjectHandle handle, BufferHandle value)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetBody(buffer);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StreamObject_Release(StreamObjectHandle handle)
{
	return ObjectRelease<StreamObject, StreamObjectHandle>(handle);
}
