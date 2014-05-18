#ifndef _C_ARRAY_OBJECT_H
#define _C_ARRAY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API int CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle);
	GOTCHANG_PDF_API ObjectHandle CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle, int at);
	GOTCHANG_PDF_API void CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_ARRAY_OBJECT_H */
