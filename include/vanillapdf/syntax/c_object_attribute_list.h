#ifndef _C_OBJECT_ATTRIBUTE_LIST_H
#define _C_OBJECT_ATTRIBUTE_LIST_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#include "vanillapdf/syntax/c_object_attributes.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_object_attributes.h
	* \brief This file contains class definitions for multiple object attributes.
	*/

	/**
	* \class ObjectAttributeListHandle
	* \extends IUnknownHandle
	* \ingroup group_objects
	* \brief Represents a list of object attributes
	* \see \ref BaseObjectAttributeHandle
	*/

	/**
	* \memberof ObjectAttributeListHandle
	* @{
	*/

	/**
	* \brief Create new ObjectAttributeList instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Create(ObjectAttributeListHandle** result);

	/**
	* \brief Adds an element with the provided key and value to the list
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Add(ObjectAttributeListHandle* handle, BaseObjectAttributeHandle* value, boolean_type overwrite);

	/**
	* \brief Removes the element with the specified key from the list
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Remove(ObjectAttributeListHandle* handle, ObjectAttributeType key, boolean_type* result);

	/**
	* \brief Determine if collection contains \p key
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Contains(ObjectAttributeListHandle* handle, ObjectAttributeType key, boolean_type* result);

	/**
	* \brief Find mapped value for key \p key
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Get(ObjectAttributeListHandle* handle, ObjectAttributeType key, BaseObjectAttributeHandle** result);

	/**
	* \brief Remove all items from the collection
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Clear(ObjectAttributeListHandle* handle);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_ToUnknown(ObjectAttributeListHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref ObjectAttributeListHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_FromUnknown(IUnknownHandle* handle, ObjectAttributeListHandle** result);

	/**
	* \copydoc IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Release(ObjectAttributeListHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_ATTRIBUTE_LIST_H */
