#include "dictionary_object.h"
#include "objects.h"

#include "c_dictionary_object.h"
#include "c_values.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryHandle handle, const char *str, int len)
{
	auto dictionary = reinterpret_cast<DictionaryObject*>(handle);
	Buffer set(str, len);
	NameObject name(set);
	ObjectBaseVisitor visitor;
	auto object = dictionary->Find(name).apply_visitor(visitor);
	auto ptr = object.AddRefGet();
	//boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API DictionaryIteratorHandle CALLING_CONVENTION DictionaryObject_Iterator(DictionaryHandle handle)
{
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(handle);
	DictionaryObject::Iterator* begin = dictionary->Begin().Clone();
	return reinterpret_cast<DictionaryIteratorHandle>(begin);
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryIteratorHandle handle)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	++(*iterator);
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryIteratorHandle handle)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	delete iterator;
}

GOTCHANG_PDF_API NameHandle CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryIteratorHandle handle)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	NameObjectPtr name = iterator->First();
	NameObject* ptr = name.AddRefGet();
	return reinterpret_cast<NameHandle>(ptr);
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryIteratorHandle handle)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	ObjectBaseVisitor visitor;
	DirectObject direct = iterator->Second();
	Object* ptr = direct.apply_visitor(visitor).AddRefGet();
	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API int CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryIteratorHandle handle, DictionaryHandle dict)
{
	DictionaryObject::Iterator* iterator = reinterpret_cast<DictionaryObject::Iterator*>(handle);
	DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(dict);

	if (dictionary->End() == *iterator)
		return GOTCHANG_PDF_RV_FALSE;
	else
		return GOTCHANG_PDF_RV_TRUE;
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObject_Release(DictionaryHandle handle)
{
	DictionaryObject* obj = reinterpret_cast<DictionaryObject*>(handle);
	obj->Release();
}