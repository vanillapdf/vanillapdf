#include "precompiled.h"

#include "syntax/utils/object_attribute_list.h"

#include "vanillapdf/syntax/c_object_attribute_list.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_ToUnknown(ObjectAttributeListHandle* handle, IUnknownHandle** result) {
	return SafeObjectConvert<AttributeList, IUnknown, ObjectAttributeListHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_FromUnknown(IUnknownHandle* handle, ObjectAttributeListHandle** result) {
	return SafeObjectConvert<IUnknown, AttributeList, IUnknownHandle, ObjectAttributeListHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Release(ObjectAttributeListHandle* handle) {
	return ObjectRelease<AttributeList, ObjectAttributeListHandle>(handle);
}
