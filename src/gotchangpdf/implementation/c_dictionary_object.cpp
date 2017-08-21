#include "precompiled.h"

#include "syntax/objects/dictionary_object.h"

#include "gotchangpdf/c_dictionary_object.h"
#include "gotchangpdf/c_values.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, const struct NameObjectHandleTag* key, ObjectHandle* result)
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
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle handle, DictionaryObjectIteratorHandle* result)
{
	DictionaryObject* obj = reinterpret_cast<DictionaryObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto begin = make_deferred<DictionaryObject::Iterator>(obj->begin());
		auto ptr = begin.AddRefGet();
		*result = reinterpret_cast<DictionaryObjectIteratorHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Next(
	DictionaryObjectIteratorHandle handle)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	try
	{
		++(*iterator);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Release(
	DictionaryObjectIteratorHandle handle)
{
	return ObjectRelease<DictionaryObject::Iterator, DictionaryObjectIteratorHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetKey(
	DictionaryObjectIteratorHandle handle,
	NameObjectHandle* result)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		NameObjectPtr name = iterator->First();
		NameObject* ptr = name.AddRefGet();
		*result = reinterpret_cast<NameObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetValue(
	DictionaryObjectIteratorHandle handle,
	ObjectHandle* result)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		auto direct = iterator->Second();
		auto base = ObjectUtils::GetObjectBase(direct);
		auto ptr = base.AddRefGet();
		*result = reinterpret_cast<ObjectHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_IsValid(
	DictionaryObjectIteratorHandle iterator_handle,
	DictionaryObjectHandle dictionary_handle,
	boolean_type* result)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(iterator_handle);
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		if (dictionary->end() == iterator->RawIterator()) {
			*result = GOTCHANG_PDF_RV_FALSE;
		}
		else {
			*result = GOTCHANG_PDF_RV_TRUE;
		}

		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Remove(DictionaryObjectHandle dictionary_handle, const struct NameObjectHandleTag* key)
{
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	const NameObject* name = reinterpret_cast<const NameObject*>(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);

	try
	{
		dictionary->Remove(*name);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Insert(DictionaryObjectHandle dictionary_handle, NameObjectHandle key, ObjectHandle value)
{
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	NameObject* name = reinterpret_cast<NameObject*>(key);
	Object* obj = reinterpret_cast<Object*>(value);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

	try
	{
		auto containable_ptr = dynamic_cast<ContainableObject*>(obj);
		if (nullptr == containable_ptr)
			return GOTCHANG_PDF_ERROR_PARAMETER_VALUE;

		dictionary->Insert(name, containable_ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Contains(DictionaryObjectHandle dictionary_handle, const struct NameObjectHandleTag* key, boolean_type* result)
{
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	const NameObject* name = reinterpret_cast<const NameObject*>(key);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		*result = dictionary->Contains(*name);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle handle)
{
	return ObjectRelease<DictionaryObject, DictionaryObjectHandle>(handle);
}
