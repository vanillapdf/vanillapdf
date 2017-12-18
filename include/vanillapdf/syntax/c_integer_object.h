#ifndef _C_INTEGER_OBJECT_H
#define _C_INTEGER_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_integer_object.h
	* \brief This file contains class definitions for \ref IntegerObjectHandle
	*/

	/**
	* \class IntegerObjectHandle
	* \extends ObjectHandle
	* \ingroup group_objects
	* \brief Represents integer values
	*/

	/**
	* \memberof IntegerObjectHandle
	* @{
	*/

	/**
	* \brief Return objects contained integer value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_GetIntegerValue(IntegerObjectHandle* handle, bigint_type* result);

	/**
	* \brief Return objects contained unsigned integer value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_GetUnsignedIntegerValue(IntegerObjectHandle* handle, biguint_type* result);

	/**
	* \brief Set objects new integer value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_SetIntegerValue(IntegerObjectHandle* handle, bigint_type value);

	/**
	* \brief Set objects new unsigned integer value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_SetUnsignedIntegerValue(IntegerObjectHandle* handle, biguint_type value);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IntegerObject_Release(IntegerObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INTEGER_OBJECT_H */
