#include "precompiled.h"

#include "syntax/objects/boolean_object.h"

#include "vanillapdf/syntax/c_boolean_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_Create(BooleanObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<BooleanObject>();
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<BooleanObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_GetValue(BooleanObjectHandle* handle, boolean_type* result)
{
	BooleanObject* obj = reinterpret_cast<BooleanObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetValue();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_SetValue(BooleanObjectHandle* handle, boolean_type value)
{
	BooleanObject* obj = reinterpret_cast<BooleanObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	if (value == VANILLAPDF_RV_TRUE)
		obj->SetValue(true);
	else if (value == VANILLAPDF_RV_FALSE)
		obj->SetValue(false);
	else
		return VANILLAPDF_ERROR_GENERAL;

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_Release(BooleanObjectHandle* handle)
{
	return ObjectRelease<BooleanObject, BooleanObjectHandle>(handle);
}
