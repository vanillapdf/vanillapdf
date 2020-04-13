#include "precompiled.h"

#include "syntax/objects/string_object.h"

#include "vanillapdf/syntax/c_string_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION StringObject_Type(StringObjectHandle* handle, StringType* result)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetStringType()) {
	case StringObjectBase::StringType::Literal:
		*result = StringType_Literal; break;
	case StringObjectBase::StringType::Hexadecimal:
		*result = StringType_Hexadecimal; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION StringObject_GetValue(StringObjectHandle* handle, BufferHandle** result)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetValue();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StringObject_SetValue(StringObjectHandle* handle, BufferHandle* value)
{
	StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetValue(buffer);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION StringObject_ToObject(StringObjectHandle* handle, ObjectHandle** result) {
	return SafeObjectConvert<StringObjectBase, Object, StringObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION StringObject_FromObject(ObjectHandle* handle, StringObjectHandle** result) {
	return SafeObjectConvert<Object, StringObjectBase, ObjectHandle, StringObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION StringObject_Release(StringObjectHandle* handle) {
	return ObjectRelease<StringObjectBase, StringObjectHandle>(handle);
}

// Literal string object

VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_Create(LiteralStringObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<LiteralStringObject>();
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<LiteralStringObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_GetValue(LiteralStringObjectHandle* handle, BufferHandle** result)
{
	LiteralStringObject* obj = reinterpret_cast<LiteralStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetValue();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_SetValue(LiteralStringObjectHandle* handle, BufferHandle* value)
{
	LiteralStringObject* obj = reinterpret_cast<LiteralStringObject*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetValue(buffer);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_ToStringObject(LiteralStringObjectHandle* handle, StringObjectHandle** result) {
	return SafeObjectConvert<LiteralStringObject, StringObjectBase, LiteralStringObjectHandle, StringObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_FromStringObject(StringObjectHandle* handle, LiteralStringObjectHandle** result) {
	return SafeObjectConvert<StringObjectBase, LiteralStringObject, StringObjectHandle, LiteralStringObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringObjectHandle* handle)
{
	return ObjectRelease<LiteralStringObject, LiteralStringObjectHandle>(handle);
}

// Hexadecimal string object

VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Create(HexadecimalStringObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto object = make_deferred<HexadecimalStringObject>();
		auto ptr = object.AddRefGet();
		*result = reinterpret_cast<HexadecimalStringObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_GetValue(HexadecimalStringObjectHandle* handle, BufferHandle** result)
{
	HexadecimalStringObject* obj = reinterpret_cast<HexadecimalStringObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto buffer = obj->GetValue();
		auto ptr = buffer.AddRefGet();
		*result = reinterpret_cast<BufferHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_SetValue(HexadecimalStringObjectHandle* handle, BufferHandle* value)
{
	HexadecimalStringObject* obj = reinterpret_cast<HexadecimalStringObject*>(handle);
	Buffer* buffer = reinterpret_cast<Buffer*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(buffer);

	try
	{
		obj->SetValue(buffer);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}


VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_ToStringObject(HexadecimalStringObjectHandle* handle, StringObjectHandle** result) {
	return SafeObjectConvert<HexadecimalStringObject, StringObjectBase, HexadecimalStringObjectHandle, StringObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_FromStringObject(StringObjectHandle* handle, HexadecimalStringObjectHandle** result) {
	return SafeObjectConvert<StringObjectBase, HexadecimalStringObject, StringObjectHandle, HexadecimalStringObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringObjectHandle* handle)
{
	return ObjectRelease<HexadecimalStringObject, HexadecimalStringObjectHandle>(handle);
}
