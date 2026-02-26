#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/page_object.h"

#include <mutex>
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

    // Pre-warm the page cache completely. Call this on a background thread to
    // build the full flat page vector before interactive use begins.
    // Idempotent: calling it again after the cache is already warm is a no-op.
    void WarmPageCache() const;

private:
    // Flat cache of page dictionary pointers in document order.
    // Built in full on first access (by GetCachedPage or WarmPageCache).
    // Cleared by Insert/Remove/Append so structural changes are reflected
    // on the next access. GetCachedPage constructs a PageObject on demand
    // from the cached dictionary pointer.
    mutable std::unique_ptr<std::recursive_mutex> m_cache_lock;
    mutable std::vector<syntax::DictionaryObjectPtr> m_page_cache;
    mutable bool m_cache_built = false;

    PageObjectPtr GetCachedPage(types::size_type page_number) const;
    void InvalidatePageCache();
    void BuildPageCache() const;

    bool FindPageIndexInternal(PageTreeNodePtr node, syntax::DictionaryObjectPtr page_dict, types::size_type& current_index) const;
    void UpdateKidsCount();
    syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> GetKidsInternal();

    static types::size_type UpdateKidsCount(PageNodeBasePtr node);

};

} // semantics
} // vanillapdf

#endif /* _PAGE_TREE_H */
