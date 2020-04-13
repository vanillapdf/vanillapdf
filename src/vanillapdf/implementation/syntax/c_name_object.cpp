#include "precompiled.h"

#include "syntax/objects/name_object.h"

#include "vanillapdf/syntax/c_name_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Create(NameObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<NameObject>();
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NameObject_GetValue(const NameObjectHandle* handle, BufferHandle** result)
{
	const NameObject* obj = reinterpret_cast<const NameObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetValue();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NameObject_SetValue(NameObjectHandle* handle, BufferHandle* value)
{
	NameObject* obj = reinterpret_cast<NameObject*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetValue(buffer);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Equals(const NameObjectHandle* handle, const NameObjectHandle* other, boolean_type* result) {
	const NameObject* obj = reinterpret_cast<const NameObject*>(handle);
	const NameObject* other_obj = reinterpret_cast<const NameObject*>(other);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(other_obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = obj->Equals(*other_obj);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NameObject_ToObject(NameObjectHandle* handle, ObjectHandle** result) {
	return SafeObjectConvert<NameObject, Object, NameObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION NameObject_FromObject(ObjectHandle* handle, NameObjectHandle** result) {
	return SafeObjectConvert<Object, NameObject, ObjectHandle, NameObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Release(NameObjectHandle* handle)
{
	return ObjectRelease<NameObject, NameObjectHandle>(handle);
}
