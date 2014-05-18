#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "object_reference_wrapper.h"
#include "token.h"

#include <vector>

namespace gotchangpdf
{
	/*
	class ArrayObjectBase : public Object
	{
	public:
		ArrayObjectBase() : Object(Object::Type::ArrayObject) {};

		static ObjectReferenceWrapper<ArrayObjectBase> ReadArray(lexical::Parser& s);

		virtual ~ArrayObjectBase() = 0;
	};
	*/
	template <typename T>
	class ArrayObject : public Object
	{
	public:
		ArrayObject() : Object(Object::Type::ArrayObject) {};

		inline int Size(void) const { return _list.size(); }
		inline ObjectReferenceWrapper<T> operator[](unsigned int i) const { return _list[i]; }
		inline ObjectReferenceWrapper<T> At(unsigned int at) const { return _list.at(at); }

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject<T>& o)
		{
			s.LexicalSettingsPush();
			auto settings = s.LexicalSettingsGet();
			settings->skip.push_back(lexical::Token::Type::EOL);

			if (s.PeekTokenType() == lexical::Token::Type::ARRAY_BEGIN)
				s.ReadToken();

			while (s.PeekTokenType() != lexical::Token::Type::ARRAY_END)
			{
				auto val = s.readObjectWithType<T>();
				o._list.push_back(val);
			}

			s.ReadTokenWithType(lexical::Token::Type::ARRAY_END);

			s.LexicalSettingsPop();
			return s;
		}

		//virtual ~ArrayObject() {};

	protected:
		std::vector<ObjectReferenceWrapper<T>> _list;

		//ArrayObject(std::vector<ObjectReferenceWrapper<T>> list) : _list(std::move(list)) {}
		//friend class ArrayObjectBase;
	};

	class MixedArrayObject : public ArrayObject<Object>
	{
	public:
		//MixedArrayObject() : ArrayObject() {}
		//using ArrayObject::ArrayObject;
	private:
		//MixedArrayObject(std::vector<ObjectReferenceWrapper<Object>> list) : ArrayObject(std::move(list)) {}
		//friend class ArrayObjectBase;
	};

	/*
	class ArrayObject : public Object
	{
	public:
		//ArrayObject() : Object(Object::Type::ArrayObject) {};

		static ObjectReferenceWrapper<ArrayObject> ReadArray(lexical::Parser& s)
		{
			std::vector<ObjectReferenceWrapper<Object>> list;

			s.LexicalSettingsPush();
			auto settings = s.LexicalSettingsGet();
			settings->skip.push_back(lexical::Token::Type::EOL);

			if (s.PeekTokenType() == lexical::Token::Type::ARRAY_BEGIN)
				s.ReadToken();

			auto type = s.peekObject()->GetType();
			bool is_single = true;

			while (s.PeekTokenType() != lexical::Token::Type::ARRAY_END)
			{
				auto val = s.readObject();
				list.push_back(val);

				is_single &= val->GetType() == type;
			}

			s.ReadTokenWithType(lexical::Token::Type::ARRAY_END);

			s.LexicalSettingsPop();
		}

		inline int Size(void) const { return _list.size(); }
		inline ObjectReferenceWrapper<Object> operator[](unsigned int i) const { return _list[i]; }
		inline ObjectReferenceWrapper<Object> At(unsigned int at) const { return _list.at(at); }

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject& o)
		{
			s.LexicalSettingsPush();
			auto settings = s.LexicalSettingsGet();
			settings->skip.push_back(lexical::Token::Type::EOL);

			if (s.PeekTokenType() == lexical::Token::Type::ARRAY_BEGIN)
				s.ReadToken();

			while (s.PeekTokenType() != lexical::Token::Type::ARRAY_END)
			{
				auto val = s.readObjectWithType<T>();
				o._list.push_back(val);
			}

			s.ReadTokenWithType(lexical::Token::Type::ARRAY_END);

			s.LexicalSettingsPop();
			return s;
		}

	protected:
		std::vector<ObjectReferenceWrapper<Object>> _list;
	};

	template <typename T>
	class SpecializedArrayObject : public ArrayObject
	{
		inline ObjectReferenceWrapper<T> operator[](unsigned int i) const { return _list[i]; }
		inline ObjectReferenceWrapper<T> At(unsigned int at) const { return _list.at(at); }
	};
	*/
}

#endif /* _ARRAY_OBJECT_H */
