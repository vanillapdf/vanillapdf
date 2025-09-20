#ifndef BOOKMARKS_H
#define BOOKMARKS_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"
#include "semantics/objects/outline.h"

#include <vector>
#include <stack>

namespace vanillapdf {
namespace semantics {

class BookmarkItem : public HighLevelObject<OutlineItemPtr> {
public:
    explicit BookmarkItem(OutlineItemPtr item, size_t level = 0);

    syntax::StringObjectPtr GetTitle() const;
    syntax::ObjectPtr GetDestination() const;
    size_t GetLevel() const;
    bool HasChildren() const;
    bool IsOpen() const;
    OutlineItemColorPtr GetColor() const;
    OutlineItemFlagsPtr GetFlags() const;
    OutlineItemPtr GetOutlineItem() const;

private:
    size_t m_level;
};

class BookmarksIterator : public IUnknown {
public:
    explicit BookmarksIterator(OutlinePtr outline);

    bool HasNext() const;
    std::unique_ptr<BookmarkItem> GetNext();
    void Reset();
    size_t GetCount() const;

private:
    struct IteratorState {
        OutlineItemPtr item;
        size_t level;
        
        IteratorState(OutlineItemPtr i, size_t l) : item(i), level(l) {}
    };

    void InitializeStack();
    size_t CountBookmarks(OutlineItemPtr item) const;

    OutlinePtr m_outline;
    std::stack<IteratorState> m_stack;
    size_t m_total_count;
};

} // semantics
} // vanillapdf

#endif /* BOOKMARKS_H */