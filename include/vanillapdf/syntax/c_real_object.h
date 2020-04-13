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
	* \brief This file contains class definitions for \ref RealObjectHandle
	*/

	/**
	* \class RealObjectHandle
	* \extends ObjectHandle
	* \ingroup group_objects
	* \brief Represents floating point numbers.
	*/

	/**
	* \memberof RealObjectHandle
	* @{
	*/

	/**
	* \brief Creates a new RealObject instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_Create(RealObjectHandle** result);

	/**
	* \brief Return objects contained real value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_GetValue(RealObjectHandle* handle, real_type* result);

	/**
	* \brief Set objects new real value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_SetValue(RealObjectHandle* handle, real_type value);

	/**
	* \brief Reinterpret current object as \ref ObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_ToObject(RealObjectHandle* handle, ObjectHandle** result);

	/**
	* \brief Convert \ref ObjectHandle to \ref RealObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_FromObject(ObjectHandle* handle, RealObjectHandle** result);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION RealObject_Release(RealObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_REAL_OBJECT_H */
