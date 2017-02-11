#ifndef _XREF_CHAIN_H
#define _XREF_CHAIN_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/deferred.h"
#include "syntax/files/xref.h"

#include <list>

namespace gotchangpdf {
namespace syntax {

class XrefChain : public virtual IUnknown, public IWeakReferenceable<XrefChain> {
public:
	using list_type = std::list<XrefBasePtr>;

	typedef typename list_type::value_type value_type;
	typedef typename list_type::iterator iterator;
	typedef typename list_type::const_iterator const_iterator;
	typedef typename list_type::size_type size_type;
	typedef typename list_type::reference reference;
	typedef typename list_type::const_reference const_reference;

public:
	class Iterator : public virtual IUnknown, public IWeakReferenceable<Iterator> {
	public:
		typedef list_type::const_iterator::value_type value_type;
		typedef list_type::const_iterator::difference_type difference_type;
		typedef list_type::const_iterator::pointer pointer;
		typedef list_type::const_iterator::reference reference;

	public:
		Iterator() = default;
		Iterator(list_type::const_iterator it) : _it(it) {}

		const Iterator& operator++() {
			++_it;
			return *this;
		}

		const Iterator operator++(int) {
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
	iterator begin() { return _list.begin(); }
	const_iterator begin() const { return _list.begin(); }

	iterator end(void) { return _list.end(); }
	const_iterator end(void) const { return _list.end(); }

	IteratorPtr Begin() const { return _list.begin(); }
	IteratorPtr End(void) const { return _list.end(); }
	void Append(value_type item) { _list.push_back(item); }
	void Prepend(value_type item) { _list.push_front(item); }
	void Reverse(void) { _list.reverse(); }

	XrefUsedEntryBasePtr AllocateNewEntry();
	XrefEntryBasePtr GetXrefEntry(types::big_uint objNumber, types::ushort genNumber) const;
	bool Contains(types::big_uint objNumber, types::ushort genNumber) const;

private:
	list_type _list;

	types::big_uint m_next_allocation = 0;
};

} // syntax
} // gotchangpdf

#endif /* _XREF_CHAIN_H */
