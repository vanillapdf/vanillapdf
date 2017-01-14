#include "precompiled.h"
#include "file.h"

#include "gotchangpdf/c_object.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type Object_TypeName(ObjectType type, string_type* result)
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

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetType(ObjectHandle handle, ObjectType* result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

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
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, offset_type* result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetOffset();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle)
{
	return ObjectRelease<Object, ObjectHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, ArrayObjectHandle* result)
{
	return SafeObjectConvert<Object, MixedArrayObject, ObjectHandle, ArrayObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle handle, DictionaryObjectHandle* result)
{
	return SafeObjectConvert<Object, DictionaryObject, ObjectHandle, DictionaryObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle handle, StreamObjectHandle* result)
{
	return SafeObjectConvert<Object, StreamObject, ObjectHandle, StreamObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle handle, IndirectObjectReferenceHandle* result)
{
	return SafeObjectConvert<Object, IndirectObjectReference, ObjectHandle, IndirectObjectReferenceHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle handle, IntegerObjectHandle* result)
{
	return SafeObjectConvert<Object, IntegerObject, ObjectHandle, IntegerObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, NameObjectHandle* result)
{
	return SafeObjectConvert<Object, NameObject, ObjectHandle, NameObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToBoolean(ObjectHandle handle, BooleanObjectHandle* result)
{
	return SafeObjectConvert<Object, BooleanObject, ObjectHandle, BooleanObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToReal(ObjectHandle handle, RealObjectHandle* result)
{
	return SafeObjectConvert<Object, RealObject, ObjectHandle, RealObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToNull(ObjectHandle handle, NullObjectHandle* result)
{
	return SafeObjectConvert<Object, NullObject, ObjectHandle, NullObjectHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle handle, StringObjectHandle* result)
{
	return SafeObjectConvert<Object, StringObjectBase, ObjectHandle, StringObjectHandle>(handle, result);
}
