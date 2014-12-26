#ifndef _C_XREF_H
#define _C_XREF_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_In_Use(XrefEntryHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Reference(XrefEntryHandle handle, PIndirectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION XrefEntry_Release(XrefEntryHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Size(XrefHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_At(XrefHandle handle, integer_type at, PXrefEntryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Xref_Release(XrefHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_XREF_H */
