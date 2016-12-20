#ifndef _C_RECTANGLE_H
#define _C_RECTANGLE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_LowerLeftX(RectangleHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_LowerLeftY(RectangleHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_UpperRightX(RectangleHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_UpperRightY(RectangleHandle handle, PIntegerObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_Release(RectangleHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_RECTANGLE_H */
