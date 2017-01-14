#ifndef _C_RECTANGLE_H
#define _C_RECTANGLE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_rectangle.h
	* This file contains class definitions for RectangleHandle
	*/

	/**
	* \class RectangleHandle
	* \extends IUnknownHandle
	* \ingroup CommonDataStructures
	* \brief Represents rectangle in 2D space
	*/

	/**
	* \memberof RectangleHandle
	* @{
	*/

	/**
	* \brief Get X-coordinate of lower-left corner
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_LowerLeftX(RectangleHandle handle, IntegerObjectHandle* result);

	/**
	* \brief Get Y-coordinate of lower-left corner
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_LowerLeftY(RectangleHandle handle, IntegerObjectHandle* result);

	/**
	* \brief Get X-coordinate of upper-right corner
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_UpperRightX(RectangleHandle handle, IntegerObjectHandle* result);

	/**
	* \brief Get Y-coordinate of upper-right corner
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_UpperRightY(RectangleHandle handle, IntegerObjectHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see ::IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Rectangle_Release(RectangleHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_RECTANGLE_H */
