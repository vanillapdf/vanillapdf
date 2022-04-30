#include "precompiled.h"

#include "syntax/objects/integer_object.h"

#include "vanillapdf/syntax/c_integer_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_Create(IntegerObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<IntegerObject>();
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_GetIntegerValue(IntegerObjectHandle* handle, bigint_type* result)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetIntegerValue();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_GetUnsignedIntegerValue(IntegerObjectHandle* handle, biguint_type* result)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = obj->GetUnsignedIntegerValue();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_SetIntegerValue(IntegerObjectHandle* handle, bigint_type value)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetValue(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_SetUnsignedIntegerValue(IntegerObjectHandle* handle, biguint_type value)
{
	IntegerObject* obj = reinterpret_cast<IntegerObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		obj->SetValue(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_ToObject(IntegerObjectHandle* handle, ObjectHandle** result) {
	return SafeObjectConvert<IntegerObject, Object, IntegerObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_FromObject(ObjectHandle* handle, IntegerObjectHandle** result) {
	Object* obj = reinterpret_cast<Object*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		bool converted = false;
		auto integer = ConversionHelper<IntegerObjectPtr>::Get(obj, converted);
		if (!converted) {
			return VANILLAPDF_ERROR_PARAMETER_VALUE;
		}

		auto ptr = integer.AddRefGet();
		*result = reinterpret_cast<IntegerObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle* handle)
{
	return ObjectRelease<IntegerObject, IntegerObjectHandle>(handle);
}
