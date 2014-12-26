#ifndef _C_INDIRECT_OBJECT_H
#define _C_INDIRECT_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectObject_GetObject(IndirectHandle handle, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectObject_GetOffset(IndirectHandle handle, out_offset_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectObject_Release(IndirectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_INDIRECT_OBJECT_H */
