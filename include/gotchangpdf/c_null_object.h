#ifndef _C_NULL_OBJECT_H
#define _C_NULL_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_null_object.h
	* This file contains class definitions for NullObjectHandle
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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION NullObject_Release(NullObjectHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NULL_OBJECT_H */
