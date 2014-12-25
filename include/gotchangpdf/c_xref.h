#ifndef _C_XREF_H
#define _C_XREF_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API int CALLING_CONVENTION XrefEntry_In_Use(XrefEntryHandle handle);
	GOTCHANG_PDF_API IndirectHandle CALLING_CONVENTION XrefEntry_Reference(XrefEntryHandle handle);
	GOTCHANG_PDF_API void CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle);

	GOTCHANG_PDF_API int CALLING_CONVENTION Xref_Size(XrefHandle);
	GOTCHANG_PDF_API XrefEntryHandle CALLING_CONVENTION Xref_At(XrefHandle, int at);
	GOTCHANG_PDF_API void CALLING_CONVENTION Xref_Release(XrefHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_XREF_H */
