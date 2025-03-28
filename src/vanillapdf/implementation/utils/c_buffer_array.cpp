#include "precompiled.h"

#include "utils/buffer_array.h"

#include "vanillapdf/utils/c_buffer_array.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Create(BufferArrayHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto buffer = make_deferred_container<BufferArray>();
        auto ptr = buffer.AddRefGet();
        *result = reinterpret_cast<BufferArrayHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_GetSize(BufferArrayHandle* handle, size_type* result) {
    BufferArray* obj = reinterpret_cast<BufferArray*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetSize();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_At(BufferArrayHandle* handle, size_type at, BufferHandle** result) {
    BufferArray* obj = reinterpret_cast<BufferArray*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto value = obj->At(at);
        auto ptr = value.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Append(BufferArrayHandle* handle, BufferHandle* value) {
    BufferArray* obj = reinterpret_cast<BufferArray*>(handle);
    Buffer* item = reinterpret_cast<Buffer*>(value);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);

    try {
        obj->Append(item);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Insert(BufferArrayHandle* handle, size_type at, BufferHandle* value) {
    BufferArray* obj = reinterpret_cast<BufferArray*>(handle);
    Buffer* item = reinterpret_cast<Buffer*>(value);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);

    try {
        obj->Insert(item, at);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Remove(BufferArrayHandle* handle, size_type at) {
    BufferArray* obj = reinterpret_cast<BufferArray*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try {
        bool removed = obj->Remove(at);
        if (!removed) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_ToUnknown(BufferArrayHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<BufferArray, IUnknown, BufferArrayHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_FromUnknown(IUnknownHandle* handle, BufferArrayHandle** result) {
    return SafeObjectConvert<IUnknown, BufferArray, IUnknownHandle, BufferArrayHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION BufferArray_Release(BufferArrayHandle* handle) {
    return ObjectRelease<BufferArray, BufferArrayHandle>(handle);
}
