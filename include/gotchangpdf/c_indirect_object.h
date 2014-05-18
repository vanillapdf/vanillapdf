#ifndef _C_INDIRECT_OBJECT_H
#define _C_INDIRECT_OBJECT_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION IndirectObject_GetObject(IndirectObjectHandle);
	GOTCHANG_PDF_API long long CALLING_CONVENTION IndirectObject_GetOffset(IndirectObjectHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION IndirectObject_Release(IndirectObjectHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_INDIRECT_OBJECT_H */
