#include "precompiled.h"
#include "file.h"

#include "c_dictionary_object.h"
#include "c_values.h"
#include "c_helper.h"

using namespace gotchangpdf;
using namespace gotchangpdf::syntax;

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Find(DictionaryHandle handle, NameHandle name, PObjectHandle result)
{
	DictionaryObject* obj = reinterpret_cast<DictionaryObject*>(handle);
	NameObject* name_object = reinterpret_cast<NameObject*>(name);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(name_object);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_OBJECT_SCOPE(obj);
		try
		{
			auto direct = obj->Find(*name_object);
			auto base = ObjectUtils::GetObjectBase(direct);
			auto ptr = base.AddRefGet();
			*result = reinterpret_cast<ObjectHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Iterator(DictionaryHandle handle, PDictionaryIteratorHandle result)
{
	DictionaryObject* obj = reinterpret_cast<DictionaryObject*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_OBJECT_SCOPE(obj);

		try
		{
			auto begin = DictionaryObject::IteratorPtr(obj->begin());
			auto ptr = begin.AddRefGet();
			*result = reinterpret_cast<DictionaryIteratorHandle>(ptr);
			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_Next(
	DictionaryIteratorHandle handle)
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
	DictionaryIteratorHandle handle)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

	iterator->Release();
	return GOTCHANG_PDF_ERROR_SUCCES;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetKey(
	DictionaryIteratorHandle handle,
	PNameHandle result)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		NameObjectPtr name = iterator->First();
		NameObject* ptr = name.AddRefGet();
		*result = reinterpret_cast<NameHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObjectIterator_GetValue(
	DictionaryIteratorHandle handle,
	PObjectHandle result)
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
	DictionaryIteratorHandle iterator_handle,
	DictionaryHandle dictionary_handle,
	out_boolean_type result)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(iterator_handle);
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dictionary_handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try
	{
		LOG_OBJECT_SCOPE(dictionary);
		try
		{
			if (dictionary->end() == iterator->Value())
				*result = GOTCHANG_PDF_FALSE;
			else
				*result = GOTCHANG_PDF_TRUE;

			return GOTCHANG_PDF_ERROR_SUCCES;
		} CATCH_GOTCHNGPDF_EXCEPTIONS
	} CATCH_SCOPE_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION DictionaryObject_Release(DictionaryHandle handle)
{
	return ObjectRelease<DictionaryObject, DictionaryHandle>(handle);
}
