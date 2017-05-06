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
	class Iterator : public BaseIterator<list_type::const_iterator>, public IWeakReferenceable<Iterator> {
	public:
		using BaseIterator<list_type::const_iterator>::BaseIterator;

		const Iterator& operator++() {
			++BaseIterator<list_type::const_iterator>::m_it;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(BaseIterator<list_type::const_iterator>::m_it);
			++BaseIterator<list_type::const_iterator>::m_it;
			return temp;
		}
	};

	using IteratorPtr = DeferredIterator<Iterator>;

public:
	// stl compatibility
	iterator begin() { return _list.begin(); }
	const_iterator begin() const { return _list.begin(); }

	iterator end(void) { return _list.end(); }
	const_iterator end(void) const { return _list.end(); }

	IteratorPtr Begin() const { return make_deferred<Iterator>(_list.begin()); }
	IteratorPtr End(void) const { return make_deferred<Iterator>(_list.end()); }
	void Append(value_type item) { _list.push_back(item); }
	void Prepend(value_type item) { _list.push_front(item); }
	void Reverse(void) { _list.reverse(); }
	void Clear(void) { _list.clear(); }
	size_t Size(void) const { return _list.size(); }
	bool Empty(void) const { return _list.empty(); }

	XrefUsedEntryBasePtr AllocateNewEntry();
	bool ReleaseEntry(XrefUsedEntryBasePtr entry);
	XrefEntryBasePtr GetXrefEntry(types::big_uint objNumber, types::ushort genNumber) const;
	bool Contains(types::big_uint objNumber, types::ushort genNumber) const;

private:
	list_type _list;

	types::big_uint m_next_allocation = 0;
};

} // syntax
} // gotchangpdf

#endif /* _XREF_CHAIN_H */
