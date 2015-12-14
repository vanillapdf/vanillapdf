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
	CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Type(ObjectHandle handle, PObjectType result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_OBJECT_SCOPE(obj);

		switch (obj->GetType()) {
		case Object::Type::Array:
			*result = ObjectType_Array; break;
		case Object::Type::Boolean:
			*result = ObjectType_Boolean; break;
		case Object::Type::Dictionary:
			*result = ObjectType_Dictionary; break;
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
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, out_offset_type result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	LOG_WEAK_FILE_SCOPE(obj->GetFile());

	try
	{
		LOG_OBJECT_SCOPE(obj);

		*result = obj->GetOffset();
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	return ObjectRelease<Object, ObjectHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, PArrayHandle result)
{
	return SafeObjectConvert<Object, MixedArrayObject, ObjectHandle, ArrayHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle handle, PDictionaryHandle result)
{
	return SafeObjectConvert<Object, DictionaryObject, ObjectHandle, DictionaryHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle handle, PStreamHandle result)
{
	return SafeObjectConvert<Object, StreamObject, ObjectHandle, StreamHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle handle, PIndirectReferenceHandle result)
{
	return SafeObjectConvert<Object, IndirectObjectReference, ObjectHandle, IndirectReferenceHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle handle, PIntegerHandle result)
{
	return SafeObjectConvert<Object, IntegerObject, ObjectHandle, IntegerHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, PNameHandle result)
{
	return SafeObjectConvert<Object, NameObject, ObjectHandle, NameHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToBoolean(ObjectHandle handle, PBooleanHandle result)
{
	return SafeObjectConvert<Object, BooleanObject, ObjectHandle, BooleanHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToReal(ObjectHandle handle, PRealHandle result)
{
	return SafeObjectConvert<Object, RealObject, ObjectHandle, RealHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToNull(ObjectHandle handle, PNullHandle result)
{
	return SafeObjectConvert<Object, NullObject, ObjectHandle, NullHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle handle, PStringHandle result)
{
	return SafeObjectConvert<Object, StringObjectBase, ObjectHandle, StringHandle>(handle, result);
}
