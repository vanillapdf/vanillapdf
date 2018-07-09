#ifndef _C_ARRAY_OBJECT_H
#define _C_ARRAY_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_array_object.h
	* \brief This file contains class definitions for \ref ArrayObjectHandle
	*/

	/**
	* \class ArrayObjectHandle
	* \extends ObjectHandle
	* \ingroup group_objects
	* \brief Represents array of mixed type elements.
	*/

	/**
	* \memberof ArrayObjectHandle
	* @{
	*/

	/**
	* \brief Creates a new ArrayObject instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Create(ArrayObjectHandle** result);

	/**
	* \brief Return size of an array
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle* handle, size_type* result);

	/**
	* \brief Get element at location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle* handle, size_type at, ObjectHandle** result);

	/**
	* \brief Insert new element at the end of the array
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Append(ArrayObjectHandle* handle, ObjectHandle* value);

	/**
	* \brief Insert new element at location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Insert(ArrayObjectHandle* handle, size_type at, ObjectHandle* value);

	/**
	* \brief Remove element from location \p at
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Remove(ArrayObjectHandle* handle, size_type at);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_ARRAY_OBJECT_H */
