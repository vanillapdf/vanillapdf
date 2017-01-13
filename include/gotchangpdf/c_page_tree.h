#ifndef _C_PAGE_TREE_H
#define _C_PAGE_TREE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_page_tree.h
	* This file contains class definitions for PageTreeHandle
	*/

	/**
	* \class PageTreeHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* The pages of a document are accessed through a structure
	* known as the page tree, which defines the ordering
	* of pages in the document.
	* \see CatalogHandle
	*/

	/**
	* \memberof PageTreeHandle
	* @{
	*/

	/**
	* \brief Get total number of pages in current document.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle handle, out_integer_type result);

	/**
	* \brief Get page at index \p at.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_GetPage(PageTreeHandle handle, integer_type at, PPageObjectHandle result);

	/**
	* \brief Insert new page at index \p at.
	*
	* The page tree is extended by inserting new element before the
	* element \p at the specified position,
	* effectively increasing the container by one.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_InsertPage(PageTreeHandle handle, integer_type at, PageObjectHandle page);

	/**
	* \brief
	* Adds a new page at the end of the page tree,
	* after its current last element.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_AppendPage(PageTreeHandle handle, PageObjectHandle page);

	/**
	* \brief Removed a page at index \p at.
	*
	* This effectively reduces the container size by one.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_RemovePage(PageTreeHandle handle, integer_type at);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageTree_Release(PageTreeHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_TREE_H */
