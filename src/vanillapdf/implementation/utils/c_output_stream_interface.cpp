#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/output_stream_interface.h"

#include "vanillapdf/utils/c_output_stream_interface.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_CreateFromFile(string_type filename, IOutputStreamHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto output_stream = StreamUtils::OutputStreamFromFile(filename);
		auto ptr = output_stream.AddRefGet();
		*result = reinterpret_cast<IOutputStreamHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_GetOutputPosition(IOutputStreamHandle* handle, offset_type* result) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = stream->GetOutputPosition();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_SetOutputPosition(IOutputStreamHandle* handle, offset_type value) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

	try {
		stream->SetOutputPosition(value);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_WriteString(IOutputStreamHandle* handle, string_type data) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try {
		stream->Write(data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_WriteBuffer(IOutputStreamHandle* handle, BufferHandle* data_handle) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try {
		stream->Write(*data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_Flush(IOutputStreamHandle* handle) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);

	try {
		stream->Flush();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IOutputStream_Release(IOutputStreamHandle* handle) {
	return ObjectRelease<IOutputStream, IOutputStreamHandle>(handle);
}
