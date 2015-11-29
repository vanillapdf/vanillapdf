#include "precompiled.h"
#include "file.h"

#include "c_string_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Value(LiteralStringHandle handle, PBufferHandle result)
{
	LiteralStringObject* obj = reinterpret_cast<LiteralStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	auto buffer = obj->Value();
	auto ptr = buffer.AddRefGet();
	*result = reinterpret_cast<BufferHandle>(ptr);
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringHandle handle)
{
	LiteralStringObject* obj = reinterpret_cast<LiteralStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Value(HexadecimalStringHandle handle, PBufferHandle result)
{
	HexadecimalStringObject* obj = reinterpret_cast<HexadecimalStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	auto buffer = obj->Value();
	auto ptr = buffer.AddRefGet();
	*result = reinterpret_cast<BufferHandle>(ptr);
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringHandle handle)
{
	HexadecimalStringObject* obj = reinterpret_cast<HexadecimalStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
