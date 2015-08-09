#ifndef _C_OBJECT_H
#define _C_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// TODO i have no idea how export enum without clone
	typedef enum {
		Array,
		Boolean,
		Dictionary,
		Function,
		Integer,
		Name,
		Null,
		Real,
		Stream,
		HexadecimalString,
		LiteralString,
		IndirectReference
	} ObjectType, *PObjectType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Type(ObjectHandle handle, PObjectType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_TypeName(ObjectType type, out_string_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, out_offset_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, PArrayHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle handle, PStreamHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle handle, PDictionaryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle handle, PIndirectReferenceHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle handle, PIntegerHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToLiteralString(ObjectHandle handle, PLiteralStringHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToHexadecimalString(ObjectHandle handle, PHexadecimalStringHandle result);

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_H */
