#ifndef _C_REAL_OBJECT_H
#define _C_REAL_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_real_object.h
	* This file contains class definitions for \ref RealObjectHandle
	*/

	/**
	* \class RealObjectHandle
	* \extends ObjectHandle
	* \ingroup Objects
	* \brief Represents floating point numbers.
	*/

	/**
	* \memberof RealObjectHandle
	* @{
	*/

	/**
	* \brief Return objects contained real value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_GetValue(RealObjectHandle* handle, real_type* result);

	/**
	* \brief Set objects new real value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_SetValue(RealObjectHandle* handle, real_type value);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_Release(RealObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_REAL_OBJECT_H */
