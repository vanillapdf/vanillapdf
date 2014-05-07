#include "dictionary_object.h"
#include "exception.h"
#include "array_object.h"
#include "string_object.h"
#include "real_object.h"
#include "integer_object.h"
#include "indirect_object_reference.h"
#include "export.h"
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

			if (val->GetType() == IObject::Type::NullObject)
				continue;

			o._list[*name] = val;
		}

		s.ReadTokenWithType(Token::Type::DICTIONARY_END);

		s.LexicalSettingsPop();
		return s;
	}

	DictionaryObject::DictionaryObject() : Object(Object::Type::DictionaryObject) {}

	boost::intrusive_ptr<Object> DictionaryObject::Find(const NameObject& name) const
	{
		auto result = _list.find(name);
		return result->second;
	}

	IObject* DictionaryObject::IObjectFind(const INameObject& name) const
	{
		// TODO
		//auto result = _list.find(name);
		//return result->second;
		return nullptr;
	}

	IObject* DictionaryObject::IObjectFind(const char* name, int len) const
	{
		auto found = _list.find(NameObject(Buffer(name, len)));
		gotchangpdf::Object* ptr = found->second.get();
		boost::intrusive_ptr_add_ref(ptr);
		return ptr;
	}

	DictionaryObject::listType::const_iterator DictionaryObject::Begin() const { return _list.begin(); }
	DictionaryObject::listType::const_iterator DictionaryObject::End() const { return _list.end();}
}

typedef std::pair<gotchangpdf::NameObject, boost::intrusive_ptr<gotchangpdf::Object>> DictionaryObjectPair;
typedef gotchangpdf::DictionaryObject::listType::const_iterator DictionaryObjectConstIterator;

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, const char *str, int len)
{
	gotchangpdf::DictionaryObject* dictionary = reinterpret_cast<gotchangpdf::DictionaryObject*>(handle);
	gotchangpdf::Buffer set(str, len);
	gotchangpdf::NameObject name(set);
	boost::intrusive_ptr<gotchangpdf::Object> object = dictionary->Find(name);
	gotchangpdf::Object* ptr = object.get();
	boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API DictionaryObjectIteratorHandle CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle handle)
{
	gotchangpdf::DictionaryObject* dictionary = reinterpret_cast<gotchangpdf::DictionaryObject*>(handle);
	auto begin = dictionary->Begin();
	auto result = new DictionaryObjectConstIterator(begin);
	return reinterpret_cast<DictionaryObjectIteratorHandle>(result);
}

GOTCHANG_PDF_API DictionaryObjectPairHandle CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle handle, DictionaryObjectHandle dictionaryHandle)
{
	gotchangpdf::DictionaryObject* dictionary = reinterpret_cast<gotchangpdf::DictionaryObject*>(dictionaryHandle);
	DictionaryObjectConstIterator* iterator = reinterpret_cast<DictionaryObjectConstIterator*>(handle);
	(*iterator)++;

	if (*iterator == dictionary->End())
		return nullptr;

	DictionaryObjectPair* result = new DictionaryObjectPair((*iterator)->first, (*iterator)->second);
	return reinterpret_cast<DictionaryObjectPairHandle>(result);
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectIterator_Release(DictionaryObjectIteratorHandle handle)
{
	DictionaryObjectConstIterator* iterator = reinterpret_cast<DictionaryObjectConstIterator*>(handle);
	delete iterator;
}

GOTCHANG_PDF_API NameObjectHandle CALLING_CONVENTION DictionaryObjectPair_GetKey(DictionaryObjectPairHandle handle)
{
	DictionaryObjectPair* pair = reinterpret_cast<DictionaryObjectPair*>(handle);
	return reinterpret_cast<NameObjectHandle>(&pair->first);
}

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObjectPair_GetValue(DictionaryObjectPairHandle handle)
{
	DictionaryObjectPair* pair = reinterpret_cast<DictionaryObjectPair*>(handle);

	boost::intrusive_ptr<gotchangpdf::Object> object = pair->second;
	gotchangpdf::Object* ptr = object.get();
	boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API int CALLING_CONVENTION DictionaryObjectPair_IsValid(DictionaryObjectPairHandle handle)
{
	if (handle == nullptr)
		return GOTCHANG_PDF_RV_FALSE;
	else
		return GOTCHANG_PDF_RV_TRUE;
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObjectPair_Release(DictionaryObjectPairHandle handle)
{
	if (handle == nullptr)
		return;

	DictionaryObjectPair* pair = reinterpret_cast<DictionaryObjectPair*>(handle);
	delete pair;
}

GOTCHANG_PDF_API void CALLING_CONVENTION DictionaryObject_Release(DictionaryObjectHandle handle)
{
	gotchangpdf::DictionaryObject* obj = reinterpret_cast<gotchangpdf::DictionaryObject*>(handle);
	obj->Release();
}
