#include "precompiled.h"

#include "syntax/objects/real_object.h"

#include "vanillapdf/syntax/c_real_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION RealObject_Create(RealObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<RealObject>();
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<RealObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION RealObject_CreateFromData(real_type data, integer_type precision, RealObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<RealObject>(data, precision);
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<RealObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION RealObject_GetValue(RealObjectHandle* handle, real_type* result)
{
	RealObject* obj = reinterpret_cast<RealObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetValue();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION RealObject_SetValue(RealObjectHandle* handle, real_type value)
{
	RealObject* obj = reinterpret_cast<RealObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetValue(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION RealObject_ToObject(RealObjectHandle* handle, ObjectHandle** result) {
	return SafeObjectConvert<RealObject, Object, RealObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION RealObject_FromObject(ObjectHandle* handle, RealObjectHandle** result) {
	Object* obj = reinterpret_cast<Object*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		bool converted = false;
		auto integer = ConversionHelper<RealObjectPtr>::Get(obj, converted);
		if (!converted) {
			return VANILLAPDF_ERROR_PARAMETER_VALUE;
		}

		auto ptr = integer.AddRefGet();
		*result = reinterpret_cast<RealObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION RealObject_Release(RealObjectHandle* handle) {
	return ObjectRelease<RealObject, RealObjectHandle>(handle);
}
