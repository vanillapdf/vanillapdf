#include "precompiled.h"

#include "syntax/utils/object_attribute_list.h"

#include "vanillapdf/syntax/c_object_attribute_list.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Create(ObjectAttributeListHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto attribute_list = make_deferred_container<AttributeList>();
        auto ptr = attribute_list.AddRefGet();
        *result = reinterpret_cast<ObjectAttributeListHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Add(ObjectAttributeListHandle* handle, BaseObjectAttributeHandle* value, boolean_type overwrite) {

    AttributeList* attribute_list = reinterpret_cast<AttributeList*>(handle);
    BaseAttribute* base_attribute = reinterpret_cast<BaseAttribute*>(value);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(attribute_list);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(base_attribute);

    try
    {
        attribute_list->Add(base_attribute, overwrite);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Remove(ObjectAttributeListHandle* handle, ObjectAttributeType key, boolean_type* result) {

    AttributeList* attribute_list = reinterpret_cast<AttributeList*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(attribute_list);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // The values between internal and public enum match exactly allowing to make this conversion
        auto attribute_key_converted = static_cast<BaseAttribute::Type>(key);

        *result = attribute_list->Remove(attribute_key_converted);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Contains(ObjectAttributeListHandle* handle, ObjectAttributeType key, boolean_type* result) {

    AttributeList* attribute_list = reinterpret_cast<AttributeList*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(attribute_list);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // The values between internal and public enum match exactly allowing to make this conversion
        auto attribute_key_converted = static_cast<BaseAttribute::Type>(key);

        *result = attribute_list->Contains(attribute_key_converted);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Get(ObjectAttributeListHandle* handle, ObjectAttributeType key, BaseObjectAttributeHandle** result) {

    AttributeList* attribute_list = reinterpret_cast<AttributeList*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(attribute_list);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // The values between internal and public enum match exactly allowing to make this conversion
        auto attribute_key_converted = static_cast<BaseAttribute::Type>(key);

        auto attribute_value = attribute_list->Get(attribute_key_converted);
        auto ptr = attribute_value.AddRefGet();
        *result = reinterpret_cast<BaseObjectAttributeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Clear(ObjectAttributeListHandle* handle) {

    AttributeList* attribute_list = reinterpret_cast<AttributeList*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(attribute_list);

    try {
        attribute_list->Clear();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_ToUnknown(ObjectAttributeListHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<AttributeList, IUnknown, ObjectAttributeListHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_FromUnknown(IUnknownHandle* handle, ObjectAttributeListHandle** result) {
    return SafeObjectConvert<IUnknown, AttributeList, IUnknownHandle, ObjectAttributeListHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ObjectAttributeList_Release(ObjectAttributeListHandle* handle) {
    return ObjectRelease<AttributeList, ObjectAttributeListHandle>(handle);
}
