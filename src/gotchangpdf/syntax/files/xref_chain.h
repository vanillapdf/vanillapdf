#ifndef _XREF_CHAIN_H
#define _XREF_CHAIN_H

#include "syntax_fwd.h"
#include "deferred.h"
#include "xref.h"

#include <list>

namespace gotchangpdf
{
	namespace syntax
	{
		class XrefChain : public IUnknown
		{
		public:
			using list_type = std::list<XrefBasePtr>;

		public:
			class Iterator : public IUnknown
			{
			public:
				typedef list_type::const_iterator::value_type value_type;
				typedef list_type::const_iterator::difference_type difference_type;
				typedef list_type::const_iterator::pointer pointer;
				typedef list_type::const_iterator::reference reference;

			public:
				Iterator() = default;
				Iterator(list_type::const_iterator it) : _it(it) {}

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

				list_type::value_type Value() const { return *_it; }

				bool operator==(const Iterator& other) const { return _it == other._it; }
				bool operator!=(const Iterator& other) const { return _it != other._it; }

			private:
				list_type::const_iterator _it;
			};

			using IteratorPtr = DeferredIterator<Iterator>;

		public:
			// stl compatibility
			list_type::const_iterator begin() const { return _list.begin(); }
			list_type::const_iterator end(void) const { return _list.end(); }

			IteratorPtr Begin() const { return _list.begin(); }
			IteratorPtr End(void) const { return _list.end(); }
			void Append(list_type::value_type item) { _list.push_back(item); }
			void Prepend(list_type::value_type item) { _list.push_front(item); }
			void Reverse(void) { _list.reverse(); }

			XrefUsedEntryBasePtr AllocateNewEntry();
			XrefEntryBasePtr GetXrefEntry(types::big_uint objNumber, types::ushort genNumber);
			bool Contains(types::big_uint objNumber, types::ushort genNumber) const;

		private:
			list_type _list;

			types::big_uint m_next_allocation = 0;
		};
	}
}

#endif /* _XREF_CHAIN_H */
