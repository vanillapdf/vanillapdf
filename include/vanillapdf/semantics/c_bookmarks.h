#ifndef _C_BOOKMARKS_H
#define _C_BOOKMARKS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_bookmarks.h
    * \brief This file contains high-level API for iterating and accessing PDF bookmarks (outlines)
    */

    /**
    * \class BookmarksIteratorHandle
    * \extends IUnknownHandle
    * \ingroup group_outline
    * \brief
    * Iterator for traversing PDF bookmarks in a hierarchical manner.
    * Provides a convenient way to iterate through all bookmarks in a document.
    */

    /**
    * \class BookmarkItemHandle
    * \extends IUnknownHandle
    * \ingroup group_outline
    * \brief
    * Represents a single bookmark item with easy access to its properties.
    * This is a high-level wrapper around OutlineItemHandle for easier use.
    */

    /**
    * \memberof BookmarksIteratorHandle
    * @{
    */

    /**
    * \brief Create a new bookmarks iterator from document's outline.
    * \param outline The outline handle from the document catalog
    * \param result The created iterator handle
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_Create(OutlineHandle* outline, BookmarksIteratorHandle** result);

    /**
    * \brief Check if there are more bookmarks to iterate.
    * \param handle The iterator handle
    * \param result True if there are more bookmarks, false otherwise
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_HasNext(BookmarksIteratorHandle* handle, boolean_type* result);

    /**
    * \brief Get the next bookmark item.
    * \param handle The iterator handle
    * \param result The next bookmark item
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_GetNext(BookmarksIteratorHandle* handle, BookmarkItemHandle** result);

    /**
    * \brief Reset the iterator to start from the beginning.
    * \param handle The iterator handle
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_Reset(BookmarksIteratorHandle* handle);

    /**
    * \brief Get the total count of bookmarks (if available).
    * \param handle The iterator handle
    * \param result The total count of bookmarks
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_GetCount(BookmarksIteratorHandle* handle, size_type* result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarksIterator_Release(BookmarksIteratorHandle* handle);

    /** @} */

    /**
    * \memberof BookmarkItemHandle
    * @{
    */

    /**
    * \brief Get the title/text of the bookmark.
    * \param handle The bookmark item handle
    * \param result The title as a string
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetTitle(BookmarkItemHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the destination of the bookmark.
    * \param handle The bookmark item handle
    * \param result The destination handle, or NULL if no destination
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetDestination(BookmarkItemHandle* handle, DestinationHandle** result);

    /**
    * \brief Get the hierarchical level of the bookmark (0 for top-level).
    * \param handle The bookmark item handle
    * \param result The level of the bookmark
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetLevel(BookmarkItemHandle* handle, size_type* result);

    /**
    * \brief Check if the bookmark has children.
    * \param handle The bookmark item handle
    * \param result True if has children, false otherwise
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_HasChildren(BookmarkItemHandle* handle, boolean_type* result);

    /**
    * \brief Check if the bookmark is open/expanded by default.
    * \param handle The bookmark item handle
    * \param result True if open by default, false otherwise
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_IsOpen(BookmarkItemHandle* handle, boolean_type* result);

    /**
    * \brief Get the color of the bookmark text (if specified).
    * \param handle The bookmark item handle
    * \param result The color handle, or NULL if no color specified
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetColor(BookmarkItemHandle* handle, OutlineItemColorHandle** result);

    /**
    * \brief Get the style flags of the bookmark (bold, italic).
    * \param handle The bookmark item handle
    * \param result The flags handle, or NULL if no flags specified
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetFlags(BookmarkItemHandle* handle, OutlineItemFlagsHandle** result);

    /**
    * \brief Get the underlying outline item handle.
    * \param handle The bookmark item handle
    * \param result The outline item handle
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_GetOutlineItem(BookmarkItemHandle* handle, OutlineItemHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION BookmarkItem_Release(BookmarkItemHandle* handle);

    /** @} */

    /**
    * \brief Helper function to get all bookmarks as a flat list.
    * This function traverses the entire bookmark tree and returns all bookmarks
    * in a flat array for easy iteration.
    *
    * \param outline The outline handle from the document catalog
    * \param items Output array of bookmark items (must be freed by caller)
    * \param count Number of items in the array
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Bookmarks_GetAllItems(OutlineHandle* outline, BookmarkItemHandle*** items, size_type* count);

    /**
    * \brief Free the array returned by Bookmarks_GetAllItems.
    * \param items The array to free
    * \param count Number of items in the array
    * \return Error code indicating success or failure
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Bookmarks_FreeItemsArray(BookmarkItemHandle** items, size_type count);

#ifdef __cplusplus
};
#endif

#endif /* _C_BOOKMARKS_H */