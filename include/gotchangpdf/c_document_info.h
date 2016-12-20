#ifndef _C_DOCUMENT_INFO_H
#define _C_DOCUMENT_INFO_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#include "c_version.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		DocumentTrapped_Unknown = 0,
		DocumentTrapped_True,
		DocumentTrapped_False,
	} DocumentTrapped, *PDocumentTrapped;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetTitle(DocumentInfoHandle handle, PStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetAuthor(DocumentInfoHandle handle, PStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetSubject(DocumentInfoHandle handle, PStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetKeywords(DocumentInfoHandle handle, PStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreator(DocumentInfoHandle handle, PStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetProducer(DocumentInfoHandle handle, PStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreationDate(DocumentInfoHandle handle, PDateHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetModificationDate(DocumentInfoHandle handle, PDateHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetTrapped(DocumentInfoHandle handle, PDocumentTrapped result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_Release(DocumentInfoHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_INFO_H */
