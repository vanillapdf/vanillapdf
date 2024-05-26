#include "precompiled.h"

#include "vanillapdf/utils/c_buffer.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Create(BufferHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto buffer = make_deferred_container<Buffer>();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_CreateFromData(BufferHandle** result, string_type data, size_type size) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto buffer = make_deferred_container<Buffer>();
		buffer->assign(data, data + size);

		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_GetData(const BufferHandle* handle, string_type* data, size_type* size)
{
	const Buffer* obj = reinterpret_cast<const Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(size);

	*data = obj->data();
	*size = obj->size();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_SetData(BufferHandle* handle, string_type data, size_type size)
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

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_CopyTo(BufferHandle* handle, byte_array_type data, size_type size) {

	Buffer* obj = reinterpret_cast<Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

	try
	{
		if (size < 0 || obj->size() > size) {
			return VANILLAPDF_ERROR_INSUFFICIENT_SPACE;
		}

		std::copy(obj->data(), obj->data() + obj->size(), data);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_ToInputStream(const BufferHandle* handle, InputStreamHandle** result) {
	const Buffer* obj = reinterpret_cast<const Buffer*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto input_stream = obj->ToInputStream();
		auto ptr = input_stream.AddRefGet();
		*result = reinterpret_cast<InputStreamHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Equals(const BufferHandle* handle, const BufferHandle* other, boolean_type* result) {
	const Buffer* obj = reinterpret_cast<const Buffer*>(handle);
	const Buffer* other_obj = reinterpret_cast<const Buffer*>(other);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(other_obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = obj->Equals(*other_obj);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Hash(const BufferHandle* handle, size_type* result) {
	const Buffer* obj = reinterpret_cast<const Buffer*>(handle);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = obj->Hash();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_ToUnknown(BufferHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<Buffer, IUnknown, BufferHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_FromUnknown(IUnknownHandle* handle, BufferHandle** result) {
	return SafeObjectConvert<IUnknown, Buffer, IUnknownHandle, BufferHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Buffer_Release(BufferHandle* handle)
{
	return ObjectRelease<Buffer, BufferHandle>(handle);
}
