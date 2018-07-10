#include "precompiled.h"

#include "utils/streams/stream_utils.h"
#include "utils/streams/input_output_stream_interface.h"

#include "vanillapdf/utils/c_input_output_stream_interface.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION IInputOutputStream_CreateFromFile(string_type filename, IInputOutputStreamHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto input_stream = StreamUtils::InputOutputStreamFromFile(filename);
		auto ptr = input_stream.AddRefGet();
		*result = reinterpret_cast<IInputOutputStreamHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION IInputOutputStream_ToInputStream(IInputOutputStreamHandle* handle, IInputStreamHandle** result) {
	return SafeObjectConvert<IInputOutputStream, IInputStream, IInputOutputStreamHandle, IInputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION IInputOutputStream_ToOutputStream(IInputOutputStreamHandle* handle, IOutputStreamHandle** result) {
	return SafeObjectConvert<IInputOutputStream, IOutputStream, IInputOutputStreamHandle, IOutputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION IInputOutputStream_Release(IInputOutputStreamHandle* handle) {
	return ObjectRelease<IInputOutputStream, IInputOutputStreamHandle>(handle);
}
