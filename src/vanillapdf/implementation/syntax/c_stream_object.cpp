#include "precompiled.h"

#include "syntax/objects/stream_object.h"

#include "vanillapdf/syntax/c_stream_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_Create(StreamObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<StreamObject>(true);
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<StreamObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_GetHeader(StreamObjectHandle* handle, DictionaryObjectHandle** result)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto header = obj->GetHeader();
		auto ptr = header.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_SetHeader(StreamObjectHandle* handle, DictionaryObjectHandle* value)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	DictionaryObject* header = reinterpret_cast<DictionaryObject*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(header);

	try
	{
		obj->SetHeader(header);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_GetBody(StreamObjectHandle* handle, BufferHandle** result)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto header = obj->GetBody();
		auto ptr = header.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_GetBodyRaw(StreamObjectHandle* handle, BufferHandle** result)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto body = obj->GetBodyRaw();
		auto ptr = body.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_SetBody(StreamObjectHandle* handle, BufferHandle* value)
{
	StreamObject* obj = reinterpret_cast<StreamObject*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetBody(buffer);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_ToObject(StreamObjectHandle* handle, ObjectHandle** result) {
	return SafeObjectConvert<StreamObject, Object, StreamObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_FromObject(ObjectHandle* handle, StreamObjectHandle** result) {
	return SafeObjectConvert<Object, StreamObject, ObjectHandle, StreamObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION StreamObject_Release(StreamObjectHandle* handle)
{
	return ObjectRelease<StreamObject, StreamObjectHandle>(handle);
}
