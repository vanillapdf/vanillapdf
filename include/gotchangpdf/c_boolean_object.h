#ifndef _C_BOOLEAN_OBJECT_H
#define _C_BOOLEAN_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_Value(BooleanHandle handle, out_boolean_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_Release(BooleanHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_BOOLEAN_OBJECT_H */
