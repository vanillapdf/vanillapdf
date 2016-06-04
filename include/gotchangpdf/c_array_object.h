#ifndef _C_ARRAY_OBJECT_H
#define _C_ARRAY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayHandle handle, out_integer_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_At(ArrayHandle handle, integer_type at, PObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Append(ArrayHandle handle, ObjectHandle value);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Insert(ArrayHandle handle, integer_type at, ObjectHandle value);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Remove(ArrayHandle handle, integer_type at);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_ARRAY_OBJECT_H */
