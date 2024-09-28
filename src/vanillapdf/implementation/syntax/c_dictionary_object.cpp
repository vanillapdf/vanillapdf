#include "precompiled.h"

#include "syntax/objects/dictionary_object.h"

#include "vanillapdf/syntax/c_dictionary_object.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Create(DictionaryObjectHandle** result) {
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = make_deferred<DictionaryObject>();
		auto base = ObjectUtils::GetObjectBase(direct);
		auto ptr = base.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_GetSize(DictionaryObjectHandle* handle, size_type* result) {
	DictionaryObject* obj = reinterpret_cast<DictionaryObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = obj->GetSize();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle* handle, const NameObjectHandle* key, ObjectHandle** result)
{
	DictionaryObject* obj = reinterpret_cast<DictionaryObject*>(handle);
	const NameObject* name_object = reinterpret_cast<const NameObject*>(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name_object);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = obj->Find(*name_object);
		auto base = ObjectUtils::GetObjectBase(direct);
		auto ptr = base.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_TryFind(DictionaryObjectHandle* handle, const NameObjectHandle* key, boolean_type* result, ObjectHandle** result_object) {
	DictionaryObject* obj = reinterpret_cast<DictionaryObject*>(handle);
	const NameObject* name_object = reinterpret_cast<const NameObject*>(key);

	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name_object);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result_object);

	try
	{
		OutputContainableObjectPtr found_object;

		auto found = obj->TryFind(*name_object, found_object);
		if (!found) {
			*result_object = nullptr;
			*result = false;
			return VANILLAPDF_ERROR_SUCCESS;
		}

		ContainableObjectPtr found_containable = found_object;
		auto base = ObjectUtils::GetObjectBase(found_containable);
		auto ptr = base.AddRefGet();

		*result_object = reinterpret_cast<ObjectHandle*>(ptr);
		*result = true;

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_GetIterator(DictionaryObjectHandle* handle, DictionaryObjectIteratorHandle** result)
{
	DictionaryObject* obj = reinterpret_cast<DictionaryObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto begin = make_deferred<DictionaryObject::Iterator>(obj->begin(), obj->end());
		auto ptr = begin.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectIteratorHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Next(
	DictionaryObjectIteratorHandle* handle)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	try
	{
		++(*iterator);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_ToUnknown(DictionaryObjectIteratorHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<DictionaryObject::Iterator, IUnknown, DictionaryObjectIteratorHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_FromUnknown(IUnknownHandle* handle, DictionaryObjectIteratorHandle** result) {
	return SafeObjectConvert<IUnknown, DictionaryObject::Iterator, IUnknownHandle, DictionaryObjectIteratorHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Release(
	DictionaryObjectIteratorHandle* handle)
{
	return ObjectRelease<DictionaryObject::Iterator, DictionaryObjectIteratorHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetKey(
	DictionaryObjectIteratorHandle* handle,
	NameObjectHandle** result)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		NameObjectPtr name = iterator->First();
		NameObject* ptr = name.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetValue(
	DictionaryObjectIteratorHandle* handle,
	ObjectHandle** result)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = iterator->Second();
		auto base = ObjectUtils::GetObjectBase(direct);
		auto ptr = base.AddRefGet();
		*result = reinterpret_cast<ObjectHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryObjectIteratorHandle* handle, boolean_type* result)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		if (iterator->IsValid()) {
			*result = VANILLAPDF_RV_TRUE;
		}
		else {
			*result = VANILLAPDF_RV_FALSE;
		}

		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Remove(DictionaryObjectHandle* dictionary_handle, const NameObjectHandle* key, boolean_type* result)
{
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	const NameObject* name = reinterpret_cast<const NameObject*>(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);

	try
	{
		*result = dictionary->Remove(*name);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Clear(DictionaryObjectHandle* handle) {
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);

	try {
		dictionary->Clear();
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Insert(DictionaryObjectHandle* dictionary_handle, NameObjectHandle* key, ObjectHandle* value, boolean_type overwrite)
{
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	NameObject* name = reinterpret_cast<NameObject*>(key);
	Object* obj = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		auto containable_ptr = static_cast<ContainableObject*>(obj);
		if (containable_ptr == nullptr) {
			return VANILLAPDF_ERROR_PARAMETER_VALUE;
		}

		dictionary->Insert(name, containable_ptr, overwrite);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_InsertConst(DictionaryObjectHandle* dictionary_handle, const NameObjectHandle* key, ObjectHandle* value, boolean_type overwrite) {
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	const NameObject* name = reinterpret_cast<const NameObject*>(key);
	Object* obj = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try {
		auto containable_ptr = static_cast<ContainableObject*>(obj);
		if (containable_ptr == nullptr) {
			return VANILLAPDF_ERROR_PARAMETER_VALUE;
		}

		dictionary->Insert(*name, containable_ptr, overwrite);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Contains(DictionaryObjectHandle* dictionary_handle, const NameObjectHandle* key, boolean_type* result)
{
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	const NameObject* name = reinterpret_cast<const NameObject*>(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = dictionary->Contains(*name);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_ToObject(DictionaryObjectHandle* handle, ObjectHandle** result) {
	return SafeObjectConvert<DictionaryObject, Object, DictionaryObjectHandle, ObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_FromObject(ObjectHandle* handle, DictionaryObjectHandle** result) {
	return SafeObjectConvert<Object, DictionaryObject, ObjectHandle, DictionaryObjectHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle* handle)
{
	return ObjectRelease<DictionaryObject, DictionaryObjectHandle>(handle);
}
