#include "precompiled.h"

#include "syntax/objects/indirect_reference_object.h"

#include "vanillapdf/syntax/c_indirect_reference_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_Create(IndirectReferenceObjectHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto object = make_deferred<IndirectReferenceObject>();
        auto ptr = object.AddRefGet();
        *result = reinterpret_cast<IndirectReferenceObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_GetReferencedObject(IndirectReferenceObjectHandle* handle, ObjectHandle** result)
{
    IndirectReferenceObject* obj = reinterpret_cast<IndirectReferenceObject*>(handle);
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

VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_SetReferencedObject(IndirectReferenceObjectHandle* handle, ObjectHandle* value) {
    IndirectReferenceObject* obj = reinterpret_cast<IndirectReferenceObject*>(handle);
    Object* value_object = reinterpret_cast<Object*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(value_object);

    try {
        obj->SetReferencedObject(value_object);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_GetReferencedObjectNumber(IndirectReferenceObjectHandle* handle, biguint_type* result)
{
    IndirectReferenceObject* obj = reinterpret_cast<IndirectReferenceObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetReferencedObjectNumber();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_GetReferencedGenerationNumber(IndirectReferenceObjectHandle* handle, ushort_type* result)
{
    IndirectReferenceObject* obj = reinterpret_cast<IndirectReferenceObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetReferencedGenerationNumber();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_ToObject(IndirectReferenceObjectHandle* handle, ObjectHandle** result) {
    return SafeObjectConvert<IndirectReferenceObject, Object, IndirectReferenceObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_FromObject(ObjectHandle* handle, IndirectReferenceObjectHandle** result) {
    return SafeObjectConvert<Object, IndirectReferenceObject, ObjectHandle, IndirectReferenceObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION IndirectReferenceObject_Release(IndirectReferenceObjectHandle* handle) {
    return ObjectRelease<IndirectReferenceObject, IndirectReferenceObjectHandle>(handle);
}
