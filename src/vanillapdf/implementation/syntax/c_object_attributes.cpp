#include "precompiled.h"

#include "vanillapdf/syntax/c_object_attributes.h"
#include "implementation/c_helper.h"

#include "syntax/utils/base_object_attribute.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_ToUnknown(BaseObjectAttributeHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<BaseAttribute, IUnknown, BaseObjectAttributeHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_FromUnknown(IUnknownHandle* handle, BaseObjectAttributeHandle** result) {
	return SafeObjectConvert<IUnknown, BaseAttribute, IUnknownHandle, BaseObjectAttributeHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION BaseObjectAttribute_Release(BaseObjectAttributeHandle* handle) {
	return ObjectRelease<BaseAttribute, BaseObjectAttributeHandle>(handle);
}
