#ifndef _C_ARRAY_OBJECT_H
#define _C_ARRAY_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_array_object.h
	* This file contains class definitions for ArrayObjectHandle
	*/

	/**
	* \brief Return size of an array
	* \memberof ArrayObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Size(ArrayObjectHandle handle, out_integer_type result);

	/**
	* \brief Get element at location \p at
	* \memberof ArrayObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_At(ArrayObjectHandle handle, integer_type at, PObjectHandle result);

	/**
	* \brief Insert new element at the end of the array
	* \memberof ArrayObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Append(ArrayObjectHandle handle, ObjectHandle value);

	/**
	* \brief Insert new element at location \p at
	* \memberof ArrayObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Insert(ArrayObjectHandle handle, integer_type at, ObjectHandle value);

	/**
	* \brief Remove element from location \p at
	* \memberof ArrayObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Remove(ArrayObjectHandle handle, integer_type at);

	/**
	* \memberof ArrayObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ArrayObject_Release(ArrayObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_ARRAY_OBJECT_H */
