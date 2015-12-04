#include "precompiled.h"
#include "file.h"

#include "c_string_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_Type(StringHandle handle, PStringType result)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	switch (obj->GetStringType()) {
	case StringObjectBase::StringType::Literal:
		*result = StringType_Literal; break;
	case StringObjectBase::StringType::Hexadecimal:
		*result = StringType_Hexadecimal; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToLiteral(StringHandle handle, PLiteralStringHandle result)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	try {
		LiteralStringObject* converted = dynamic_cast<LiteralStringObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<LiteralStringHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToHexadecimal(StringHandle handle, PHexadecimalStringHandle result)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	try {
		HexadecimalStringObject* converted = dynamic_cast<HexadecimalStringObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<HexadecimalStringHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Value(LiteralStringHandle handle, PBufferHandle result)
{
	LiteralStringObject* obj = reinterpret_cast<LiteralStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	auto buffer = obj->Value();
	auto ptr = buffer.AddRefGet();
	*result = reinterpret_cast<BufferHandle>(ptr);
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringHandle handle)
{
	LiteralStringObject* obj = reinterpret_cast<LiteralStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Value(HexadecimalStringHandle handle, PBufferHandle result)
{
	HexadecimalStringObject* obj = reinterpret_cast<HexadecimalStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	auto buffer = obj->Value();
	auto ptr = buffer.AddRefGet();
	*result = reinterpret_cast<BufferHandle>(ptr);
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringHandle handle)
{
	HexadecimalStringObject* obj = reinterpret_cast<HexadecimalStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}
