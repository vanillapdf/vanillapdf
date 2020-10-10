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
	* \ingroup group_objects
	* \brief Represents unique name references in document.
	* \see \ref group_name_constants
	*/

	/**
	* \memberof NameObjectHandle
	* @{
	*/

	/**
	* \brief Creates a new NameObject instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Create(NameObjectHandle** result);

	/**
	* \brief Get names binary representation
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_GetValue(const NameObjectHandle* handle, BufferHandle** result);

	/**
	* \brief Set names new value
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_SetValue(NameObjectHandle* handle, BufferHandle* value);

	/**
	* \brief Compares two name objects
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Equals(const NameObjectHandle* handle, const NameObjectHandle* other, boolean_type* result);

	/**
	* \brief Get object hash code
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Hash(const NameObjectHandle* handle, size_type* result);

	/**
	* \brief Reinterpret current object as \ref ObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_ToObject(NameObjectHandle* handle, ObjectHandle** result);

	/**
	* \brief Convert \ref ObjectHandle to \ref NameObjectHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_FromObject(ObjectHandle* handle, NameObjectHandle** result);

	/**
	* \copydoc Object_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION NameObject_Release(NameObjectHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_NAME_OBJECT_H */
