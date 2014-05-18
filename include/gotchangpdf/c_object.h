#ifndef _C_OBJECT_H
#define _C_OBJECT_H

#include "c_export.h"
#include "c_handles.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API enum ObjectType CALLING_CONVENTION Object_Type(ObjectHandle);
	GOTCHANG_PDF_API const char* Object_TypeName(enum ObjectType type);
	GOTCHANG_PDF_API void CALLING_CONVENTION Object_Release(ObjectHandle);

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_H */
