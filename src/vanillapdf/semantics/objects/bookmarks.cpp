#include "precompiled.h"

#include "semantics/objects/bookmarks.h"
#include "semantics/objects/destinations.h"

namespace vanillapdf {
namespace semantics {

// BookmarkItem implementation
BookmarkItem::BookmarkItem(OutlineItemPtr item, size_t level) 
    : HighLevelObject(item), m_level(level) {
}

syntax::StringObjectPtr BookmarkItem::GetTitle() const {
    return _obj->Title();
}

syntax::ObjectPtr BookmarkItem::GetDestination() const {
    syntax::ObjectPtr dest;
    if (_obj->Destination(dest)) {
        return dest;
    }
    return syntax::NullObject::GetInstance();
}

size_t BookmarkItem::GetLevel() const {
    return m_level;
}

bool BookmarkItem::HasChildren() const {
    OutputOutlineItemPtr first;
    return _obj->First(first);
}

bool BookmarkItem::IsOpen() const {
    syntax::IntegerObjectPtr count;
    if (_obj->Count(count)) {
        // If count is positive, the item is open
        return count->GetIntegerValue() > 0;
    }
    return false;
}

OutlineItemColorPtr BookmarkItem::GetColor() const {
    OutputOutlineItemColorPtr color;
    if (_obj->Color(color)) {
        return color;
    }
    return nullptr;
}

OutlineItemFlagsPtr BookmarkItem::GetFlags() const {
    OutputOutlineItemFlagsPtr flags;
    if (_obj->Flags(flags)) {
        return flags;
    }
    return nullptr;
}

OutlineItemPtr BookmarkItem::GetOutlineItem() const {
    return _obj;
}

// BookmarksIterator implementation
BookmarksIterator::BookmarksIterator(OutlinePtr outline) 
    : m_outline(outline), m_total_count(0) {
    InitializeStack();
    
    // Calculate total count
    OutputOutlineItemPtr first;
    if (outline->First(first)) {
        m_total_count = CountBookmarks(first);
    }
}

void BookmarksIterator::InitializeStack() {
    // Clear the stack
    while (!m_stack.empty()) {
        m_stack.pop();
    }
    
    // Start with the first top-level item
    OutputOutlineItemPtr first;
    if (m_outline->First(first)) {
        // Push siblings in reverse order (so we process them in forward order)
        std::vector<OutlineItemPtr> siblings;
        OutlineItemPtr current = first;
        while (current) {
            siblings.push_back(current);
            OutputOutlineItemPtr next;
            if (!current->Next(next)) {
                break;
            }
            current = next;
        }
        
        // Push in reverse order
        for (auto it = siblings.rbegin(); it != siblings.rend(); ++it) {
            m_stack.push(IteratorState(*it, 0));
        }
    }
}

bool BookmarksIterator::HasNext() const {
    return !m_stack.empty();
}

std::unique_ptr<BookmarkItem> BookmarksIterator::GetNext() {
    if (m_stack.empty()) {
        return nullptr;
    }
    
    IteratorState state = m_stack.top();
    m_stack.pop();
    
    // Add children to stack (in reverse order)
    OutputOutlineItemPtr first_child;
    if (state.item->First(first_child)) {
        std::vector<OutlineItemPtr> children;
        OutlineItemPtr current = first_child;
        while (current) {
            children.push_back(current);
            OutputOutlineItemPtr next;
            if (!current->Next(next)) {
                break;
            }
            current = next;
        }
        
        // Push in reverse order
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            m_stack.push(IteratorState(*it, state.level + 1));
        }
    }
    
    return std::make_unique<BookmarkItem>(state.item, state.level);
}

void BookmarksIterator::Reset() {
    InitializeStack();
}

size_t BookmarksIterator::GetCount() const {
    return m_total_count;
}

size_t BookmarksIterator::CountBookmarks(OutlineItemPtr item) const {
    size_t count = 1;  // Count this item
    
    // Count children
    OutputOutlineItemPtr first_child;
    if (item->First(first_child)) {
        count += CountBookmarks(first_child);
    }
    
    // Count siblings
    OutputOutlineItemPtr next_sibling;
    if (item->Next(next_sibling)) {
        count += CountBookmarks(next_sibling);
    }
    
    return count;
}

} // semantics
} // vanillapdf