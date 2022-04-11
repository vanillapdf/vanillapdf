#ifndef _C_PAGE_OBJECT_H
#define _C_PAGE_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_page_object.h
	* \brief This file contains class definitions for \ref PageObjectHandle
	*/

	/**
	* \class PageObjectHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
	* \brief
	* The leaves of the page tree are page objects,
	* each of which is a dictionary specifying the
	* attributes of a single page of the document.
	* \see \ref PageTreeHandle
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
	* \see \ref PageTreeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_CreateFromDocument(DocumentHandle* handle, PageObjectHandle** result);

	/**
	* \copydoc PageObject_CreateFromDocument
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_CreateFromObject(DictionaryObjectHandle* handle, PageObjectHandle** result);

	/**
	* \brief
	* A content stream (see 7.8.2, "Content Streams") that shall describe
	* the contents of this page. If this entry is absent, the page shall be empty.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetContents(PageObjectHandle* handle, PageContentsHandle** result);

	/**
	* \brief
	* The page tree node that is the immediate parent of this page object.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetParent(PageObjectHandle* handle, PageTreeHandle** result);

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
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetResources(PageObjectHandle* handle, ResourceDictionaryHandle** result);

	/**
	* \brief
	* An array of annotation dictionaries that shall contain
	* indirect references to all annotations associated
	* with the page (see 12.5, "Annotations").
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetAnnotations(PageObjectHandle* handle, PageAnnotationsHandle** result);

	/**
	* \brief
	* A rectangle expressed in default user space units,
	* that shall define the boundaries of the
	* physical medium on which the page shall
	* be displayed or printed (see 14.11.2, "Page Boundaries").
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_GetMediaBox(PageObjectHandle* handle, RectangleHandle** result);

	/**
	* \copydoc PageObject_GetMediaBox
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_SetMediaBox(PageObjectHandle* handle, RectangleHandle* value);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_ToUnknown(PageObjectHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref PageObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_FromUnknown(IUnknownHandle* handle, PageObjectHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageObject_Release(PageObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PAGE_OBJECT_H */
