#ifndef _C_INTEGER_OBJECT_H
#define _C_INTEGER_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_GetValue(IntegerObjectHandle handle, out_bigint_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_SetValue(IntegerObjectHandle handle, bigint_type value);

	/**
	* \memberof IntegerObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_INTEGER_OBJECT_H */
