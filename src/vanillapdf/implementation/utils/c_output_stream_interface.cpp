#include "precompiled.h"

#include "utils/streams/output_stream.h"
#include "utils/streams/stream_utils.h"

#include "vanillapdf/utils/c_output_stream_interface.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION OutputStreamInterface_CreateFromFile(string_type filename, OutputStreamInterfaceHandle* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(filename);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto output_stream = StreamUtils::OutputStreamFromFile(filename);
		auto ptr = output_stream.AddRefGet();
		*result = reinterpret_cast<OutputStreamInterfaceHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStreamInterface_WriteString(OutputStreamInterfaceHandle handle, string_type data) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try {
		stream->Write(data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStreamInterface_WriteBuffer(OutputStreamInterfaceHandle handle, BufferHandle data_handle) {
	IOutputStream* stream = reinterpret_cast<IOutputStream*>(handle);
	Buffer* data = reinterpret_cast<Buffer*>(data_handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(stream);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try {
		stream->Write(*data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION OutputStreamInterface_Release(OutputStreamInterfaceHandle handle) {
	return ObjectRelease<IOutputStream, OutputStreamInterfaceHandle>(handle);
}
