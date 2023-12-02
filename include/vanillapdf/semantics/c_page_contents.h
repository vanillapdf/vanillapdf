#ifndef _C_CONTENTS_H
#define _C_CONTENTS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_page_contents.h
	* \brief This file contains class definitions for working with compressed page contents
	*/

	/**
	* \class PageContentsHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
	* \brief
	* A content stream is a PDF stream object whose data consists of
	* a sequence of instructions describing the graphical elements
	* to be painted on a page.
	* \see \ref PageObjectHandle
	*/

	/**
	* \memberof PageContentsHandle
	* @{
	*/

	/**
	* \brief Read the collection of instructions within the page contents
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageContents_GetInstructionCollection(PageContentsHandle* handle, ContentInstructionCollectionHandle** result);

	/**
	* \brief Updates the referenced stream content to reflect content operation changes
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageContents_RecalculateStreamData(PageContentsHandle* handle, boolean_type* result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageContents_ToUnknown(PageContentsHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref PageContentsHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageContents_FromUnknown(IUnknownHandle* handle, PageContentsHandle** result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION PageContents_Release(PageContentsHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_CONTENTS_H */
