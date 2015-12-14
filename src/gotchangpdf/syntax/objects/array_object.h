#ifndef _ARRAY_OBJECT_H
#define _ARRAY_OBJECT_H

#include "syntax_fwd.h"
#include "object.h"
#include "containable.h"

#include <vector>
#include <algorithm>
#include <functional>

namespace gotchangpdf
{
	namespace syntax
	{
		template <typename T>
		class ArrayObject : public ContainableObject
		{
		public:
			typedef std::vector<T> list_type;
			typedef typename list_type::value_type value_type;
			typedef typename list_type::iterator iterator;
			typedef typename list_type::const_iterator const_iterator;
			typedef typename list_type::size_type size_type;
			typedef typename list_type::reference reference;
			typedef typename list_type::const_reference const_reference;

		public:
			explicit ArrayObject() {}
			explicit ArrayObject(list_type& list) : _list(list) {}
			explicit ArrayObject(std::initializer_list<T> list) : _list(list) {}
			explicit ArrayObject(const ContainableObject& other, list_type& list)
				: ContainableObject(other), _list(list) {}

			inline types::integer Size(void) const _NOEXCEPT { return _list.size(); }
			inline const T& operator[](unsigned int i) const { return _list[i]; }
			inline T& operator[](unsigned int i) { return _list[i]; }
			inline const T& At(unsigned int at) const { return _list.at(at); }
			inline T& At(unsigned int at) { return _list.at(at); }

			iterator begin() _NOEXCEPT { return _list.begin(); }
			const_iterator begin() const _NOEXCEPT { return _list.begin(); }
			iterator end() _NOEXCEPT { return _list.end(); }
			const_iterator end() const _NOEXCEPT { return _list.end(); }

			template <typename U>
			ArrayObjectPtr<U> Convert(std::function<const U(T& obj)> f)
			{
				std::vector<U> list;
				list.reserve(_list.size());
				std::transform(_list.begin(), _list.end(), std::back_inserter(list), f);
				return ArrayObject<U>(*this, list);
			}

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Array; }

			virtual std::string ToString(void) const override
			{
				std::stringstream ss;
				ss << "[";
				bool first = true;
				for (auto item : _list) {
					//ss << (first ? "" : " ") << item;
					ss << (first ? "" : " ") << "TODO";
					first = false;
				}

				ss << "]";
				return ss.str();
			}

			//protected:
		public:
			list_type _list;
		};

		class MixedArrayObject : public ArrayObject<ContainableObjectPtr>
		{
		public:
			MixedArrayObject() = default;
			MixedArrayObject(list_type items) : ArrayObject<ContainableObjectPtr>(items) {}

			template <typename T>
			ArrayObjectPtr<T> CastToArrayType() { return Convert<T>([](ContainableObjectPtr obj) { return ObjectUtils::ConvertTo<T>(obj); }); }

			virtual std::string ToString(void) const override
			{
				std::stringstream ss;
				ss << "[";
				bool first = true;
				for (auto item : _list) {
					ss << (first ? "" : " ") << item->ToString();
					first = false;
				}

				ss << "]";
				return ss.str();
			}
		};
	}
}

#endif /* _ARRAY_OBJECT_H */
