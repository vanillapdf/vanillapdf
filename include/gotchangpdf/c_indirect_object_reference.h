#ifndef _C_INDIRECT_OBJECT_REFERENCE_H
#define _C_INDIRECT_OBJECT_REFERENCE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectReferenceHandle handle, PIndirectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IndirectReference_Release(IndirectReferenceHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_INDIRECT_OBJECT_REFERENCE_H */
