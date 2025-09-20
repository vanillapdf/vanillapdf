#include "precompiled.h"

#include "semantics/objects/bookmarks.h"
#include "semantics/objects/outline.h"
#include "semantics/objects/destinations.h"

#include "vanillapdf/semantics/c_bookmarks.h"
#include "vanillapdf/semantics/c_outline.h"
#include "vanillapdf/semantics/c_destinations.h"
#include "implementation/c_helper.h"

#include <vector>

using namespace vanillapdf;
using namespace vanillapdf::semantics;

// BookmarksIterator implementation
VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_Create(OutlineHandle* outline_handle, BookmarksIteratorHandle** result)
{
    Outline* outline = reinterpret_cast<Outline*>(outline_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(outline);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto iterator = new BookmarksIterator(OutlinePtr(outline));
        outline->AddRef();
        *result = reinterpret_cast<BookmarksIteratorHandle*>(iterator);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_HasNext(BookmarksIteratorHandle* handle, boolean_type* result)
{
    BookmarksIterator* iterator = reinterpret_cast<BookmarksIterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = iterator->HasNext();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_GetNext(BookmarksIteratorHandle* handle, BookmarkItemHandle** result)
{
    BookmarksIterator* iterator = reinterpret_cast<BookmarksIterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto item = iterator->GetNext();
        if (!item) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }
        *result = reinterpret_cast<BookmarkItemHandle*>(item.release());
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_Reset(BookmarksIteratorHandle* handle)
{
    BookmarksIterator* iterator = reinterpret_cast<BookmarksIterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);

    try
    {
        iterator->Reset();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_GetCount(BookmarksIteratorHandle* handle, size_type* result)
{
    BookmarksIterator* iterator = reinterpret_cast<BookmarksIterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = iterator->GetCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_Release(BookmarksIteratorHandle* handle)
{
    return ObjectRelease<BookmarksIterator, BookmarksIteratorHandle>(handle);
}

// BookmarkItem implementation
VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetTitle(BookmarkItemHandle* handle, StringObjectHandle** result)
{
    BookmarkItem* item = reinterpret_cast<BookmarkItem*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto title = item->GetTitle();
        auto ptr = title.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetDestination(BookmarkItemHandle* handle, DestinationHandle** result)
{
    BookmarkItem* item = reinterpret_cast<BookmarkItem*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto dest_obj = item->GetDestination();
        if (syntax::ObjectUtils::IsType<syntax::NullObjectPtr>(dest_obj)) {
            *result = nullptr;
            return VANILLAPDF_ERROR_SUCCESS;
        }
        
        auto destination = DestinationBase::Create(dest_obj);
        if (!destination) {
            *result = nullptr;
            return VANILLAPDF_ERROR_SUCCESS;
        }
        
        destination->AddRef();
        *result = reinterpret_cast<DestinationHandle*>(destination.release());
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetLevel(BookmarkItemHandle* handle, size_type* result)
{
    BookmarkItem* item = reinterpret_cast<BookmarkItem*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = item->GetLevel();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_HasChildren(BookmarkItemHandle* handle, boolean_type* result)
{
    BookmarkItem* item = reinterpret_cast<BookmarkItem*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = item->HasChildren();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_IsOpen(BookmarkItemHandle* handle, boolean_type* result)
{
    BookmarkItem* item = reinterpret_cast<BookmarkItem*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = item->IsOpen();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetColor(BookmarkItemHandle* handle, OutlineItemColorHandle** result)
{
    BookmarkItem* item = reinterpret_cast<BookmarkItem*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto color = item->GetColor();
        if (!color) {
            *result = nullptr;
            return VANILLAPDF_ERROR_SUCCESS;
        }
        color->AddRef();
        *result = reinterpret_cast<OutlineItemColorHandle*>(color.get());
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetFlags(BookmarkItemHandle* handle, OutlineItemFlagsHandle** result)
{
    BookmarkItem* item = reinterpret_cast<BookmarkItem*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto flags = item->GetFlags();
        if (!flags) {
            *result = nullptr;
            return VANILLAPDF_ERROR_SUCCESS;
        }
        flags->AddRef();
        *result = reinterpret_cast<OutlineItemFlagsHandle*>(flags.get());
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetOutlineItem(BookmarkItemHandle* handle, OutlineItemHandle** result)
{
    BookmarkItem* item = reinterpret_cast<BookmarkItem*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(item);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto outline_item = item->GetOutlineItem();
        outline_item->AddRef();
        *result = reinterpret_cast<OutlineItemHandle*>(outline_item.get());
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_Release(BookmarkItemHandle* handle)
{
    return ObjectRelease<BookmarkItem, BookmarkItemHandle>(handle);
}

// Helper functions
VANILLAPDF_API error_type CALLING_CONVENTION Bookmarks_GetAllItems(OutlineHandle* outline_handle, BookmarkItemHandle*** items, size_type* count)
{
    Outline* outline = reinterpret_cast<Outline*>(outline_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(outline);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(items);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(count);

    try
    {
        BookmarksIterator iterator(OutlinePtr(outline));
        outline->AddRef();
        
        std::vector<BookmarkItem*> bookmark_items;
        
        while (iterator.HasNext()) {
            auto item = iterator.GetNext();
            if (item) {
                bookmark_items.push_back(item.release());
            }
        }
        
        *count = bookmark_items.size();
        if (*count > 0) {
            *items = new BookmarkItemHandle*[*count];
            for (size_t i = 0; i < *count; ++i) {
                (*items)[i] = reinterpret_cast<BookmarkItemHandle*>(bookmark_items[i]);
            }
        } else {
            *items = nullptr;
        }
        
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Bookmarks_FreeItemsArray(BookmarkItemHandle** items, size_type count)
{
    if (items) {
        for (size_t i = 0; i < count; ++i) {
            if (items[i]) {
                BookmarkItem_Release(items[i]);
            }
        }
        delete[] items;
    }
    return VANILLAPDF_ERROR_SUCCESS;
}