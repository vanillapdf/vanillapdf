#ifndef _C_DOCUMENT_INFO_H
#define _C_DOCUMENT_INFO_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_document_info.h
	* This file contains class definitions for DocumentInfoHandle
	*/

	/**
	* \brief
	* Indication whether the document has been modified to include
	* trapping information (see 14.11.6, "Trapping Support")
	*/
	typedef enum
	{
		/**
		* \brief
		* Either it is unknown whether the document has been trapped
		* or it has been partly but not yet fully trapped.
		* Some additional trapping may still be needed.
		*/
		DocumentTrapped_Unknown = 0,

		/**
		* \brief
		* The document has been fully trapped.
		* No further trapping shall be needed.
		*/
		DocumentTrapped_True,

		/**
		* \brief The document has not yet been trapped.
		*/
		DocumentTrapped_False
	} DocumentTrapped;

	/**
	* \brief Pointer to DocumentTrapped
	*/
	typedef DocumentTrapped *PDocumentTrapped;

	/**
	* \memberof DocumentInfoHandle
	* @{
	*/

	/**
	* \brief The document's title.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetTitle(DocumentInfoHandle handle, PStringObjectHandle result);

	/**
	* \brief The name of the person who created the document.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetAuthor(DocumentInfoHandle handle, PStringObjectHandle result);

	/**
	* \brief The subject of the document.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetSubject(DocumentInfoHandle handle, PStringObjectHandle result);

	/**
	* \brief Keywords associated with the document.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetKeywords(DocumentInfoHandle handle, PStringObjectHandle result);

	/**
	* \brief
	* If the document was converted to PDF from another format,
	* the name of the conforming product that created the original
	* document from which it was converted.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreator(DocumentInfoHandle handle, PStringObjectHandle result);

	/**
	* \brief
	* If the document was converted to PDF from another format,
	* the name of the conforming product that converted it to PDF.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetProducer(DocumentInfoHandle handle, PStringObjectHandle result);

	/**
	* \brief The date and time the document was created.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreationDate(DocumentInfoHandle handle, PDateHandle result);

	/**
	* \brief The date and time the document was most recently modified.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetModificationDate(DocumentInfoHandle handle, PDateHandle result);

	/**
	* \copydoc DocumentTrapped
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetTrapped(DocumentInfoHandle handle, PDocumentTrapped result);

	/**
	* \copydoc IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_Release(DocumentInfoHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_INFO_H */
