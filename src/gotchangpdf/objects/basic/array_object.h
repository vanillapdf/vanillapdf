#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "object_reference_wrapper.h"

#include <vector>
#include <algorithm>

namespace gotchangpdf
{
	template <typename T>
	class ArrayObject : public Object
	{
	public:
		ArrayObject() {}
		explicit ArrayObject(std::vector<ObjectReferenceWrapper<T>> list) : _list(list) {}

		inline int Size(void) const { return _list.size(); }
		inline ObjectReferenceWrapper<T> operator[](unsigned int i) const { return _list[i]; }
		inline ObjectReferenceWrapper<T> At(unsigned int at) const { return _list.at(at); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::ArrayObject; }

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
				val->SetContainer(&o);
			}

			s.ReadTokenWithType(lexical::Token::Type::ARRAY_END);

			s.LexicalSettingsPop();
			return s;
		}

	protected:
		std::vector<ObjectReferenceWrapper<T>> _list;
	};

	class MixedArrayObject : public ArrayObject<Object>
	{
	public:
		template <typename T>
		ObjectReferenceWrapper<ArrayObject<T>> ToArrayType()
		{
			std::vector<ObjectReferenceWrapper<T>> list;
			list.resize(_list.size());
			transform(_list.begin(), _list.end(), list.begin(), [](ObjectReferenceWrapper<Object>& obj)->ObjectReferenceWrapper<T>{ return dynamic_wrapper_cast<T>(obj); });

			return ObjectReferenceWrapper<ArrayObject<T>>(new ArrayObject<T>(list));
		}
	};
}

#endif /* _ARRAY_OBJECT_H */
