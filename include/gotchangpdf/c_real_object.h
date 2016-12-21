#ifndef _C_REAL_OBJECT_H
#define _C_REAL_OBJECT_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/** \file c_real_object.h
	* This file contains contains class definitions for RealObjectHandle
	*/

	/**
	* \brief Return objects contained real value
	* \memberof RealObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_GetValue(RealObjectHandle handle, out_real_type result);

	/**
	* \brief Set objects new real value
	* \memberof RealObjectHandle
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_SetValue(RealObjectHandle handle, real_type value);

	/**
	* \memberof RealObjectHandle
	* \see Object_Release()
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION RealObject_Release(RealObjectHandle handle);

#ifdef __cplusplus
};
#endif

#endif /* _C_REAL_OBJECT_H */
