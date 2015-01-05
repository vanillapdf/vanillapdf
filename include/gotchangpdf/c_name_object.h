#ifndef _C_NAME_OBJECT_H
#define _C_NAME_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Value(NameHandle handle, out_string_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NameObject_Release(NameHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
