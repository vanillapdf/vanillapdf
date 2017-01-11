#ifndef _C_DOCUMENT_H
#define _C_DOCUMENT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_document.h
	* This file contains class definitions for DocumentHandle
	*/

	/**
	* \class DocumentHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief Represents high-level file access handle
	*/

	/**
	* \memberof DocumentHandle
	* @{
	*/

	/**
	* \brief Opens a new document for read access
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_OpenNew(string_type filename, PDocumentHandle result);

	/**
	* \brief Opens a document for read access using the already existing file handle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_OpenExisting(FileHandle holder, PDocumentHandle result);

	/**
	* \copydoc FileWriterHandle::FileWriter_Write
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_Save(DocumentHandle handle, string_type filename);

	/**
	* \copydoc FileWriterHandle::FileWriter_WriteIncremental
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_SaveIncremental(DocumentHandle handle, string_type filename);

	/**
	* \brief Get document's catalog property
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_GetCatalog(DocumentHandle handle, PCatalogHandle result);

	/**
	* \brief Get detailed document metadata
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_GetDocumentInfo(DocumentHandle handle, PDocumentInfoHandle result);

	/**
	* \brief Append another document's contents at the end of the file.
	*
	* This appends all \p source document's pages beyond \p handle document's pages.
	*
	* After pages are merged, all indirect referenced are resolved so they point to correct objects.
	*
	* The last step fixes link annotations, which can be referenced by page number.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_AppendContent(DocumentHandle handle, DocumentHandle source);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_Release(DocumentHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_H */
