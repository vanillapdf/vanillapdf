#ifndef _C_STRING_OBJECT_H
#define _C_STRING_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum
	{
		StringType_Literal = 0,
		StringType_Hexadecimal
	} StringType, *PStringType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_Type(StringObjectHandle handle, PStringType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToLiteral(StringObjectHandle handle, PLiteralStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_ToHexadecimal(StringObjectHandle handle, PHexadecimalStringObjectHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_GetValue(StringObjectHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_SetValue(StringObjectHandle handle, BufferHandle value);

	/**
	* \memberof StringObject
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION StringObject_Release(StringObjectHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_GetValue(LiteralStringObjectHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_SetValue(LiteralStringObjectHandle handle, BufferHandle value);

	/**
	* \memberof LiteralStringObject
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION LiteralStringObject_Release(LiteralStringObjectHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_GetValue(HexadecimalStringObjectHandle handle, PBufferHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_SetValue(HexadecimalStringObjectHandle handle, BufferHandle value);

	/**
	* \memberof HexadecimalStringObject
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION HexadecimalStringObject_Release(HexadecimalStringObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_STRING_OBJECT_H */
