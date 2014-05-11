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

	ObjectReferenceWrapper<Object> DictionaryObject::Iterator::Second() const { return _it->second; }

	bool DictionaryObject::Iterator::operator==(const Iterator& other) const
	{
		return _it == other._it;
	}

	DictionaryObject::Iterator* DictionaryObject::Iterator::Clone() const
	{
		return new DictionaryObject::Iterator(_it);
	}

	DictionaryObject::~DictionaryObject() {}

	#pragma region DllInterface

	IObject* IDictionaryObject::Find(const INameObject& name) const
	{
		auto removed = const_cast<IDictionaryObject*>(this);
		auto obj = reinterpret_cast<DictionaryObject*>(removed);
		auto name_parsed = dynamic_cast<const NameObject*>(&name);

		auto result = obj->Find(*name_parsed);
		return result.AddRefGet();
	}

	template <typename T>
	T* IDictionaryObject::Find(const INameObject& name) const
	{
		auto removed = const_cast<IDictionaryObject*>(this);
		auto obj = reinterpret_cast<DictionaryObject*>(removed);
		auto name_parsed = dynamic_cast<const NameObject*>(&name);

		auto result = obj->Find<T>(*name_parsed);
		return result.AddRefGet();
	}

	IDictionaryObject::IIterator* IDictionaryObject::Begin(void) const
	{
		auto removed = const_cast<IDictionaryObject*>(this);
		auto obj = reinterpret_cast<DictionaryObject*>(removed);

		return obj->Begin().Clone();
	}

	IDictionaryObject::IIterator* IDictionaryObject::End(void) const
	{
		auto removed = const_cast<IDictionaryObject*>(this);
		auto obj = reinterpret_cast<DictionaryObject*>(removed);

		return obj->End().Clone();
	}

	IDictionaryObject::~IDictionaryObject() {};

	INameObject* IDictionaryObject::IIterator::First() const
	{
		auto removed = const_cast<IDictionaryObject::IIterator*>(this);
		auto obj = reinterpret_cast<DictionaryObject::Iterator*>(removed);

		return reinterpret_cast<INameObject*>(new NameObject(obj->First()));
	}

	IObject* IDictionaryObject::IIterator::Second() const
	{
		auto removed = const_cast<IDictionaryObject::IIterator*>(this);
		auto obj = reinterpret_cast<DictionaryObject::Iterator*>(removed);

		return reinterpret_cast<IObject*>(obj->Second().AddRefGet());
	}

	const IDictionaryObject::IIterator& IDictionaryObject::IIterator::operator++()
	{
		auto removed = const_cast<IDictionaryObject::IIterator*>(this);
		auto obj = reinterpret_cast<DictionaryObject::Iterator*>(removed);

		auto result = (*obj)++;
		return result;
	}

	const IDictionaryObject::IIterator IDictionaryObject::IIterator::operator++(int)
	{
		auto removed = const_cast<IDictionaryObject::IIterator*>(this);
		auto obj = reinterpret_cast<DictionaryObject::Iterator*>(removed);

		return ++(*obj);
	}

	bool IDictionaryObject::IIterator::operator== (const IDictionaryObject::IIterator& other) const
	{
		auto removed = const_cast<IDictionaryObject::IIterator*>(this);
		auto obj = reinterpret_cast<DictionaryObject::Iterator*>(removed);
		auto other_parsed = reinterpret_cast<const DictionaryObject::Iterator*>(&other);

		return *obj == *other_parsed;
	}

	IDictionaryObject::IIterator* IDictionaryObject::IIterator::Clone() const
	{
		auto removed = const_cast<IDictionaryObject::IIterator*>(this);
		auto obj = reinterpret_cast<DictionaryObject::Iterator*>(removed);

		return obj->Clone();
	}

	IDictionaryObject::IIterator::~IIterator() {}

	#pragma endregion
}
