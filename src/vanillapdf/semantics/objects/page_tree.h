#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/page_object.h"

#include <vector>

namespace vanillapdf {
namespace semantics {

class PageTree : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
    explicit PageTree(syntax::DictionaryObjectPtr root);

    types::size_type PageCount(void) const;
    PageObjectPtr Page(types::size_type page_number) const { return GetCachedPage(page_number); }
    PageObjectPtr operator[](types::size_type page_number) const { return GetCachedPage(page_number); }

    bool FindPageIndex(syntax::DictionaryObjectPtr page_dict, types::size_type& result) const;

    void Insert(PageObjectPtr object, types::size_type page_index);
    void Append(PageObjectPtr object);
    void Remove(types::size_type page_index);

private:
    // Flat page cache. Built lazily on first GetCachedPage call by walking the
    // page tree once in document order. Cleared by Insert/Remove/Append so that
    // structural changes are always reflected on the next access.
    // GetCachedPage returns PageObjectPtr by value (copy of the intrusive
    // pointer, ref count bumped), so callers are independent of the vector.
    // reserve(PageCount()) is called before the walk to prevent reallocation.
    mutable std::vector<PageObjectPtr> m_page_cache;

    PageObjectPtr GetCachedPage(types::size_type page_number) const;
    void BuildPageCache() const;
    void BuildPageCacheInternal(PageTreeNodePtr node) const;
    void InvalidatePageCache() noexcept;

    bool FindPageIndexInternal(PageTreeNodePtr node, syntax::DictionaryObjectPtr page_dict, types::size_type& current_index) const;
    void UpdateKidsCount();
    syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> GetKidsInternal();

    static types::size_type UpdateKidsCount(PageNodeBasePtr node);

};

} // semantics
} // vanillapdf

#endif /* _PAGE_TREE_H */
