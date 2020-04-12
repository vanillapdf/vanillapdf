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

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToInputStream(InputOutputStreamHandle* handle, InputStreamHandle** result) {
	return SafeObjectConvert<IInputOutputStream, IInputStream, InputOutputStreamHandle, InputStreamHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InputOutputStream_ToOutputStream(InputOutputStreamHandle* handle, OutputStreamHandle** result) {
	return SafeObjectConvert<IInputOutputStream, IOutputStream, InputOutputStreamHandle, OutputStreamHandle>(handle, result);
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
