#ifndef _C_PAGE_TREE_H
#define _C_PAGE_TREE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_page_tree.h
    * \brief This file contains class definitions for \ref PageTreeHandle
    */

    /**
    * \class PageTreeHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * The pages of a document are accessed through a structure known as the page tree,
    * which defines the ordering of pages in the document.
    * \see \ref CatalogHandle
    */

    /**
    * \memberof PageTreeHandle
    * @{
    */

    /**
    * \brief Get total number of pages in current document.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle* handle, size_type* result);

    /**
    * \brief Get page at index \p at.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_GetPage(PageTreeHandle* handle, size_type at, PageObjectHandle** result);

    /**
    * \brief Pre-warm the page cache by walking the entire page tree once.
    *
    * Builds the internal flat page vector so that subsequent
    * \ref PageTree_GetPage calls return in O(1). Call this on a background
    * thread to eliminate the cold-start delay on the first page access.
    * Idempotent: safe to call again after the cache is already warm.
    *
    * \note Do not call concurrently with other PageTree operations.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_WarmPageCache(PageTreeHandle* handle);

    /**
    * \brief Insert new page at index \p at.
    *
    * The page tree is extended by inserting new element before the
    * element \p at the specified position,
    * effectively increasing the container by one.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_InsertPage(PageTreeHandle* handle, size_type at, PageObjectHandle* page);

    /**
    * \brief
    * Adds a new page at the end of the page tree,
    * after its current last element.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_AppendPage(PageTreeHandle* handle, PageObjectHandle* page);

    /**
    * \brief Removed a page at index \p at.
    *
    * This effectively reduces the container size by one.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_RemovePage(PageTreeHandle* handle, size_type at);

    /**
    * \brief Find the 1-based page index for a given page dictionary object.
    *
    * Walks the page tree and returns the 1-based index of the page
    * whose underlying dictionary matches the provided object.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_FindPageIndex(PageTreeHandle* handle, ObjectHandle* page_ref, size_type* result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_ToUnknown(PageTreeHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref PageTreeHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_FromUnknown(IUnknownHandle* handle, PageTreeHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PageTree_Release(PageTreeHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_TREE_H */
