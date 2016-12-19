#ifndef _C_OBJECT_H
#define _C_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \addtogroup Objects
	* @{
	*/

	typedef enum
	{
		ObjectType_Null = 0,
		ObjectType_Array,
		ObjectType_Boolean,
		ObjectType_Dictionary,
		ObjectType_Integer,
		ObjectType_Name,
		ObjectType_Real,
		ObjectType_Stream,
		ObjectType_String,
		ObjectType_IndirectReference
	} ObjectType, *PObjectType;

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Type(ObjectHandle handle, PObjectType result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_TypeName(ObjectType type, out_string_type result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, out_offset_type result);

	/**
	* \brief Decrement the internal reference counter.
	*
	* All objects available via API implements some sort of IUnknown
	* interface. When the internal counter reaches zero the object
	* is deleted.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle);

	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, PArrayHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle handle, PStreamHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle handle, PDictionaryHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle handle, PIndirectReferenceHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle handle, PIntegerHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToBoolean(ObjectHandle handle, PBooleanHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToReal(ObjectHandle handle, PRealHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToNull(ObjectHandle handle, PNullHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, PNameHandle result);
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle handle, PStringHandle result);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_H */
