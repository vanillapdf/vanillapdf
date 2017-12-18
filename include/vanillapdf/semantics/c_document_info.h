#ifndef _C_DOCUMENT_INFO_H
#define _C_DOCUMENT_INFO_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_document_info.h
	* \brief This file contains class definitions for \ref DocumentInfoHandle
	*/

	/**
	* \class DocumentInfoHandle
	* \extends IUnknownHandle
	* \ingroup group_documents
	* \brief Metadata for the document
	* \see \ref DocumentHandle
	*/

	/**
	* \brief
	* Indication whether the document has been modified to include
	* trapping information (see 14.11.6, "Trapping Support")
	*/
	typedef enum {
		/**
		* \brief
		* Either it is unknown whether the document has been trapped
		* or it has been partly but not yet fully trapped.
		* Some additional trapping may still be needed.
		*/
		DocumentTrappedType_Unknown = 0,

		/**
		* \brief
		* The document has been fully trapped.
		* No further trapping shall be needed.
		*/
		DocumentTrappedType_True,

		/**
		* \brief The document has not yet been trapped.
		*/
		DocumentTrappedType_False
	} DocumentTrappedType;

	/**
	* \memberof DocumentInfoHandle
	* @{
	*/

	/**
	* \brief The document's title.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetTitle(DocumentInfoHandle* handle, StringObjectHandle** result);

	/**
	* \brief The name of the person who created the document.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetAuthor(DocumentInfoHandle* handle, StringObjectHandle** result);

	/**
	* \brief The subject of the document.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetSubject(DocumentInfoHandle* handle, StringObjectHandle** result);

	/**
	* \brief Keywords associated with the document.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetKeywords(DocumentInfoHandle* handle, StringObjectHandle** result);

	/**
	* \brief
	* If the document was converted to PDF from another format,
	* the name of the conforming product that created the original
	* document from which it was converted.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreator(DocumentInfoHandle* handle, StringObjectHandle** result);

	/**
	* \brief
	* If the document was converted to PDF from another format,
	* the name of the conforming product that converted it to PDF.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetProducer(DocumentInfoHandle* handle, StringObjectHandle** result);

	/**
	* \brief The date and time the document was created.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreationDate(DocumentInfoHandle* handle, DateHandle** result);

	/**
	* \brief The date and time the document was most recently modified.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetModificationDate(DocumentInfoHandle* handle, DateHandle** result);

	/**
	* \copydoc DocumentTrappedType
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_GetTrapped(DocumentInfoHandle* handle, DocumentTrappedType* result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION DocumentInfo_Release(DocumentInfoHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_INFO_H */
