#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "fwd.h"
#include "deferred.h"
#include "object.h"
#include "direct_object.h"
#include "containerable.h"

#include <vector>
#include <algorithm>

namespace gotchangpdf
{
	template <typename T, typename Container = ParentContainer<ContainerPtr>>
	class ArrayObject : public Object, public Container
	{
	public:
		typedef std::vector<T> value_type;

	public:
		explicit ArrayObject() {}
		explicit ArrayObject(value_type& list) : _list(list) {}

		inline int Size(void) const { return _list.size(); }
		inline const T& operator[](unsigned int i) const { return _list[i]; }
		inline const T& At(unsigned int at) const { return _list.at(at); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::ArrayObject; }

		friend lexical::Parser& operator>> (lexical::Parser& s, ArrayObject<T, Container>& o)
		{
			/*
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
			*/
			return s;
		}

		template <typename U>
		Deferred<ArrayObject<U, Container>> Convert(std::function<const U(T& obj)> f)
		{
			std::vector<U> list;
			list.resize(_list.size());
			transform(_list.begin(), _list.end(), list.begin(), f);

			return ArrayObject<U, Container>(list);
		}

		const value_type& GetList(void) const { return _list; }
		void SetList(value_type& list) { _list = list; }

	//protected:
	public:
		value_type _list;
	};

	class MixedArrayObject : public ArrayObject<DirectObject>
	{
	public:
		template <typename T>
		Deferred<ArrayObject<T>> CastToArrayType()
		{
			return Convert<T>([](DirectObject obj)
			{
				ObjectVisitor<T> visitor;
				return obj.apply_visitor(visitor);
			});
		}
	};
}

#endif /* _ARRAY_OBJECT_H */
