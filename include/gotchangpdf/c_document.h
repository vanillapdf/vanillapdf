#ifndef _C_DOCUMENT_H
#define _C_DOCUMENT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_OpenNew(string_type filename, PDocumentHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_OpenExisting(FileHolderHandle holder, PDocumentHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_GetCatalog(DocumentHandle handle, PCatalogHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Document_Release(DocumentHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_DOCUMENT_H */
