#ifndef _C_STRING_OBJECT_H
#define _C_STRING_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum {
		StringType_Literal = 0,
		StringType_Hexadecimal = 1
	} StringType, *PStringType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_Type(StringHandle handle, PStringType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToLiteral(StringHandle handle, PLiteralStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToHexadecimal(StringHandle handle, PHexadecimalStringHandle result);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Value(LiteralStringHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Value(HexadecimalStringHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_STRING_OBJECT_H */
