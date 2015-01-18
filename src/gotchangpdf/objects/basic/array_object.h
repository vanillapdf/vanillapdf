#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "containable.h"

#include <vector>
#include <algorithm>

namespace gotchangpdf
{
	template <typename T>
	class ArrayObject : public Containable, public Object
	{
	public:
		typedef std::vector<T> value_type;

	public:
		explicit ArrayObject() {}
		explicit ArrayObject(value_type& list) : _list(list) {}

		inline int Size(void) const { return _list.size(); }
		inline const T& operator[](unsigned int i) const { return _list[i]; }
		inline const T& At(unsigned int at) const { return _list.at(at); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::Array; }

		template <typename U>
		ArrayObjectPtr<U> Convert(std::function<const U(T& obj)> f)
		{
			std::vector<U> list;
			list.resize(_list.size());
			transform(_list.begin(), _list.end(), list.begin(), f);

			return ArrayObject<U>(list);
		}

	//protected:
	public:
		value_type _list;
	};

	class MixedArrayObject : public ArrayObject<ContainableObject>
	{
	public:
		template <typename T>
		ArrayObjectPtr<T> CastToArrayType()
		{
			return Convert<T>([](ContainableObject obj)
			{
				ObjectVisitor<T> visitor;
				return obj.apply_visitor(visitor);
			});
		}
	};
}

#endif /* _ARRAY_OBJECT_H */
