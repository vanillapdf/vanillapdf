#include "precompiled.h"
#include "syntax/files/file.h"

#include "gotchangpdf/c_string_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_Type(StringObjectHandle handle, StringType* result)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

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

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_Release(StringObjectHandle handle)
{
	return ObjectRelease<StringObjectBase, StringObjectHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToLiteral(StringObjectHandle handle, LiteralStringObjectHandle* result)
{
	return SafeObjectConvert<StringObjectBase, LiteralStringObject, StringObjectHandle, LiteralStringObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToHexadecimal(StringObjectHandle handle, HexadecimalStringObjectHandle* result)
{
	return SafeObjectConvert<StringObjectBase, HexadecimalStringObject, StringObjectHandle, HexadecimalStringObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_GetValue(StringObjectHandle handle, BufferHandle* result)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetValue();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_SetValue(StringObjectHandle handle, BufferHandle value)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetValue(buffer);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_GetValue(LiteralStringObjectHandle handle, BufferHandle* result)
{
	LiteralStringObject* obj = reinterpret_cast<LiteralStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetValue();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_SetValue(LiteralStringObjectHandle handle, BufferHandle value)
{
	LiteralStringObject* obj = reinterpret_cast<LiteralStringObject*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetValue(buffer);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringObjectHandle handle)
{
	return ObjectRelease<LiteralStringObject, LiteralStringObjectHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_GetValue(HexadecimalStringObjectHandle handle, BufferHandle* result)
{
	HexadecimalStringObject* obj = reinterpret_cast<HexadecimalStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetValue();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_SetValue(HexadecimalStringObjectHandle handle, BufferHandle value)
{
	HexadecimalStringObject* obj = reinterpret_cast<HexadecimalStringObject*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetValue(buffer);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringObjectHandle handle)
{
	return ObjectRelease<HexadecimalStringObject, HexadecimalStringObjectHandle>(handle);
}
