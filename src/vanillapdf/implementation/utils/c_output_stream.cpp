#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/output_stream_interface.h"

#include "vanillapdf/utils/c_output_stream.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_CreateFromFile(string_type filename, OutputStreamHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto output_stream = StreamUtils::OutputStreamFromFile(filename);
		auto ptr = output_stream.AddRefGet();
		*result = reinterpret_cast<OutputStreamHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_GetOutputPosition(OutputStreamHandle* handle, offset_type* result) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = stream->GetOutputPosition();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_SetOutputPosition(OutputStreamHandle* handle, offset_type value) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

	try {
		stream->SetOutputPosition(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_WriteString(OutputStreamHandle* handle, string_type data) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try {
		stream->Write(data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_WriteBuffer(OutputStreamHandle* handle, BufferHandle* data_handle) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try {
		stream->Write(*data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_Flush(OutputStreamHandle* handle) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

	try {
		stream->Flush();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_ToUnknown(OutputStreamHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<IOutputStream, IUnknown, OutputStreamHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_FromUnknown(IUnknownHandle* handle, OutputStreamHandle** result) {
	return SafeObjectConvert<IUnknown, IOutputStream, IUnknownHandle, OutputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStream_Release(OutputStreamHandle* handle) {
	return ObjectRelease<IOutputStream, OutputStreamHandle>(handle);
}
