#ifndef _C_UNKNOWN_INTERFACE_H
#define _C_UNKNOWN_INTERFACE_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_unknown_interface.h
	* \brief This file contains class definitions for \ref IUnknownHandle interface
	*/

	/**
	* \class IUnknownHandle
	* \ingroup group_utils
	* \brief Base class for reference counting
	*/

	/**
	* \memberof IUnknownHandle
	* @{
	*/

	/**
	* \brief Increment the internal reference counter.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IUnknown_AddRef(IUnknownHandle* handle);

	/**
	* \brief Decrement the internal reference counter.
	* 
	* When the internal counter reaches zero the object is deleted.
	* Releasing already disposed object causes <b>undefined behavior</b>.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION IUnknown_Release(IUnknownHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_UNKNOWN_INTERFACE_H */
