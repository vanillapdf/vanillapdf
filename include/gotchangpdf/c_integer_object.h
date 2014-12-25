#ifndef _C_INTEGER_OBJECT_H
#define _C_INTEGER_OBJECT_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API int CALLING_CONVENTION IntegerObject_Value(IntegerObjectHandle);
	GOTCHANG_PDF_API void CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_INTEGER_OBJECT_H */
