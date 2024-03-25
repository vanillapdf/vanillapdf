#include "precompiled.h"

#include "vanillapdf/syntax/c_object_attributes.h"
#include "implementation/c_helper.h"

#include "syntax/utils/base_object_attribute.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

// Verify the enum values to match at compile time
static_assert(sizeof(ObjectAttributeType) == sizeof(BaseAttribute::Type));

static_assert(static_cast<int32_t>(ObjectAttributeType_Undefined) == static_cast<int32_t>(BaseAttribute::Type::Undefined));
static_assert(static_cast<int32_t>(ObjectAttributeType_Empty) == static_cast<int32_t>(BaseAttribute::Type::Empty));
static_assert(static_cast<int32_t>(ObjectAttributeType_SerializationOverride) == static_cast<int32_t>(BaseAttribute::Type::SerializationOverride));
static_assert(static_cast<int32_t>(ObjectAttributeType_TrackingIdentifier) == static_cast<int32_t>(BaseAttribute::Type::TrackingIdentifier));
static_assert(static_cast<int32_t>(ObjectAttributeType_ImageColorSpace) == static_cast<int32_t>(BaseAttribute::Type::ImageColorSpace));
static_assert(static_cast<int32_t>(ObjectAttributeType_ImageColorComponents) == static_cast<int32_t>(BaseAttribute::Type::ImageColorComponents));

VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_GetAtrributeType(BaseObjectAttributeHandle* handle, ObjectAttributeType* result) {

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
	case BaseAttribute::Type::ImageColorSpace:
		*result = ObjectAttributeType_ImageColorSpace; break;
	case BaseAttribute::Type::ImageColorComponents:
		*result = ObjectAttributeType_ImageColorComponents; break;
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
