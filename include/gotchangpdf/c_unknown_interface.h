#ifndef _C_UNKNOWN_INTERFACE_H
#define _C_UNKNOWN_INTERFACE_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_unknown_interface.h
	* This file contains class definitions for IUnknown interface
	*/

	/**
	* \class IUnknownHandle
	* \ingroup GotchangPDF
	* \brief Base class for reference counting
	*/

	/**
	* \memberof IUnknownHandle
	* @{
	*/

	/**
	* \brief Increment the internal reference counter.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IUnknown_AddRef(IUnknownHandle handle);

	/**
	* \brief Decrement the internal reference counter.
	* 
	* When the internal counter reaches zero the object is deleted.
	* Releasing already disposed object causes undefined behavior.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION IUnknown_Release(IUnknownHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_UNKNOWN_INTERFACE_H */
