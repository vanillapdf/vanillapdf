#ifndef _C_OBJECT_ATTRIBUTE_LIST_H
#define _C_OBJECT_ATTRIBUTE_LIST_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

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
	* \ingroup group_utils
	* \brief Represents a list of object attributes
	*/

	/**
	* \memberof ObjectAttributeListHandle
	* @{
	*/

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
