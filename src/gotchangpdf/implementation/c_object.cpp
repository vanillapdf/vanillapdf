#include "precompiled.h"
#include "file.h"

#include "c_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

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
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	switch (obj->GetType()) {
	case Object::Type::Array:
		*result = ObjectType_Array; break;
	case Object::Type::Boolean:
		*result = ObjectType_Boolean; break;
	case Object::Type::Dictionary:
		*result = ObjectType_Dictionary; break;
	case Object::Type::Function:
		*result = ObjectType_Function; break;
	case Object::Type::Integer:
		*result = ObjectType_Integer; break;
	case Object::Type::Name:
		*result = ObjectType_Name; break;
	case Object::Type::Null:
		*result = ObjectType_Null; break;
	case Object::Type::Real:
		*result = ObjectType_Real; break;
	case Object::Type::Stream:
		*result = ObjectType_Stream; break;
	case Object::Type::String:
		*result = ObjectType_String; break;
	case Object::Type::IndirectReference:
		*result = ObjectType_IndirectReference; break;
	default:
		return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, out_offset_type result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	*result = obj->GetOffset();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	return ObjectRelease<Object, ObjectHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, PArrayHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

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
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

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
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

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
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

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
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	return SafeObjectConvert<Object, IntegerObject, IntegerHandle>(obj, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, PNameHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	return SafeObjectConvert<Object, NameObject, NameHandle>(obj, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToFunction(ObjectHandle handle, PFunctionHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	return SafeObjectConvert<Object, FunctionObject, FunctionHandle>(obj, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToBoolean(ObjectHandle handle, PBooleanHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	return SafeObjectConvert<Object, BooleanObject, BooleanHandle>(obj, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToReal(ObjectHandle handle, PRealHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	return SafeObjectConvert<Object, RealObject, RealHandle>(obj, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToNull(ObjectHandle handle, PNullHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	return SafeObjectConvert<Object, NullObject, NullHandle>(obj, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle handle, PStringHandle result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	return SafeObjectConvert<Object, StringObjectBase, StringHandle>(obj, result);
}
