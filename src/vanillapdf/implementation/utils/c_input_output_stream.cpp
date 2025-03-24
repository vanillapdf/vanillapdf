#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/input_output_stream_interface.h"

#include "vanillapdf/utils/c_input_output_stream.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_CreateFromFile(string_type filename, InputOutputStreamHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto input_stream = StreamUtils::InputOutputStreamFromFile(filename);
        auto ptr = input_stream.AddRefGet();
        *result = reinterpret_cast<InputOutputStreamHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_CreateFromMemory(InputOutputStreamHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto input_stream = StreamUtils::InputOutputStreamFromMemory();
        auto ptr = input_stream.AddRefGet();
        *result = reinterpret_cast<InputOutputStreamHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_Read(InputOutputStreamHandle* handle, bigint_type length, char* result, bigint_type* read_length) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(read_length);

    try {
        *read_length = stream->Read(result, length);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ReadBuffer(InputOutputStreamHandle* handle, bigint_type length, BufferHandle** result) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto buffer = stream->Read(length);
        auto ptr = buffer.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_GetInputPosition(InputOutputStreamHandle* handle, offset_type* result) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = stream->GetInputPosition();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_SetInputPosition(InputOutputStreamHandle* handle, offset_type value) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

    try {
        stream->SetInputPosition(value);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_GetOutputPosition(InputOutputStreamHandle* handle, offset_type* result) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = stream->GetOutputPosition();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_SetOutputPosition(InputOutputStreamHandle* handle, offset_type value) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

    try {
        stream->SetOutputPosition(value);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_WriteString(InputOutputStreamHandle* handle, string_type data) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    try {
        stream->Write(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_WriteBuffer(InputOutputStreamHandle* handle, BufferHandle* data_handle) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);
    Buffer* data = reinterpret_cast<Buffer*>(data_handle);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    try {
        stream->Write(*data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_Flush(InputOutputStreamHandle* handle) {
    IInputOutputStream* stream = reinterpret_cast<IInputOutputStream*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

    try {
        stream->Flush();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToInputStream(InputOutputStreamHandle* handle, InputStreamHandle** result) {
    return SafeObjectConvert<IInputOutputStream, IInputStream, InputOutputStreamHandle, InputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_FromInputStream(InputStreamHandle* handle, InputOutputStreamHandle** result) {
    return SafeObjectConvert<IInputStream, IInputOutputStream, InputStreamHandle, InputOutputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToOutputStream(InputOutputStreamHandle* handle, OutputStreamHandle** result) {
    return SafeObjectConvert<IInputOutputStream, IOutputStream, InputOutputStreamHandle, OutputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_FromOutputStream(OutputStreamHandle* handle, InputOutputStreamHandle** result) {
    return SafeObjectConvert<IOutputStream, IInputOutputStream, OutputStreamHandle, InputOutputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToUnknown(InputOutputStreamHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<IInputOutputStream, IUnknown, InputOutputStreamHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_FromUnknown(IUnknownHandle* handle, InputOutputStreamHandle** result) {
    return SafeObjectConvert<IUnknown, IInputOutputStream, IUnknownHandle, InputOutputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_Release(InputOutputStreamHandle* handle) {
    return ObjectRelease<IInputOutputStream, InputOutputStreamHandle>(handle);
}
