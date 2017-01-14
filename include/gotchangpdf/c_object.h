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
	* \file c_object.h
	* This file contains contains class definitions for ObjectHandle
	*/

	/**
	* \class ObjectHandle
	* \extends IUnknownHandle
	* \ingroup Objects
	* \brief Base class for syntactic tokens
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
	* \memberof ObjectHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetType(ObjectHandle handle, ObjectType* result);

	/**
	* \brief Get string representation of object type
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_TypeName(ObjectType type, string_type* result);

	/**
	* \brief Get input file offset where this object was found
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle handle, offset_type* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle handle);

	/**
	* \brief Reinterpret current object as ArrayObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle handle, ArrayObjectHandle* result);

	/**
	* \brief Reinterpret current object as StreamObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle handle, StreamObjectHandle* result);

	/**
	* \brief Reinterpret current object as DictionaryObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle handle, DictionaryObjectHandle* result);

	/**
	* \brief Reinterpret current object as IndirectObjectReferenceHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle handle, IndirectObjectReferenceHandle* result);

	/**
	* \brief Reinterpret current object as IntegerObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle handle, IntegerObjectHandle* result);

	/**
	* \brief Reinterpret current object as BooleanObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToBoolean(ObjectHandle handle, BooleanObjectHandle* result);

	/**
	* \brief Reinterpret current object as RealObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToReal(ObjectHandle handle, RealObjectHandle* result);

	/**
	* \brief Reinterpret current object as NullObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToNull(ObjectHandle handle, NullObjectHandle* result);

	/**
	* \brief Reinterpret current object as NameObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle handle, NameObjectHandle* result);

	/**
	* \brief Reinterpret current object as StringObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle handle, StringObjectHandle* result);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_H */
