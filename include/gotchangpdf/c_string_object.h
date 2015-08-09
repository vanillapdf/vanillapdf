#ifndef _C_STRING_OBJECT_H
#define _C_STRING_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Value(LiteralStringHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Value(HexadecimalStringHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_STRING_OBJECT_H */
