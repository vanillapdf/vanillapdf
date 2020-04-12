#include "precompiled.h"

#include "syntax/objects/array_object.h"

#include "vanillapdf/syntax/c_array_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Create(ArrayObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto buffer = make_deferred<MixedArrayObject>();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<ArrayObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle* handle, size_type at, ObjectHandle** result)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->At(at);
		auto base = ObjectUtils::GetObjectBase(direct);
		auto ptr = base.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle* handle, size_type* result)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->Size();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Append(ArrayObjectHandle* handle, ObjectHandle* value)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	Object* data = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		auto containable_ptr = dynamic_cast<ContainableObject*>(data);
		if (nullptr == containable_ptr) {
			return VANILLAPDF_ERROR_PARAMETER_VALUE;
		}

		obj->Append(containable_ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Insert(ArrayObjectHandle* handle, size_type at, ObjectHandle* value)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	Object* data = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		auto containable_ptr = dynamic_cast<ContainableObject*>(data);
		if (nullptr == containable_ptr) {
			return VANILLAPDF_ERROR_PARAMETER_VALUE;
		}

		obj->Insert(containable_ptr, at);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Remove(ArrayObjectHandle* handle, size_type at)
{
	MixedArrayObject* obj = reinterpret_cast<MixedArrayObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		bool removed = obj->Remove(at);
		if (!removed) {
			return VANILLAPDF_ERROR_OBJECT_MISSING;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_ToObject(ArrayObjectHandle* handle, ObjectHandle** result) {
	return SafeObjectConvert<MixedArrayObject, Object, ArrayObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_FromObject(ObjectHandle* handle, ArrayObjectHandle** result) {
	return SafeObjectConvert<Object, MixedArrayObject, ObjectHandle, ArrayObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle* handle)
{
	return ObjectRelease<MixedArrayObject, ArrayObjectHandle>(handle);
}
