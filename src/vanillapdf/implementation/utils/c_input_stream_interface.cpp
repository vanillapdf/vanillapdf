#include "precompiled.h"

#include "utils/streams/input_stream.h"
#include "utils/streams/stream_utils.h"

#include "vanillapdf/utils/c_input_stream_interface.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION IInputStream_CreateFromFile(string_type filename, IInputStreamHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto input_stream = StreamUtils::InputStreamFromFile(filename);
		auto ptr = input_stream.AddRefGet();
		*result = reinterpret_cast<IInputStreamHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IInputStream_CreateFromBuffer(BufferHandle* data_handle, IInputStreamHandle** result) {
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto input_stream = StreamUtils::InputStreamFromBuffer(data);
		auto ptr = input_stream.AddRefGet();
		*result = reinterpret_cast<IInputStreamHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IInputStream_ToBuffer(IInputStreamHandle* handle, BufferHandle** result) {
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

VANILLAPDF_API error_type CALLING_CONVENTION IInputStream_Release(IInputStreamHandle* handle) {
	return ObjectRelease<IInputStream, IInputStreamHandle>(handle);
}
