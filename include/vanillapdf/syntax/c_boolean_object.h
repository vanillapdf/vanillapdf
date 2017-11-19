#ifndef _C_BOOLEAN_OBJECT_H
#define _C_BOOLEAN_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_boolean_object.h
	* This file contains class definitions for \ref BooleanObjectHandle
	*/

	/**
	* \class BooleanObjectHandle
	* \extends ObjectHandle
	* \ingroup Objects
	* \brief Represents boolean type
	*/

	/**
	* \memberof BooleanObjectHandle
	* @{
	*/

	/**
	* \brief Return current boolean value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_GetValue(BooleanObjectHandle handle, boolean_type* result);

	/**
	* \brief Set new boolean value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_SetValue(BooleanObjectHandle handle, boolean_type value);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BooleanObject_Release(BooleanObjectHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_BOOLEAN_OBJECT_H */
