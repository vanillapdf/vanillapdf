#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "syntax_fwd.h"
#include "object.h"
#include "exception.h"
#include "containable.h"

//#include <unordered_map>
#include <map>
//#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		class DictionaryObject : public ContainableObject
		{
		public:
			//typedef std::unordered_map<NameObjectPtr, ContainableObject, std::hash<NameObjectPtr>> list_type;
			typedef std::map<NameObjectPtr, ContainableObjectPtr> list_type;
			//typedef std::vector<std::pair<NameObjectPtr, ContainableObject>> list_type;

			typedef list_type::value_type value_type;
			typedef list_type::iterator iterator;
			typedef list_type::const_iterator const_iterator;
			typedef list_type::size_type size_type;
			typedef list_type::reference reference;
			typedef list_type::const_reference const_reference;

		public:
			class Iterator : public IUnknown
			{
			public:
				typedef const_iterator::value_type value_type;
				typedef const_iterator::difference_type difference_type;
				typedef const_iterator::pointer pointer;
				typedef const_iterator::reference reference;

			public:
				Iterator() = default;
				Iterator(const_iterator it) : _it(it) {}

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

				NameObjectPtr First() const { return _it->first; }
				ContainableObjectPtr Second() const { return _it->second; }
				const_iterator Value() const { return _it; }

				bool operator==(const Iterator& other) const
				{
					return _it == other._it;
				}

			private:
				const_iterator _it;
			};

			using IteratorPtr = DeferredIterator<Iterator>;

			template <typename U>
			U FindAs(const NameObjectPtr& name) const
			{
				auto result = Find(name);
				return ObjectUtils::ConvertTo<U>(result);
			}

			list_type GetItems(void) const { return _list; }
			void SetItems(const list_type& list) { _list = list; }

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

			ContainableObjectPtr Find(const NameObjectPtr& name) const
			{
				auto result = _list.find(name);
				if (result == _list.end()) {
					throw GeneralException("Item with name " + name->Value()->ToString() + " was not found in dictionary");
				}

				return result->second;
			}

			bool TryFind(const NameObjectPtr& name, ContainableObjectPtr& result) const
			{
				auto item = _list.find(name);
				if (item == _list.end())
					return false;

				result = item->second;
				return true;
			}

			inline bool Contains(const NameObjectPtr& name) const { return (_list.find(name) != _list.end()); }

			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Dictionary; }

		public:
			list_type _list;
		};
	}
}

#endif /* _DICTIONARY_OBJECT_H */
