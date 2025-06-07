#include "precompiled.h"

#include "utils/streams/memory_buffer_output_stream.h"

#include "vanillapdf/utils/c_memory_buffer_output_stream.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_Create(MemoryBufferOutputStreamHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto output_stream = make_deferred<MemoryBufferOutputStream>();
        auto ptr = output_stream.AddRefGet();
        *result = reinterpret_cast<MemoryBufferOutputStreamHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_GetOutputPosition(MemoryBufferOutputStreamHandle* handle, offset_type* result) {
    MemoryBufferOutputStream* stream = reinterpret_cast<MemoryBufferOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = stream->GetOutputPosition();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_SetOutputPosition(MemoryBufferOutputStreamHandle* handle, offset_type value) {
    MemoryBufferOutputStream* stream = reinterpret_cast<MemoryBufferOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

    try {
        stream->SetOutputPosition(value);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_WriteString(MemoryBufferOutputStreamHandle* handle, string_type data) {
    MemoryBufferOutputStream* stream = reinterpret_cast<MemoryBufferOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    try {
        stream->Write(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_WriteBuffer(MemoryBufferOutputStreamHandle* handle, BufferHandle* data_handle) {
    MemoryBufferOutputStream* stream = reinterpret_cast<MemoryBufferOutputStream*>(handle);
    Buffer* data = reinterpret_cast<Buffer*>(data_handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    try {
        stream->Write(*data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_WriteBufferRange(MemoryBufferOutputStreamHandle* handle, BufferHandle* data_handle, offset_type length) {
    MemoryBufferOutputStream* stream = reinterpret_cast<MemoryBufferOutputStream*>(handle);
    Buffer* data = reinterpret_cast<Buffer*>(data_handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    try {
        stream->Write(*data, length);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_Flush(MemoryBufferOutputStreamHandle* handle) {
    MemoryBufferOutputStream* stream = reinterpret_cast<MemoryBufferOutputStream*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

    try {
        stream->Flush();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_ToUnknown(MemoryBufferOutputStreamHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<MemoryBufferOutputStream, IUnknown, MemoryBufferOutputStreamHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_FromUnknown(IUnknownHandle* handle, MemoryBufferOutputStreamHandle** result) {
    return SafeObjectConvert<IUnknown, MemoryBufferOutputStream, IUnknownHandle, MemoryBufferOutputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION MemoryBufferOutputStream_Release(MemoryBufferOutputStreamHandle* handle) {
    return ObjectRelease<MemoryBufferOutputStream, MemoryBufferOutputStreamHandle>(handle);
}
