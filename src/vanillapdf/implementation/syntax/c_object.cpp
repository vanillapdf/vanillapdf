#include "precompiled.h"

#include "syntax/objects/object.h"

#include "vanillapdf/syntax/c_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type Object_TypeName(ObjectType type, string_type* result)
{
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	try
	{
		auto value = static_cast<Object::Type>(type);
		*result = Object::TypeName(value);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Object_GetObjectType(ObjectHandle* handle, ObjectType* result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetObjectType()) {
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
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle* handle, offset_type* result)
{
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetOffset();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle* handle, BufferHandle** result) {
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		BufferPtr buffer = make_deferred_container<Buffer>(obj->ToString());
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Object_ToPdf(ObjectHandle* handle, BufferHandle** result) {
	Object* obj = reinterpret_cast<Object*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		BufferPtr buffer = make_deferred_container<Buffer>(obj->ToPdf());
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	}
	CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Object_ToUnknown(ObjectHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<Object, IUnknown, ObjectHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Object_FromUnknown(IUnknownHandle* handle, ObjectHandle** result) {
	return SafeObjectConvert<IUnknown, Object, IUnknownHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle* handle)
{
	return ObjectRelease<Object, ObjectHandle>(handle);
}
