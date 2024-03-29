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
	* \brief Base class for representing object attributes that are augumenting specific properties with additional metadata
	*/

	/**
	* \class ImageMetadataObjectAttributeHandle
	* \extends BaseObjectAttributeHandle
	* \ingroup group_objects
	* \brief Attribute object contains information about image colorspace and components
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
		ObjectAttributeType_ImageMetadata

	} ObjectAttributeType;

	/**
	* \memberof BaseObjectAttributeHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object attribute
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

	/**
	* \memberof ImageMetadataObjectAttributeHandle
	* @{
	*/

	/**
	* \brief Get number of color components inside the associated image
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageColorComponentsObjectAttribute_GetColorComponents(ImageMetadataObjectAttributeHandle* handle, integer_type* result);

	/**
	* \brief Reinterpret current object as \ref BaseObjectAttributeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_ToBaseAttribute(ImageMetadataObjectAttributeHandle* handle, BaseObjectAttributeHandle** result);

	/**
	* \brief Convert \ref BaseObjectAttributeHandle to \ref ImageMetadataObjectAttributeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_FromBaseAttribute(BaseObjectAttributeHandle* handle, ImageMetadataObjectAttributeHandle** result);

	/**
	* \copydoc IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_Release(ImageMetadataObjectAttributeHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_ATTRIBUTES_H */
