#include "precompiled.h"

#include "vanillapdf/syntax/c_object_attributes.h"
#include "implementation/c_helper.h"

#include "syntax/utils/base_object_attribute.h"
#include "syntax/utils/image_metadata_object_attribute.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

// Verify the enum values to match at compile time
static_assert(sizeof(ObjectAttributeType) == sizeof(BaseAttribute::Type));

static_assert(static_cast<int32_t>(ObjectAttributeType_Undefined) == static_cast<int32_t>(BaseAttribute::Type::Undefined));
static_assert(static_cast<int32_t>(ObjectAttributeType_Empty) == static_cast<int32_t>(BaseAttribute::Type::Empty));
static_assert(static_cast<int32_t>(ObjectAttributeType_SerializationOverride) == static_cast<int32_t>(BaseAttribute::Type::SerializationOverride));
static_assert(static_cast<int32_t>(ObjectAttributeType_TrackingIdentifier) == static_cast<int32_t>(BaseAttribute::Type::TrackingIdentifier));
static_assert(static_cast<int32_t>(ObjectAttributeType_ImageMetadata) == static_cast<int32_t>(BaseAttribute::Type::ImageMetadata));

VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_GetAttributeType(BaseObjectAttributeHandle* handle, ObjectAttributeType* result) {

	BaseAttribute* base_attribute = reinterpret_cast<BaseAttribute*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(base_attribute);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (base_attribute->GetType()) {
	case BaseAttribute::Type::Undefined:
		*result = ObjectAttributeType_Undefined; break;
	case BaseAttribute::Type::Empty:
		*result = ObjectAttributeType_Empty; break;
	case BaseAttribute::Type::SerializationOverride:
		*result = ObjectAttributeType_SerializationOverride; break;
	case BaseAttribute::Type::TrackingIdentifier:
		*result = ObjectAttributeType_TrackingIdentifier; break;
	case BaseAttribute::Type::ImageMetadata:
		*result = ObjectAttributeType_ImageMetadata; break;
	default:
		return VANILLAPDF_ERROR_GENERAL;
	}

	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_ToUnknown(BaseObjectAttributeHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<BaseAttribute, IUnknown, BaseObjectAttributeHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_FromUnknown(IUnknownHandle* handle, BaseObjectAttributeHandle** result) {
	return SafeObjectConvert<IUnknown, BaseAttribute, IUnknownHandle, BaseObjectAttributeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_Release(BaseObjectAttributeHandle* handle) {
	return ObjectRelease<BaseAttribute, BaseObjectAttributeHandle>(handle);
}

// ImageMetadataObjectAttribute

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetColorComponents(ImageMetadataObjectAttributeHandle* handle, integer_type* result) {

	ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	*result = atrribute_object->GetColorComponents();
	return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_ToBaseAttribute(ImageMetadataObjectAttributeHandle* handle, BaseObjectAttributeHandle** result) {
	return SafeObjectConvert<ImageMetadataObjectAttribute, BaseAttribute, ImageMetadataObjectAttributeHandle, BaseObjectAttributeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_FromBaseAttribute(BaseObjectAttributeHandle* handle, ImageMetadataObjectAttributeHandle** result) {
	return SafeObjectConvert<BaseAttribute, ImageMetadataObjectAttribute, BaseObjectAttributeHandle, ImageMetadataObjectAttributeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_Release(ImageMetadataObjectAttributeHandle* handle) {
	return ObjectRelease<ImageMetadataObjectAttribute, ImageMetadataObjectAttributeHandle>(handle);
}
