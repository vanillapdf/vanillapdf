#ifndef _C_ARRAY_OBJECT_H
#define _C_ARRAY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_array_object.h
	* This file contains class definitions for ArrayObjectHandle
	*/

	/**
	* \class ArrayObjectHandle
	* \extends ObjectHandle
	* \ingroup Objects
	* \brief Represents array of mixed type elements.
	*/

	/**
	* \memberof ArrayObjectHandle
	* @{
	*/

	/**
	* \brief Return size of an array
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle handle, size_type* result);

	/**
	* \brief Get element at location \p at
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle handle, size_type at, ObjectHandle* result);

	/**
	* \brief Insert new element at the end of the array
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Append(ArrayObjectHandle handle, ObjectHandle value);

	/**
	* \brief Insert new element at location \p at
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Insert(ArrayObjectHandle handle, size_type at, ObjectHandle value);

	/**
	* \brief Remove element from location \p at
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Remove(ArrayObjectHandle handle, size_type at);

	/**
	* \copydoc Object_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_ARRAY_OBJECT_H */
