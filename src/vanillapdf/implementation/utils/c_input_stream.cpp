#include "precompiled.h"

#include "utils/streams/input_stream.h"
#include "utils/streams/stream_utils.h"

#include "vanillapdf/utils/c_input_stream.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_CreateFromFile(string_type filename, InputStreamHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto input_stream = StreamUtils::InputStreamFromFile(filename);
		auto ptr = input_stream.AddRefGet();
		*result = reinterpret_cast<InputStreamHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_CreateFromBuffer(BufferHandle* data_handle, InputStreamHandle** result) {
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto input_stream = StreamUtils::InputStreamFromBuffer(data);
		auto ptr = input_stream.AddRefGet();
		*result = reinterpret_cast<InputStreamHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_Read(InputStreamHandle* handle, size_type length, char* result, size_type* read_length) {
	IInputStream* stream = reinterpret_cast<IInputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(read_length);

	try {
		*read_length = stream->Read(result, length);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_ReadBuffer(InputStreamHandle* handle, size_type length, BufferHandle** result) {
	IInputStream* stream = reinterpret_cast<IInputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto buffer = stream->Read(length);
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_ToBuffer(InputStreamHandle* handle, BufferHandle** result) {
	IInputStream* stream = reinterpret_cast<IInputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto buffer = StreamUtils::InputStreamToBuffer(stream);
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_GetInputPosition(InputStreamHandle* handle, offset_type* result) {
	IInputStream* stream = reinterpret_cast<IInputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = stream->GetInputPosition();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_SetInputPosition(InputStreamHandle* handle, offset_type value) {
	IInputStream* stream = reinterpret_cast<IInputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

	try {
		stream->SetInputPosition(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_ToUnknown(InputStreamHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<IInputStream, IUnknown, InputStreamHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_FromUnknown(IUnknownHandle* handle, InputStreamHandle** result) {
	return SafeObjectConvert<IUnknown, IInputStream, IUnknownHandle, InputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputStream_Release(InputStreamHandle* handle) {
	return ObjectRelease<IInputStream, InputStreamHandle>(handle);
}
