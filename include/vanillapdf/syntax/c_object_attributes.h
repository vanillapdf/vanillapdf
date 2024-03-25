#ifndef _C_OBJECT_ATTRIBUTES_H
#define _C_OBJECT_ATTRIBUTES_H

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
	* \class BaseObjectAttributeHandle
	* \extends IUnknownHandle
	* \ingroup group_objects
	* \brief Represents a base class for representing object attributes
	*/

	/**
	* \brief Derived types of \ref BaseObjectAttributeHandle
	* \ingroup group_objects
	*/
	typedef enum {

		/**
		* \brief Undefined unitialized default value, triggers error when used
		*/
		ObjectAttributeType_Undefined = 0,
		ObjectAttributeType_Empty,
		ObjectAttributeType_SerializationOverride,
		ObjectAttributeType_TrackingIdentifier,

		// Image attributes
		ObjectAttributeType_ImageColorSpace = 0x00001000,
		ObjectAttributeType_ImageColorComponents

	} ObjectAttributeType;

	/**
	* \memberof BaseObjectAttributeHandle
	* @{
	*/

	VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_GetAtrributeType(BaseObjectAttributeHandle* handle, ObjectAttributeType* result);

	/**
	* \brief Reinterpret current object as \ref IUnknownHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_ToUnknown(BaseObjectAttributeHandle* handle, IUnknownHandle** result);

	/**
	* \brief Convert \ref IUnknownHandle to \ref BaseObjectAttributeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_FromUnknown(IUnknownHandle* handle, BaseObjectAttributeHandle** result);

	/**
	* \copydoc IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_Release(BaseObjectAttributeHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_ATTRIBUTES_H */
