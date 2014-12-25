#ifndef _C_FILE_H
#define _C_FILE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Create(const char *filename, PFileHandle result);
	GOTCHANG_PDF_API void CALLING_CONVENTION File_Release(FileHandle);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Xref(FileHandle, PXrefHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetIndirectObject(FileHandle, int objNumber, int genNumber, PIndirectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetDocumentCatalog(FileHandle, PCatalogHandle result);

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_H */
