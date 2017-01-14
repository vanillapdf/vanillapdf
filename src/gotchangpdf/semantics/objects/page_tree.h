#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/page_object.h"

namespace gotchangpdf {
namespace semantics {

class PageTree : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	//PageTree();
	explicit PageTree(syntax::DictionaryObjectPtr root);

	types::uinteger PageCount(void) const;
	PageObjectPtr Page(types::integer number) const { return PageInternal(number); }
	PageObjectPtr operator[](types::integer number) const { return PageInternal(number); }

	void Insert(PageObjectPtr object, types::integer index);
	void Append(PageObjectPtr object);
	void Remove(types::integer index);

private:
	PageObjectPtr PageInternal(types::integer number) const;
	bool HasTreeChilds(PageTreeNodePtr node) const;
	void UpdateKidsCount(size_t new_size);

private:
	// Cache
	mutable std::vector<OutputPageObjectPtr> m_pages;
};

} // semantics
} // gotchangpdf

#endif /* _PAGE_TREE_H */
