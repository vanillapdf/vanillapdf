#ifndef _C_OBJECT_H
#define _C_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_object.h
	* This file contains contains class definitions for ObjectHandle
	*/

	/**
	* \brief Available types of objects.
	*/
	typedef enum
	{
		/**
		* \copybrief NullObjectHandle
		* \see NullObjectHandle
		*/
		ObjectType_Null = 0,

		/**
		* \copybrief ArrayObjectHandle
		* \see ArrayObjectHandle
		*/
		ObjectType_Array,

		/**
		* \copybrief BooleanObjectHandle
		* \see BooleanObjectHandle
		*/
		ObjectType_Boolean,

		/**
		* \copybrief DictionaryObjectHandle
		* \see DictionaryObjectHandle
		*/
		ObjectType_Dictionary,

		/**
		* \copybrief IntegerObjectHandle
		* \see IntegerObjectHandle
		*/
		ObjectType_Integer,

		/**
		* \copybrief NameObjectHandle
		* \see NameObjectHandle
		*/
		ObjectType_Name,

		/**
		* \copybrief RealObjectHandle
		* \see RealObjectHandle
		*/
		ObjectType_Real,

		/**
		* \copybrief StreamObjectHandle
		* \see StreamObjectHandle
		*/
		ObjectType_Stream,

		/**
		* \copybrief StringObjectHandle
		* \see StringObjectHandle
		*/
		ObjectType_String,

		/**
		* \copybrief IndirectObjectReferenceHandle
		* \see IndirectObjectReferenceHandle
		*/
		ObjectType_IndirectReference
	} ObjectType;

	/**
	* \brief Pointer to ObjectType
	*/
	typedef ObjectType *PObjectType;

	/**
	* \memberof ObjectHandle
	* @{
	*/

	/**
	* \brief Get derived type from current object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Type(ObjectHandle handle, PObjectType result);

	/**
	* \brief Get string representation of object type
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_TypeName(ObjectType type, out_string_type result);

	/**
	* \brief Get input file offset where this object was found
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, out_offset_type result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle);

	/**
	* \brief Reinterpret current object as ArrayObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, PArrayObjectHandle result);

	/**
	* \brief Reinterpret current object as StreamObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle handle, PStreamObjectHandle result);

	/**
	* \brief Reinterpret current object as DictionaryObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle handle, PDictionaryObjectHandle result);

	/**
	* \brief Reinterpret current object as IndirectObjectReferenceHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle handle, PIndirectObjectReferenceHandle result);

	/**
	* \brief Reinterpret current object as IntegerObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle handle, PIntegerObjectHandle result);

	/**
	* \brief Reinterpret current object as BooleanObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToBoolean(ObjectHandle handle, PBooleanObjectHandle result);

	/**
	* \brief Reinterpret current object as RealObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToReal(ObjectHandle handle, PRealObjectHandle result);

	/**
	* \brief Reinterpret current object as NullObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToNull(ObjectHandle handle, PNullObjectHandle result);

	/**
	* \brief Reinterpret current object as NameObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, PNameObjectHandle result);

	/**
	* \brief Reinterpret current object as StringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle handle, PStringObjectHandle result);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_H */
