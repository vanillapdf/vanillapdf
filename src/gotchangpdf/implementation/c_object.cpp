#include "precompiled.h"
#include "file.h"

#include "c_object.h"
#include "c_helper.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API error_type Object_TypeName(ObjectType type, out_string_type result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	try
	{
		auto value = static_cast<Object::Type>(type);
		*result = Object::TypeName(value);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Type(ObjectHandle handle, PObjectType result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		switch (obj->GetType()) {
		case Object::Type::Array:
			*result = Array; break;
		case Object::Type::Boolean:
			*result = Boolean; break;
		case Object::Type::Dictionary:
			*result = Dictionary; break;
		case Object::Type::Function:
			*result = Function; break;
		case Object::Type::Integer:
			*result = Integer; break;
		case Object::Type::Name:
			*result = Name; break;
		case Object::Type::Null:
			*result = Null; break;
		case Object::Type::Real:
			*result = Real; break;
		case Object::Type::Stream:
			*result = Stream; break;
		case Object::Type::HexadecimalString:
			*result = HexadecimalString; break;
		case Object::Type::LiteralString:
			*result = LiteralString; break;
		case Object::Type::IndirectReference:
			*result = IndirectReference; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, out_offset_type result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	obj->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, PArrayHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		MixedArrayObject* converted = dynamic_cast<MixedArrayObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<ArrayHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle handle, PDictionaryHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		DictionaryObject* converted = dynamic_cast<DictionaryObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<DictionaryHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle handle, PStreamHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		StreamObject* converted = dynamic_cast<StreamObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<StreamHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle handle, PIndirectReferenceHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		IndirectObjectReference* converted = dynamic_cast<IndirectObjectReference*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<IndirectReferenceHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle handle, PIntegerHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		IntegerObject* converted = dynamic_cast<IntegerObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<IntegerHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, PNameHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try
	{
		NameObject* converted = dynamic_cast<NameObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<NameHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToLiteralString(ObjectHandle handle, PLiteralStringHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try {
		LiteralStringObject* converted = dynamic_cast<LiteralStringObject*>(obj);
		*result = reinterpret_cast<LiteralStringHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToHexadecimalString(ObjectHandle handle, PHexadecimalStringHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	LOG_SCOPE(obj->GetFile()->GetFilename());

	try {
		HexadecimalStringObject* converted = dynamic_cast<HexadecimalStringObject*>(obj);
		if (nullptr == converted)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		*result = reinterpret_cast<HexadecimalStringHandle>(converted);
		return GOTCHANG_PDF_ERROR_SUCCES;
	}
	C_INTERFACE_EXCEPTION_HANDLERS
}
