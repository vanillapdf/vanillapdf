#include "DictionaryObject.h"
#include "Exception.h"
#include "ArrayObject.h"
#include "StringObject.h"
#include "RealObject.h"
#include "IntegerObject.h"
#include "IndirectObjectReference.h"
#include "Export.h"
#include "Lexical/Parser.h"

#include <cassert>

namespace Pdf
{
	using namespace Lexical;
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

	Lexical::Parser& operator>>(Lexical::Parser& s, DictionaryObject& o)
	{
		auto token = s.PeekToken();

		if (token->type() == Token::Type::DICTIONARY_BEGIN)
		{
			s.ReadToken();
			token = s.PeekToken();
		}

		if (s.PeekToken()->type() == Token::Type::EOL)
		{
			s.ReadToken();
			token = s.PeekToken();
		}

		while (token->type() != Token::Type::DICTIONARY_END)
		{
			auto name = s.readObjectWithType<NameObject>();
			auto val = s.readObject();

			o._list[*name] = val;

			while (s.PeekToken()->type() == Token::Type::EOL)
				s.ReadToken();

			token = s.PeekToken();
		}

		s.ReadTokenWithType(Token::Type::DICTIONARY_END);
		if (s.PeekToken()->type() == Token::Type::EOL)
			s.ReadToken();

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
		Pdf::Object* ptr = found->second.get();
		boost::intrusive_ptr_add_ref(ptr);
		return ptr;
	}

	DictionaryObject::listType::const_iterator DictionaryObject::Begin() const { return _list.begin(); }
	DictionaryObject::listType::const_iterator DictionaryObject::End() const { return _list.end();}
}

typedef std::pair<Pdf::NameObject, boost::intrusive_ptr<Pdf::Object>> DictionaryObjectPair;
typedef Pdf::DictionaryObject::listType::const_iterator DictionaryObjectConstIterator;

GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION DictionaryObject_Find(DictionaryObjectHandle handle, const char *str, int len)
{
	Pdf::DictionaryObject* dictionary = reinterpret_cast<Pdf::DictionaryObject*>(handle);
	Pdf::Buffer set(str, len);
	Pdf::NameObject name(set);
	boost::intrusive_ptr<Pdf::Object> object = dictionary->Find(name);
	Pdf::Object* ptr = object.get();
	boost::intrusive_ptr_add_ref(ptr);

	return reinterpret_cast<ObjectHandle>(ptr);
}

GOTCHANG_PDF_API DictionaryObjectIteratorHandle CALLING_CONVENTION DictionaryObject_Iterator(DictionaryObjectHandle handle)
{
	Pdf::DictionaryObject* dictionary = reinterpret_cast<Pdf::DictionaryObject*>(handle);
	auto begin = dictionary->Begin();
	auto result = new DictionaryObjectConstIterator(begin);
	return reinterpret_cast<DictionaryObjectIteratorHandle>(result);
}

GOTCHANG_PDF_API DictionaryObjectPairHandle CALLING_CONVENTION DictionaryObjectIterator_Next(DictionaryObjectIteratorHandle handle, DictionaryObjectHandle dictionaryHandle)
{
	Pdf::DictionaryObject* dictionary = reinterpret_cast<Pdf::DictionaryObject*>(dictionaryHandle);
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

	boost::intrusive_ptr<Pdf::Object> object = pair->second;
	Pdf::Object* ptr = object.get();
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
	Pdf::DictionaryObject* obj = reinterpret_cast<Pdf::DictionaryObject*>(handle);
	obj->Release();
}
