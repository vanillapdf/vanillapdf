#ifndef _C_INDIRECT_OBJECT_REFERENCE_H
#define _C_INDIRECT_OBJECT_REFERENCE_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API IndirectObjectHandle CALLING_CONVENTION IndirectReference_GetReferencedObject(IndirectObjectReferenceHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_INDIRECT_OBJECT_REFERENCE_H */
