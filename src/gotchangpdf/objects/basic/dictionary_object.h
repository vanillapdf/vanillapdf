#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "fwd.h"
#include "deferred.h"
#include "direct_object.h"
#include "object_visitors.h"
#include "name_object.h"
#include "containerable.h"
#include "hasher.h"

//#include <unordered_map>
#include <map>
//#include <vector>

namespace gotchangpdf
{
	template <typename NameT, typename ValueT, typename Container = ParentContainer<ContainerPtr>>
	class DictionaryObjectBase : public Object, public Container
	{
	public:
		//typedef std::unordered_map<NameType, T, std::hash<NameType>> value_type;
		typedef std::map<NameT, ValueT> value_type;
		//typedef std::vector<std::pair<NameType, T>> value_type;

	public:
		class Iterator
		{
		public:
			Iterator::Iterator(typename value_type::const_iterator it) : _it(it) {}

			Iterator Begin(void) const
			{
				return Iterator(_list.begin());
			}

			Iterator End(void) const
			{
				return Iterator(_list.end());
			}

			const Iterator& operator++()
			{
				++_it;
				return *this;
			}

			const Iterator operator++(int)
			{
				Iterator temp(_it);
				++_it;
				return temp;
			}

			Iterator* Clone() const
			{
				return new Iterator(_it);
			}

			NameT First() const { return _it->first; }
			ValueT Second() const { return _it->second; }

			bool operator==(const Iterator& other) const
			{
				return _it == other._it;
			}

		private:
			typename value_type::const_iterator _it;
		};

		virtual inline Object::Type GetType(void) const override { return Object::Type::DictionaryObject; }

		//friend Objects::ReverseStream& operator>> (Streams::Lexical::ReverseStream& s, DictionaryObject& o);

		template <typename U>
		const U FindAs(const NameT& name) const
		{
			auto result = _list.find(name);

			ObjectVisitor<U> visitor;
			return result->second.apply_visitor(visitor);
		}

		const value_type& GetMap(void) const { return _list; }
		void SetMap(value_type& list) { _list = list; }

		Iterator Begin(void) const
		{
			return Iterator(_list.begin());
		}

		Iterator End(void) const
		{
			return Iterator(_list.end());
		}

		ValueT Find(const NameT& name) const
		{
			auto result = _list.find(name);
			return result->second;
		}

		//private:
	public:
		value_type _list;
	};

	class DictionaryObject : public DictionaryObjectBase<NameObject, DirectObject>
	{
		friend lexical::Parser& operator>> (lexical::Parser& s, DictionaryObject& o);
	};
}

#endif /* _DICTIONARY_OBJECT_H */
