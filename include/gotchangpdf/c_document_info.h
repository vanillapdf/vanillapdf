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

	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetTitle(DocumentInfoHandle handle, PStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetAuthor(DocumentInfoHandle handle, PStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetSubject(DocumentInfoHandle handle, PStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetKeywords(DocumentInfoHandle handle, PStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreator(DocumentInfoHandle handle, PStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetProducer(DocumentInfoHandle handle, PStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetCreationDate(DocumentInfoHandle handle, PDateHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetModificationDate(DocumentInfoHandle handle, PDateHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_GetTrapped(DocumentInfoHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION DocumentInfo_Release(DocumentInfoHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_INFO_H */
