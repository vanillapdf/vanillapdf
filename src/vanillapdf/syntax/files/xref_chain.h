#ifndef _XREF_CHAIN_H
#define _XREF_CHAIN_H

#include "syntax/utils/syntax_fwd.h"
#include "utils/deferred.h"
#include "syntax/files/xref.h"

#include <list>

namespace vanillapdf {
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
	typedef typename list_type::difference_type difference_type;

	typedef typename list_type::reverse_iterator reverse_iterator;
	typedef typename list_type::const_reverse_iterator const_reverse_iterator;

public:
	class Iterator : public BaseIterator<list_type::const_iterator>, public IWeakReferenceable<Iterator> {
	public:
		using BaseIterator<list_type::const_iterator>::BaseIterator;

		const Iterator& operator++() {
			++BaseIterator<list_type::const_iterator>::m_current;
			return *this;
		}

		const Iterator operator++(int) {
			Iterator temp(BaseIterator<list_type::const_iterator>::m_current, BaseIterator<list_type::const_iterator>::m_invalid);
			++BaseIterator<list_type::const_iterator>::m_current;
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

	reverse_iterator rbegin() { return _list.rbegin(); }
	const_reverse_iterator rbegin() const { return _list.rbegin(); }

	reverse_iterator rend() { return _list.rend(); }
	const_reverse_iterator rend() const { return _list.rend(); }

	IteratorPtr Begin() const { return make_deferred_iterator<Iterator>(_list.begin(), _list.end()); }
	IteratorPtr End(void) const { return make_deferred_iterator<Iterator>(_list.end(), _list.end()); }

	void Append(value_type item) { _list.push_back(item); }
	void Prepend(value_type item) { _list.push_front(item); }
	void Reverse(void) { _list.reverse(); }
	void Clear(void) { _list.clear(); }
	void PopFront() { _list.pop_front(); }
	void PopBack() { _list.pop_back(); }
	types::size_type GetSize(void) const { return _list.size(); }
	bool Empty(void) const { return _list.empty(); }
	XrefBasePtr Back(void) const { return _list.back(); }

	bool ReleaseEntry(XrefUsedEntryBasePtr entry);
	XrefEntryBasePtr GetXrefEntry(types::big_uint obj_number, types::ushort gen_number) const;
	bool Contains(types::big_uint obj_number, types::ushort gen_number) const;

private:
	list_type _list;
};

} // syntax
} // vanillapdf

#endif /* _XREF_CHAIN_H */
