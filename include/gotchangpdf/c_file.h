#ifndef _C_FILE_H
#define _C_FILE_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API FileHandle CALLING_CONVENTION File_Create(const char *filename);
	GOTCHANG_PDF_API void CALLING_CONVENTION File_Release(FileHandle);
	GOTCHANG_PDF_API int CALLING_CONVENTION File_Initialize(FileHandle);
	GOTCHANG_PDF_API XrefHandle CALLING_CONVENTION File_Xref(FileHandle);
	GOTCHANG_PDF_API IndirectHandle CALLING_CONVENTION File_GetIndirectObject(FileHandle, int objNumber, int genNumber);
	GOTCHANG_PDF_API CatalogHandle CALLING_CONVENTION File_GetDocumentCatalog(FileHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_H */
