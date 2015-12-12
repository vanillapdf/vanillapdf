#ifndef _DICTIONARY_OBJECT_H
#define _DICTIONARY_OBJECT_H

#include "syntax_fwd.h"
#include "object.h"
#include "exception.h"
#include "containable.h"

#include <map>
#include <vector>

namespace gotchangpdf
{
	namespace syntax
	{
		template <typename KeyT, typename ValueT, typename MapT = std::map<KeyT, ValueT>>
		class DictionaryObjectBase : public ContainableObject
		{
		public:
			//typedef std::unordered_map<NameObjectPtr, ContainableObject, std::hash<NameObjectPtr>> list_type;
			typedef MapT list_type;

			typedef typename list_type::value_type value_type;
			typedef typename list_type::iterator iterator;
			typedef typename list_type::const_iterator const_iterator;
			typedef typename list_type::size_type size_type;
			typedef typename list_type::reference reference;
			typedef typename list_type::const_reference const_reference;

		public:
			class Iterator : public IUnknown
			{
			public:
				typedef typename const_iterator::value_type value_type;
				typedef typename const_iterator::difference_type difference_type;
				typedef typename const_iterator::pointer pointer;
				typedef typename const_iterator::reference reference;

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

				KeyT First() const { return _it->first; }
				ValueT Second() const { return _it->second; }
				const_iterator Value() const { return _it; }

				bool operator==(const Iterator& other) const
				{
					return _it == other._it;
				}

			private:
				const_iterator _it;
			};

			using IteratorPtr = DeferredIterator<Iterator>;

			// std container
			const_iterator begin(void) const _NOEXCEPT { return _list.begin(); }
			const_iterator end(void) const _NOEXCEPT { return _list.end(); }
			iterator insert(const_iterator pos, const value_type & value) { return _list.insert(pos, value); }

		public:
			list_type _list;
		};

		class DictionaryObject : public DictionaryObjectBase<NameObjectPtr, ContainableObjectPtr>
		{
		public:
			friend std::ostream& operator<<(std::ostream& os, const DictionaryObject& obj);
			virtual std::string ToString(void) const override;
			virtual inline Object::Type GetType(void) const _NOEXCEPT override { return Object::Type::Dictionary; }

			template <typename U>
			U FindAs(const NameObjectPtr& name) const
			{
				auto result = Find(name);
				return ObjectUtils::ConvertTo<U>(result);
			}
			
			std::vector<ContainableObjectPtr> Values() const;
			ContainableObjectPtr Find(const NameObjectPtr& name) const;
			bool TryFind(const NameObjectPtr& name, ContainableObjectPtr& result) const;
			bool Contains(const NameObjectPtr& name) const;
		};
	}
}

#endif /* _DICTIONARY_OBJECT_H */
