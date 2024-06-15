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
	* \brief Attribute object that contains information about image colorspace and components
	*/

	/**
	* \class SerializationOverrideObjectAttributeHandle
	* \extends BaseObjectAttributeHandle
	* \ingroup group_objects
	* \brief Attribute that modifies the attached object serialization ToPdf
	* \see \ref ObjectHandle::Object_ToPdf
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
	* \brief Image color spaces used within \ref ImageMetadataObjectAttributeHandle
	* \ingroup group_objects
	*/
	typedef enum {
		/**
		* \brief Undefined unitialized default value, triggers error when used
		*/
		ImageColorSpaceType_Undefined = 0,

		/**
		* \brief Grayscale (monochrome)
		*/
		ImageColorSpaceType_GRAY,

		/**
		* \brief RGB (red-green-blue)
		*/
		ImageColorSpaceType_RGB,

		/**
		* \brief CMYK (cyan-magenta-yellow-black)
		*/
		ImageColorSpaceType_CMYK
	} ImageColorSpaceType;

	/**
	* \memberof BaseObjectAttributeHandle
	* @{
	*/

	/**
	* \brief Get derived type of current object attribute
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_GetAttributeType(BaseObjectAttributeHandle* handle, ObjectAttributeType* result);

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
	* \brief Creates a new ImageMetadataObjectAttribute instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_Create(ImageMetadataObjectAttributeHandle** result);

	/**
	* \brief Get number of color components inside the associated image
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetColorComponents(ImageMetadataObjectAttributeHandle* handle, integer_type* result);

	/**
	* \brief Set number of color components inside the associated image
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_SetColorComponents(ImageMetadataObjectAttributeHandle* handle, integer_type data);

	/**
	* \brief Get color space of the associated image
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetColorSpace(ImageMetadataObjectAttributeHandle* handle, ImageColorSpaceType* result);

	/**
	* \brief Set color space of the associated image
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_SetColorSpace(ImageMetadataObjectAttributeHandle* handle, ImageColorSpaceType data);

	/**
	* \brief Get image width in pixels
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetWidth(ImageMetadataObjectAttributeHandle* handle, integer_type* result);

	/**
	* \brief Set image width in pixels
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_SetWidth(ImageMetadataObjectAttributeHandle* handle, integer_type data);

	/**
	* \brief Get image height in pixels
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetHeight(ImageMetadataObjectAttributeHandle* handle, integer_type* result);

	/**
	* \brief Set image height in pixels
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_SetHeight(ImageMetadataObjectAttributeHandle* handle, integer_type data);

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

	/**
	* \memberof SerializationOverrideObjectAttributeHandle
	* @{
	*/

	/**
	* \brief Creates a new SerializationOverrideObjectAttribute instance
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_Create(SerializationOverrideObjectAttributeHandle** result);

	/**
	* \brief Creates a new SerializationOverrideObjectAttribute instance with value specified in data and size
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_CreateFromData(string_type data, size_type size, SerializationOverrideObjectAttributeHandle** result);

	/**
	* \brief Reinterpret current object as \ref BaseObjectAttributeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_ToBaseAttribute(SerializationOverrideObjectAttributeHandle* handle, BaseObjectAttributeHandle** result);

	/**
	* \brief Convert \ref BaseObjectAttributeHandle to \ref SerializationOverrideObjectAttributeHandle
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_FromBaseAttribute(BaseObjectAttributeHandle* handle, SerializationOverrideObjectAttributeHandle** result);

	/**
	* \copydoc IUnknown_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_Release(SerializationOverrideObjectAttributeHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_OBJECT_ATTRIBUTES_H */
