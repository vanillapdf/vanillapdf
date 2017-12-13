#ifndef _C_NAME_OBJECT_H
#define _C_NAME_OBJECT_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_name_object.h
	* \brief This file contains class definitions for \ref NameObjectHandle
	*/

	/**
	* \class NameObjectHandle
	* \extends ObjectHandle
	* \ingroup Objects
	* \brief Represents unique name references in document.
	* \see \ref NameConstants
	*/

	/**
	* \memberof NameObjectHandle
	* @{
	*/

	/**
	* \brief Get names binary representation
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_GetValue(const NameObjectHandle* handle, BufferHandle** result);

	/**
	* \brief Set names new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_SetValue(NameObjectHandle* handle, BufferHandle* value);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Release(NameObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
