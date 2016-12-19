#ifndef _C_REAL_OBJECT_H
#define _C_REAL_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \addtogroup RealObject
	* @{
	*/

	GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_GetValue(RealHandle handle, out_real_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_SetValue(RealHandle handle, real_type value);

	/**
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_Release(RealHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_REAL_OBJECT_H */
