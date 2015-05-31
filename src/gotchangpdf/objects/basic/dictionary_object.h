#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "fwd.h"
#include "object.h"
#include "object_visitors.h"
#include "containable.h"

//#include <unordered_map>
#include <map>
//#include <vector>

#include <sstream>

namespace gotchangpdf
{
	template <typename NameT, typename ValueT>
	class DictionaryObjectBase : public Containable, public Object
	{
	public:
		//typedef std::unordered_map<NameT, ValueT, std::hash<NameT>> list_type;
		typedef std::map<NameT, ValueT> list_type;
		//typedef std::vector<std::pair<NameT, ValueT>> list_type;

		typedef typename list_type::value_type value_type;
		typedef typename list_type::iterator iterator;
		typedef typename list_type::const_iterator const_iterator;
		typedef typename list_type::size_type size_type;
		typedef typename list_type::reference reference;

	public:
		class Iterator : public IUnknown
		{
		public:
			Iterator() = default;
			Iterator(typename const_iterator it) : _it(it) {}

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

			NameT First() const { return _it->first; }
			ValueT Second() const { return _it->second; }

			bool operator==(const Iterator& other) const
			{
				return _it == other._it;
			}

		private:
			const_iterator _it;
		};

		using IteratorPtr = Deferred<Iterator>;

		template <typename U>
		U FindAs(const NameT& name) const
		{
			auto result = Find(name);

			ObjectVisitor<U> visitor;
			return result.apply_visitor(visitor);
		}

		const list_type& GetMap(void) const { return _list; }
		void SetMap(list_type& list) { _list = list; }

		IteratorPtr Begin(void) const
		{
			return Iterator(_list.begin());
		}

		IteratorPtr End(void) const
		{
			return Iterator(_list.end());
		}

		const_iterator begin(void) const _NOEXCEPT
		{
			return _list.begin();
		}

		const_iterator end(void) const _NOEXCEPT
		{
			return _list.end();
		}

		iterator insert(const_iterator pos,
			const value_type& value)
		{
			return _list.insert(pos, value);
		}

		ValueT Find(const NameT& name) const
		{
			auto result = _list.find(name);
			if (result == _list.end()) {
				std::stringstream ss;
				ss << "Item with name " << name << " was not found in dictionary";
				throw exceptions::Exception(ss.str());
			}

			return result->second;
		}

		bool TryFind(const NameT& name, ValueT& result) const
		{
			auto item = _list.find(name);
			if (item == _list.end())
				return false;

			result = item->second;
			return true;
		}

		inline bool Contains(const NameT& name) const { return (_list.find(name) != _list.end()); }

		virtual inline Object::Type GetType(void) const override { return Object::Type::Dictionary; }
		virtual inline void SetContainer(ContainerPtr obj) override { _container = obj; }
		virtual inline ContainerPtr GetContainer() const override { return _container; }

		//private:
	public:
		list_type _list;
		ContainerPtr _container;
	};

	class DictionaryObject : public DictionaryObjectBase<NameObjectPtr, ContainableObject>
	{
	public:
		inline bool Equals(const DictionaryObject& other) const { return this == &other; }

		inline bool operator==(const DictionaryObject& other) const { return Equals(other); }
		inline bool operator!=(const DictionaryObject& other) const { return !Equals(other); }
		inline bool operator<(const DictionaryObject&) const { return false; }
	};
}

#endif /* _DICTIONARY_OBJECT_H */
