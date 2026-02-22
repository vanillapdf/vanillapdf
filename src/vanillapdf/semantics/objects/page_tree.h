#ifndef _PAGE_TREE_H
#define _PAGE_TREE_H

#include "semantics/utils/semantics_fwd.h"

#include "semantics/objects/high_level_object.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/page_object.h"

#include <mutex>
#include <optional>
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
    // Resumable page-tree walker used to build m_page_cache lazily.
    // Holds a stack of {node, next-kid-index} frames. Calling Next() advances
    // through the tree in document order and returns the next leaf PageObjectPtr,
    // or a null pointer once the tree is exhausted.
    struct PageTreeWalker {
        struct Frame {
            PageTreeNodePtr node;
            size_t index = 0;

            explicit Frame(PageTreeNodePtr n) : node(n) {}
        };
        std::vector<Frame> stack;

        explicit PageTreeWalker(PageTreeNodePtr root);
        bool TryNext(OutputPageObjectPtr& result);
    };

    // Flat page cache. Extended lazily by GetCachedPage: m_walker is advanced
    // until m_page_cache.size() >= the requested page_number. Fully drained by
    // WarmPageCache(). Cleared (together with m_walker) by Insert/Remove/Append
    // so that structural changes are always reflected on the next access.
    // GetCachedPage returns PageObjectPtr by value (copy of the intrusive
    // pointer, ref count bumped), so callers are independent of the vector.
    // reserve(PageCount()) is called when m_walker is first created to prevent
    // reallocation during the lazy fill.
    mutable std::unique_ptr<std::recursive_mutex> m_cache_lock;
    mutable std::vector<PageObjectPtr> m_page_cache;
    mutable std::optional<PageTreeWalker> m_walker;

    PageObjectPtr GetCachedPage(types::size_type page_number) const;
    void InvalidatePageCache();

    bool FindPageIndexInternal(PageTreeNodePtr node, syntax::DictionaryObjectPtr page_dict, types::size_type& current_index) const;
    void UpdateKidsCount();
    syntax::ArrayObjectPtr<syntax::IndirectReferenceObjectPtr> GetKidsInternal();

    static types::size_type UpdateKidsCount(PageNodeBasePtr node);

};

} // semantics
} // vanillapdf

#endif /* _PAGE_TREE_H */
