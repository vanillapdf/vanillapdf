#include "dictionary_object.h"
#include "exception.h"
#include "array_object.h"
#include "string_object.h"
#include "real_object.h"
#include "integer_object.h"
#include "indirect_object_reference.h"
#include "parser.h"

#include "c_dictionary_object.h"
#include "c_values.h"

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
		/*
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
			val->SetContainer(&o);
		}

		s.ReadTokenWithType(Token::Type::DICTIONARY_END);

		s.LexicalSettingsPop();
		*/
		return s;
	}
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, const char *str, int len)
{
	auto dictionary = reinterpret_cast<gotchangpdf::DictionaryObject*>(handle);
	gotchangpdf::Buffer set(str, len);
	gotchangpdf::NameObject name(set);
	gotchangpdf::ObjectBaseVisitor visitor;
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
	auto result = gotchangpdf::Deferred<gotchangpdf::NameObject>(gotchangpdf::NameObject(iterator->First()));
	return reinterpret_cast<NameObjectHandle>(AddRefGet(result));
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObjectIterator_GetValue(DictionaryObjectIteratorHandle handle)
{
	gotchangpdf::DictionaryObject::Iterator* iterator = reinterpret_cast<gotchangpdf::DictionaryObject::Iterator*>(handle);
	gotchangpdf::ObjectBaseVisitor visitor;
	gotchangpdf::Object* ptr = iterator->Second().apply_visitor(visitor).AddRefGet();
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
