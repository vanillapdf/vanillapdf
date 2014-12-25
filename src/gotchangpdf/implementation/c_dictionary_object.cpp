#include "dictionary_object.h"
#include "objects.h"

#include "c_dictionary_object.h"
#include "c_values.h"

using namespace gotchangpdf;

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, const char *str, int len)
{
	auto dictionary = reinterpret_cast<gotchangpdf::DictionaryObject*>(handle);
	Buffer set(str, len);
	gotchangpdf::NameObject name(set);
	ObjectBaseVisitor visitor;
	auto object = dictionary->Find(name).apply_visitor(visitor);
	auto ptr = object.AddRefGet();
	//boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API DictionaryObjectIteratorHandle CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle handle)
{
	gotchangpdf::DictionaryObject* dictionary = reinterpret_cast<gotchangpdf::DictionaryObject*>(handle);
	gotchangpdf::DictionaryObject::Iterator* begin = dictionary->Begin().Clone();
	return reinterpret_cast<DictionaryObjectIteratorHandle>(begin);
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle handle)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	++(*iterator);
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle handle)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	delete iterator;;
}

GOTCHANG_PDF_API NameObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryObjectIteratorHandle handle)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	auto result = Deferred<gotchangpdf::NameObject>(gotchangpdf::NameObject(iterator->First()));
	return reinterpret_cast<NameObjectHandle>(AddRefGet(result));
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryObjectIteratorHandle handle)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	ObjectBaseVisitor visitor;
	Object* ptr = iterator->Second().apply_visitor(visitor).AddRefGet();
	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API int CALLING_CONVENTION DictionaryObjectIterator_IsValid(DictionaryObjectIteratorHandle handle, DictionaryObjectHandle dict)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	gotchangpdf::DictionaryObject* dictionary = reinterpret_cast<gotchangpdf::DictionaryObject*>(dict);

	if (dictionary->End() == *iterator)
		return GOTCHANG_PDF_RV_FALSE;
	else
		return GOTCHANG_PDF_RV_TRUE;
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle handle)
{
	gotchangpdf::DictionaryObject* obj = reinterpret_cast<gotchangpdf::DictionaryObject*>(handle);
	obj->Release();
}