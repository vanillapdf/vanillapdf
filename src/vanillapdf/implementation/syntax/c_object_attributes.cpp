#include "precompiled.h"

#include "vanillapdf/syntax/c_object_attributes.h"
#include "implementation/c_helper.h"

#include "syntax/utils/base_object_attribute.h"
#include "syntax/utils/image_metadata_object_attribute.h"
#include "syntax/utils/serialization_override_object_attribute.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

// Verify the enum values to match at compile time
static_assert(sizeof(ObjectAttributeType) == sizeof(BaseAttribute::Type));

static_assert(static_cast<int32_t>(ObjectAttributeType_Undefined) == static_cast<int32_t>(BaseAttribute::Type::Undefined));
static_assert(static_cast<int32_t>(ObjectAttributeType_Empty) == static_cast<int32_t>(BaseAttribute::Type::Empty));
static_assert(static_cast<int32_t>(ObjectAttributeType_SerializationOverride) == static_cast<int32_t>(BaseAttribute::Type::SerializationOverride));
static_assert(static_cast<int32_t>(ObjectAttributeType_TrackingIdentifier) == static_cast<int32_t>(BaseAttribute::Type::TrackingIdentifier));
static_assert(static_cast<int32_t>(ObjectAttributeType_ImageMetadata) == static_cast<int32_t>(BaseAttribute::Type::ImageMetadata));

// ImageColorSpaceType

// Verify the enum values to match at compile time
static_assert(sizeof(ImageColorSpaceType) == sizeof(ImageMetadataObjectAttribute::ColorSpaceType));

static_assert(static_cast<int32_t>(ImageColorSpaceType_Undefined) == static_cast<int32_t>(ImageMetadataObjectAttribute::ColorSpaceType::Undefined));
static_assert(static_cast<int32_t>(ImageColorSpaceType_GRAY) == static_cast<int32_t>(ImageMetadataObjectAttribute::ColorSpaceType::GRAY));
static_assert(static_cast<int32_t>(ImageColorSpaceType_RGB) == static_cast<int32_t>(ImageMetadataObjectAttribute::ColorSpaceType::RGB));
static_assert(static_cast<int32_t>(ImageColorSpaceType_CMYK) == static_cast<int32_t>(ImageMetadataObjectAttribute::ColorSpaceType::CMYK));

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

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_Create(ImageMetadataObjectAttributeHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto object = make_deferred<ImageMetadataObjectAttribute>();
        auto ptr = object.AddRefGet();
        *result = reinterpret_cast<ImageMetadataObjectAttributeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetColorComponents(ImageMetadataObjectAttributeHandle* handle, integer_type* result) {

    ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = atrribute_object->GetColorComponents();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_SetColorComponents(ImageMetadataObjectAttributeHandle* handle, integer_type data) {
    ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);

    atrribute_object->SetColorComponents(data);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetColorSpace(ImageMetadataObjectAttributeHandle* handle, ImageColorSpaceType* result) {

    ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    // The values between internal and public enum match exactly allowing to make this conversion
    *result = static_cast<ImageColorSpaceType>(atrribute_object->GetColorSpace());
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_SetColorSpace(ImageMetadataObjectAttributeHandle* handle, ImageColorSpaceType data) {
    ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);

    // The values between internal and public enum match exactly allowing to make this conversion
    auto data_converted = static_cast<ImageMetadataObjectAttribute::ColorSpaceType>(data);
    atrribute_object->SetColorSpace(data_converted);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetWidth(ImageMetadataObjectAttributeHandle* handle, integer_type* result) {
    ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = atrribute_object->GetWidth();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_SetWidth(ImageMetadataObjectAttributeHandle* handle, integer_type data) {
    ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);

    atrribute_object->SetWidth(data);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_GetHeight(ImageMetadataObjectAttributeHandle* handle, integer_type* result) {
    ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = atrribute_object->GetHeight();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION ImageMetadataObjectAttribute_SetHeight(ImageMetadataObjectAttributeHandle* handle, integer_type data) {
    ImageMetadataObjectAttribute* atrribute_object = reinterpret_cast<ImageMetadataObjectAttribute*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(atrribute_object);

    atrribute_object->SetHeight(data);
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

// SerializationOverrideObjectAttribute

VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_Create(SerializationOverrideObjectAttributeHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto object = make_deferred<SerializationOverrideAttribute>();
        auto ptr = object.AddRefGet();
        *result = reinterpret_cast<SerializationOverrideObjectAttributeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_CreateFromData(string_type data, size_type size, SerializationOverrideObjectAttributeHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        std::string_view user_data(data, size);
        auto object = make_deferred<SerializationOverrideAttribute>(user_data);
        auto ptr = object.AddRefGet();
        *result = reinterpret_cast<SerializationOverrideObjectAttributeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_ToBaseAttribute(SerializationOverrideObjectAttributeHandle* handle, BaseObjectAttributeHandle** result) {
    return SafeObjectConvert<SerializationOverrideAttribute, BaseAttribute, SerializationOverrideObjectAttributeHandle, BaseObjectAttributeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_FromBaseAttribute(BaseObjectAttributeHandle* handle, SerializationOverrideObjectAttributeHandle** result) {
    return SafeObjectConvert<BaseAttribute, SerializationOverrideAttribute, BaseObjectAttributeHandle, SerializationOverrideObjectAttributeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SerializationOverrideObjectAttribute_Release(SerializationOverrideObjectAttributeHandle* handle) {
    return ObjectRelease<SerializationOverrideAttribute, SerializationOverrideObjectAttributeHandle>(handle);
}
