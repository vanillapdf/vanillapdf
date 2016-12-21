#ifndef _C_BOOLEAN_OBJECT_H
#define _C_BOOLEAN_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_boolean_object.h
	* This file contains contains class definitions for BooleanObjectHandle
	*/

	/**
	* \brief Return current boolean value
	* \memberof BooleanObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_GetValue(BooleanObjectHandle handle, out_boolean_type result);

	/**
	* \brief Set new boolean value
	* \memberof BooleanObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_SetValue(BooleanObjectHandle handle, boolean_type value);

	/**
	* \memberof BooleanObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION BooleanObject_Release(BooleanObjectHandle handle);


#ifdef __cplusplus
};
#endif

#endif /* _C_BOOLEAN_OBJECT_H */
