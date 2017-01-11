#ifndef _C_PAGE_OBJECT_H
#define _C_PAGE_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_page_object.h
	* This file contains class definitions for PageObjectHandle
	*/

	/**
	* \class PageObjectHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief
	* The leaves of the page tree are page objects,
	* each of which is a dictionary specifying the
	* attributes of a single page of the document.
	* \see PageTreeHandle
	*/

	/**
	* \memberof PageObjectHandle
	* @{
	*/

	/**
	* \brief Creates a new instance.
	*
	* Function creates a new indirect dictionary with it's type set to "Page".
	*
	* This new entry has to be manually added to document catalog's page tree.
	* \see PageTreeHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_CreateFromDocument(DocumentHandle handle, PPageObjectHandle result);

	/**
	* \copydoc PageObject_CreateFromDocument
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_CreateFromObject(DictionaryObjectHandle handle, PPageObjectHandle result);

	/**
	* \brief
	* A content stream (see 7.8.2, "Content Streams") that shall describe
	* the contents of this page. If this entry is absent,
	* the page shall be empty.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle handle, PContentsHandle result);

	/**
	* \brief
	* The page tree node that is the immediate parent of this page object.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetParent(PageObjectHandle handle, PPageTreeHandle result);

	/**
	* \brief
	* A dictionary containing any resources required
	* by the page (see 7.8.3, "Resource Dictionaries").
	*
	* If the page requires no resources,
	* the value of this entry shall be an empty dictionary.
	* Omitting the entry entirely indicates that the resources
	* shall be inherited from an ancestor node in the page tree.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetResources(PageObjectHandle handle, PResourceDictionaryHandle result);

	/**
	* \brief
	* An array of annotation dictionaries that shall contain
	* indirect references to all annotations associated
	* with the page (see 12.5, "Annotations").
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetAnnotations(PageObjectHandle handle, PPageAnnotationsHandle result);

	/**
	* \brief
	* A rectangle expressed in default user space units,
	* that shall define the boundaries of the
	* physical medium on which the page shall
	* be displayed or printed (see 14.11.2, "Page Boundaries").
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_GetMediaBox(PageObjectHandle handle, PRectangleHandle result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_OBJECT_H */
