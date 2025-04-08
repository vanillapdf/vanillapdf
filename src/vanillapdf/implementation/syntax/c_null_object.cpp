#include "precompiled.h"

#include "syntax/objects/null_object.h"

#include "vanillapdf/syntax/c_null_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION NullObject_Create(NullObjectHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto object = NullObject::GetInstance();
        auto ptr = object.AddRefGet();
        *result = reinterpret_cast<NullObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NullObject_ToObject(NullObjectHandle* handle, ObjectHandle** result) {
    return SafeObjectConvert<NullObject, Object, NullObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION NullObject_FromObject(ObjectHandle* handle, NullObjectHandle** result) {
    return SafeObjectConvert<Object, NullObject, ObjectHandle, NullObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION NullObject_Release(NullObjectHandle* handle)
{
    return ObjectRelease<NullObject, NullObjectHandle>(handle);
}
