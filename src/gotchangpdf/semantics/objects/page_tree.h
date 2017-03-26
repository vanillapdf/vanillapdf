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
	explicit PageTree(syntax::DictionaryObjectPtr root);

	types::integer PageCount(void) const;
	PageObjectPtr Page(types::integer page_number) const { return GetCachedPage(page_number); }
	PageObjectPtr operator[](types::integer page_number) const { return GetCachedPage(page_number); }

	void Insert(PageObjectPtr object, types::integer page_index);
	void Append(PageObjectPtr object);
	void Remove(types::integer page_index);

private:
	PageObjectPtr GetCachedPage(types::integer page_number) const;
	PageObjectPtr PageInternal(PageTreeNodePtr node, types::integer page_number, types::integer& processed) const;
	bool HasTreeChilds(PageTreeNodePtr node) const;
	types::integer PageCount(PageNodeBasePtr node);
	void UpdateKidsCount();

	static types::integer UpdateKidsCount(PageNodeBasePtr node);

private:
	// Cache
	mutable std::vector<OutputPageObjectPtr> m_pages;
};

} // semantics
} // gotchangpdf

#endif /* _PAGE_TREE_H */
