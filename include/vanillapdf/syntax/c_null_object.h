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
	* \ingroup Objects
	* \brief Used as missing value.
	*/

	/**
	* \memberof NullObjectHandle
	* @{
	*/

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NullObject_Release(NullObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NULL_OBJECT_H */
