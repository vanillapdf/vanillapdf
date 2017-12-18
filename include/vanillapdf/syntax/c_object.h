#ifndef _C_OBJECT_H
#define _C_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_object.h
	* \brief This file contains contains class definitions for \ref ObjectHandle
	*/

	/**
	* \class ObjectHandle
	* \extends IUnknownHandle
	* \ingroup group_objects
	* \brief Base class for syntactic tokens
	*/

	/**
	* \brief Available types of objects.
	*/
	typedef enum {
		/**
		* \copybrief NullObjectHandle
		* \see \ref NullObjectHandle
		*/
		ObjectType_Null = 0,

		/**
		* \copybrief ArrayObjectHandle
		* \see \ref ArrayObjectHandle
		*/
		ObjectType_Array,

		/**
		* \copybrief BooleanObjectHandle
		* \see \ref BooleanObjectHandle
		*/
		ObjectType_Boolean,

		/**
		* \copybrief DictionaryObjectHandle
		* \see \ref DictionaryObjectHandle
		*/
		ObjectType_Dictionary,

		/**
		* \copybrief IntegerObjectHandle
		* \see \ref IntegerObjectHandle
		*/
		ObjectType_Integer,

		/**
		* \copybrief NameObjectHandle
		* \see \ref NameObjectHandle
		*/
		ObjectType_Name,

		/**
		* \copybrief RealObjectHandle
		* \see \ref RealObjectHandle
		*/
		ObjectType_Real,

		/**
		* \copybrief StreamObjectHandle
		* \see \ref StreamObjectHandle
		*/
		ObjectType_Stream,

		/**
		* \copybrief StringObjectHandle
		* \see \ref StringObjectHandle
		*/
		ObjectType_String,

		/**
		* \copybrief IndirectObjectReferenceHandle
		* \see \ref IndirectObjectReferenceHandle
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
	VANILLAPDF_API error_type CALLING_CONVENTION Object_GetType(ObjectHandle* handle, ObjectType* result);

	/**
	* \brief Get string representation of object type
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_TypeName(ObjectType type, string_type* result);

	/**
	* \brief Get input file offset where this object was found
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_GetOffset(ObjectHandle* handle, offset_type* result);

	/**
	* \copydoc IUnknown_Release
	* \see \ref IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_Release(ObjectHandle* handle);

	/**
	* \brief Reinterpret current object as \ref ArrayObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToArray(ObjectHandle* handle, ArrayObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref StreamObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToStream(ObjectHandle* handle, StreamObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref DictionaryObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToDictionary(ObjectHandle* handle, DictionaryObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref IndirectObjectReferenceHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToIndirectReference(ObjectHandle* handle, IndirectObjectReferenceHandle** result);

	/**
	* \brief Reinterpret current object as \ref IntegerObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToInteger(ObjectHandle* handle, IntegerObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref BooleanObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToBoolean(ObjectHandle* handle, BooleanObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref RealObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToReal(ObjectHandle* handle, RealObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref NullObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToNull(ObjectHandle* handle, NullObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref NameObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToName(ObjectHandle* handle, NameObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref StringObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Object_ToString(ObjectHandle* handle, StringObjectHandle** result);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_H */
