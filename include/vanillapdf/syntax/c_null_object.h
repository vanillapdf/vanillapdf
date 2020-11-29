#ifndef _C_NULL_OBJECT_H
#define _C_NULL_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_null_object.h
	* \brief This file contains class definitions for \ref NullObjectHandle
	*/

	/**
	* \class NullObjectHandle
	* \extends ObjectHandle
	* \ingroup group_objects
	* \brief The null object has a type and value that are unequal to those of any other object.
	*/

	/**
	* \memberof NullObjectHandle
	* @{
	*/

	/**
	* \brief Creates a new NullObject instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NullObject_Create(NullObjectHandle** result);

	/**
	* \brief Reinterpret current object as \ref ObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NullObject_ToObject(NullObjectHandle* handle, ObjectHandle** result);

	/**
	* \brief Convert \ref ObjectHandle to \ref NullObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NullObject_FromObject(ObjectHandle* handle, NullObjectHandle** result);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NullObject_Release(NullObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NULL_OBJECT_H */
