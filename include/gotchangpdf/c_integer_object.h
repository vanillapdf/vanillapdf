#ifndef _C_INTEGER_OBJECT_H
#define _C_INTEGER_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_integer_object.h
	* This file contains class definitions for IntegerObjectHandle
	*/

	/**
	* \brief Return objects contained integer value
	* \memberof IntegerObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_GetIntegerValue(IntegerObjectHandle handle, out_bigint_type result);

	/**
	* \brief Return objects contained unsigned integer value
	* \memberof IntegerObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_GetUnsignedIntegerValue(IntegerObjectHandle handle, out_biguint_type result);

	/**
	* \brief Set objects new integer value
	* \memberof IntegerObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_SetIntegerValue(IntegerObjectHandle handle, bigint_type value);

	/**
	* \brief Set objects new unsigned integer value
	* \memberof IntegerObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_SetUnsignedIntegerValue(IntegerObjectHandle handle, biguint_type value);

	/**
	* \memberof IntegerObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_INTEGER_OBJECT_H */
