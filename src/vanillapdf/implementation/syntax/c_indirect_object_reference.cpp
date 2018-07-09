#include "precompiled.h"

#include "syntax/objects/indirect_object_reference.h"

#include "vanillapdf/syntax/c_indirect_object_reference.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_Create(IndirectObjectReferenceHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<IndirectObjectReference>();
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<IndirectObjectReferenceHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_GetReferencedObject(IndirectObjectReferenceHandle* handle, ObjectHandle** result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->GetReferencedObject();
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_SetReferencedObject(IndirectObjectReferenceHandle* handle, ObjectHandle* value) {
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	Object* value_object = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(value_object);

	try {
		obj->SetReferencedObject(value_object);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_Release(IndirectObjectReferenceHandle* handle)
{
	return ObjectRelease<IndirectObjectReference, IndirectObjectReferenceHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_GetReferencedObjectNumber(IndirectObjectReferenceHandle* handle, biguint_type* result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetReferencedObjectNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectObjectReference_GetReferencedGenerationNumber(IndirectObjectReferenceHandle* handle, ushort_type* result)
{
	IndirectObjectReference* obj = reinterpret_cast<IndirectObjectReference*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetReferencedGenerationNumber();
	return VANILLAPDF_ERROR_SUCCESS;
}
