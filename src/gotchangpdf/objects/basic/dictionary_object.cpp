#include "dictionary_object.h"
#include "exception.h"
#include "array_object.h"
#include "string_object.h"
#include "real_object.h"
#include "integer_object.h"
#include "indirect_object_reference.h"
#include "gotchangpdf.h"
#include "parser.h"

#include <cassert>

namespace gotchangpdf
{
	using namespace lexical;
	//using namespace std;

	/*
	ReverseStream& operator>>(ReverseStream& s, DictionaryObject& o)
	{
		auto token = s.readToken();

		while (*token != Token::Type::DICTIONARY_END)
		{
			s >> *token;
			if (*token != Token::Type::DICTIONARY_BEGIN)
			{
				Token name;
				s >> name;
			}
		}

		//o._value = result->value();
		return s;
	}
	*/

	lexical::Parser& operator>>(lexical::Parser& s, DictionaryObject& o)
	{
		s.LexicalSettingsPush();
		auto settings = s.LexicalSettingsGet();
		settings->skip.push_back(Token::Type::EOL);

		if (s.PeekTokenType() == Token::Type::DICTIONARY_BEGIN)
			s.ReadToken();

		while (s.PeekTokenType() != Token::Type::DICTIONARY_END)
		{
			auto name = s.readObjectWithType<NameObject>();
			auto val = s.readObject();

			if (val->GetType() == Object::Type::NullObject)
				continue;

			o._list[*name] = val;
		}

		s.ReadTokenWithType(Token::Type::DICTIONARY_END);

		s.LexicalSettingsPop();
		return s;
	}

	DictionaryObject::DictionaryObject() : Object(Object::Type::DictionaryObject) {}

	ObjectReferenceWrapper<Object> DictionaryObject::Find(const NameObject& name) const
	{
		auto result = _list.find(name);
		return result->second;
	}

	DictionaryObject::Iterator DictionaryObject::Begin(void) const
	{
		return DictionaryObject::Iterator(_list.begin());
	}

	DictionaryObject::Iterator DictionaryObject::End(void) const
	{
		return DictionaryObject::Iterator(_list.end());
	}

	DictionaryObject::Iterator::Iterator(listType::const_iterator it) : _it(it) {}
	NameObject DictionaryObject::Iterator::First() const { return _it->first; }
	ObjectReferenceWrapper<Object> DictionaryObject::Iterator::Second() const { return _it->second; }
	bool DictionaryObject::Iterator::operator==(const Iterator& other) const
	{
		return _it == other._it;
	}

	const DictionaryObject::Iterator& DictionaryObject::Iterator::operator++()
	{
		++_it;
		return *this;
	}

	const DictionaryObject::Iterator DictionaryObject::Iterator::operator++(int)
	{
		DictionaryObject::Iterator temp(_it);
		++_it;
		return temp;
	}

	DictionaryObject::Iterator* DictionaryObject::Iterator::Clone() const
	{
		return new DictionaryObject::Iterator(_it);
	}
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, const char *str, int len)
{
	gotchangpdf::DictionaryObject* dictionary = reinterpret_cast<gotchangpdf::DictionaryObject*>(handle);
	gotchangpdf::Buffer set(str, len);
	gotchangpdf::NameObject name(set);
	gotchangpdf::ObjectReferenceWrapper<gotchangpdf::Object> object = dictionary->Find(name);
	gotchangpdf::Object* ptr = object.AddRefGet();
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
	(*iterator)++;
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle handle)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	delete iterator;;
}

GOTCHANG_PDF_API NameObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetKey(DictionaryObjectIteratorHandle handle)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	auto result = gotchangpdf::ObjectReferenceWrapper<gotchangpdf::NameObject>(new gotchangpdf::NameObject(iterator->First()));
	return reinterpret_cast<NameObjectHandle>(result.AddRefGet());
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryObjectIteratorHandle handle)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	gotchangpdf::Object* ptr = iterator->Second().AddRefGet();
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
