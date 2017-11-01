#include "precompiled.h"

#include "vanillapdf/c_buffer.h"
#include "c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Create(string_type data, size_type size, BufferHandle* result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto buffer = make_deferred<Buffer>(data, size);
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_GetData(BufferHandle handle, string_type* data, size_type* size)
{
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(size);

	*data = obj->data();
	*size = obj->size();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_SetData(BufferHandle handle, string_type data, size_type size)
{
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		obj->assign(data, data + size);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_ToInputStream(BufferHandle handle, InputStreamInterfaceHandle* result) {
	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto input_stream = obj->ToInputStream();
		auto ptr = input_stream.AddRefGet();
		*result = reinterpret_cast<InputStreamInterfaceHandle>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Release(BufferHandle handle)
{
	return ObjectRelease<Buffer, BufferHandle>(handle);
}
