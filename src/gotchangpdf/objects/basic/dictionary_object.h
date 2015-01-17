#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "fwd.h"
#include "unknown_interface.h"
#include "direct_object.h"
#include "object_visitors.h"
//#include "name_object.h"
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
		//typedef std::unordered_map<NameT, ValueT, std::hash<NameT>> value_type;
		typedef std::map<NameT, ValueT> value_type;
		//typedef std::vector<std::pair<NameT, ValueT>> value_type;

	public:
		class Iterator : public IUnknown
		{
		public:
			typedef typename value_type::const_iterator const_iterator;

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

		virtual inline Object::Type GetType(void) const override { return Object::Type::Dictionary; }

		template <typename U>
		const U FindAs(const NameT& name) const
		{
			auto result = Find(name);

			ObjectVisitor<U> visitor;
			return result.apply_visitor(visitor);
		}

		const value_type& GetMap(void) const { return _list; }
		void SetMap(value_type& list) { _list = list; }

		IteratorPtr Begin(void) const
		{
			return Iterator(_list.begin());
		}

		IteratorPtr End(void) const
		{
			return Iterator(_list.end());
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

		//private:
	public:
		value_type _list;
	};

	class DictionaryObject : public DictionaryObjectBase<NameObjectPtr, DirectObject>
	{
	public:
		inline bool Equals(const DictionaryObject& other) const { return this == &other; }

		inline bool operator==(const DictionaryObject& other) const { return Equals(other); }
		inline bool operator!=(const DictionaryObject& other) const { return !Equals(other); }
		inline bool operator<(const DictionaryObject& other) const { return false; }
	};
}

#endif /* _DICTIONARY_OBJECT_H */
