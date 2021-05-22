#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/page_object.h"

namespace vanillapdf {
namespace semantics {

class PageTree : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	explicit PageTree(syntax::DictionaryObjectPtr root);

	types::size_type PageCount(void) const;
	PageObjectPtr Page(types::size_type page_number) const { return GetCachedPage(page_number); }
	PageObjectPtr operator[](types::size_type page_number) const { return GetCachedPage(page_number); }

	void Insert(PageObjectPtr object, types::size_type page_index);
	void Append(PageObjectPtr object);
	void Remove(types::size_type page_index);

private:
	PageObjectPtr GetCachedPage(types::size_type page_number) const;
	PageObjectPtr PageInternal(PageTreeNodePtr node, types::size_type page_number, types::size_type& processed) const;
	bool HasTreeChilds(PageTreeNodePtr node) const;
	types::size_type PageCount(PageNodeBasePtr node);
	void UpdateKidsCount();
	syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> GetKidsInternal();

	static types::size_type UpdateKidsCount(PageNodeBasePtr node);

};

} // semantics
} // vanillapdf

#endif /* _PAGE_TREE_H */
