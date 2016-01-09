#ifndef _C_FILE_H
#define _C_FILE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Open(string_type filename, PFileHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Initialize(FileHandle handle);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_XrefChain(FileHandle handle, PXrefChainHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_GetIndirectObject(FileHandle handle, integer_type objNumber, ushort_type genNumber, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION File_Release(FileHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_FILE_H */
