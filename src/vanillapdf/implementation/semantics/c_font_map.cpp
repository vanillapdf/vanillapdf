#include "precompiled.h"
#include "semantics/objects/font.h"

#include "vanillapdf/semantics/c_font_map.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Contains(FontMapHandle* handle, const NameObjectHandle* key_handle, boolean_type* result)
{
    FontMap* obj = reinterpret_cast<FontMap*>(handle);
    const NameObject* key = reinterpret_cast<const NameObject*>(key_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(key);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->Contains(*key);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Find(FontMapHandle* handle, const NameObjectHandle* key_handle, FontHandle** result)
{
    FontMap* obj = reinterpret_cast<FontMap*>(handle);
    const NameObject* key = reinterpret_cast<const NameObject*>(key_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(key);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto direct = obj->Find(*key);
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<FontHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_GetIterator(FontMapHandle* handle, FontMapIteratorHandle** result)
{
    FontMap* obj = reinterpret_cast<FontMap*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto it = obj->Begin();
        auto ptr = it.AddRefGet();
        *result = reinterpret_cast<FontMapIteratorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_GetKey(FontMapIteratorHandle* handle, NameObjectHandle** result)
{
    FontMap::Iterator* obj = reinterpret_cast<FontMap::Iterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto name = obj->First();
        auto ptr = name.AddRefGet();
        *result = reinterpret_cast<NameObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_GetValue(FontMapIteratorHandle* handle, FontHandle** result)
{
    FontMap::Iterator* obj = reinterpret_cast<FontMap::Iterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto font = obj->Second();
        auto ptr = font.AddRefGet();
        *result = reinterpret_cast<FontHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_IsValid(FontMapIteratorHandle* handle, boolean_type* result)
{
    FontMap::Iterator* iterator = reinterpret_cast<FontMap::Iterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(iterator);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        if (iterator->IsValid()) {
            *result = VANILLAPDF_RV_TRUE;
        }
        else {
            *result = VANILLAPDF_RV_FALSE;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_Next(FontMapIteratorHandle* handle)
{
    FontMap::Iterator* obj = reinterpret_cast<FontMap::Iterator*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        ++(*obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMapIterator_Release(FontMapIteratorHandle* handle)
{
    return ObjectRelease<FontMap::Iterator, FontMapIteratorHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_ToUnknown(FontMapHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<FontMap, IUnknown, FontMapHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_FromUnknown(IUnknownHandle* handle, FontMapHandle** result) {
    return SafeObjectConvert<IUnknown, FontMap, IUnknownHandle, FontMapHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FontMap_Release(FontMapHandle* handle) {
    return ObjectRelease<FontMap, FontMapHandle>(handle);
}
