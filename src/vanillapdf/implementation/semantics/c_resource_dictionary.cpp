#include "precompiled.h"
#include "semantics/objects/resource_dictionary.h"
#include "semantics/objects/xobject.h"

#include "syntax/objects/dictionary_object.h"

#include "vanillapdf/semantics/c_resource_dictionary.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_Create(ResourceDictionaryHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        DictionaryObjectPtr dictionary;
        auto resources = make_deferred<ResourceDictionary>(dictionary);
        auto ptr = resources.AddRefGet();
        *result = reinterpret_cast<ResourceDictionaryHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_CreateFromDictionary(DictionaryObjectHandle* handle, ResourceDictionaryHandle** result)
{
    DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto resources = make_deferred<ResourceDictionary>(dictionary);
        auto ptr = resources.AddRefGet();
        *result = reinterpret_cast<ResourceDictionaryHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_GetFontMap(ResourceDictionaryHandle* handle, FontMapHandle** result)
{
    ResourceDictionary* obj = reinterpret_cast<ResourceDictionary*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputFontMapPtr font;
        bool contains = obj->Font(font);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = font.AddRefGet();
        *result = reinterpret_cast<FontMapHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_GetXObjectCount(ResourceDictionaryHandle* handle, size_type* result)
{
    ResourceDictionary* obj = reinterpret_cast<ResourceDictionary*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetXObjectCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_GetXObjectName(ResourceDictionaryHandle* handle, size_type at, NameObjectHandle** result)
{
    ResourceDictionary* obj = reinterpret_cast<ResourceDictionary*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto name = obj->GetXObjectName(at);
        auto ptr = name.AddRefGet();
        *result = reinterpret_cast<NameObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_FindXObject(ResourceDictionaryHandle* handle, const NameObjectHandle* name, XObjectHandle** result)
{
    ResourceDictionary* obj = reinterpret_cast<ResourceDictionary*>(handle);
    const NameObject* name_object = reinterpret_cast<const NameObject*>(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name_object);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputXObjectPtr xobject;
        bool found = obj->TryFindXObject(*name_object, xobject);
        if (!found) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = xobject.AddRefGet();
        *result = reinterpret_cast<XObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_AddXObject(ResourceDictionaryHandle* handle, const NameObjectHandle* name, XObjectHandle* value)
{
    ResourceDictionary* obj = reinterpret_cast<ResourceDictionary*>(handle);
    const NameObject* name_object = reinterpret_cast<const NameObject*>(name);
    XObjectBase* xobject = reinterpret_cast<XObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name_object);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(xobject);

    try
    {
        obj->AddXObject(*name_object, xobject);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_RemoveXObject(ResourceDictionaryHandle* handle, const NameObjectHandle* name, boolean_type* result)
{
    ResourceDictionary* obj = reinterpret_cast<ResourceDictionary*>(handle);
    const NameObject* name_object = reinterpret_cast<const NameObject*>(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name_object);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        if (obj->RemoveXObject(*name_object)) {
            *result = VANILLAPDF_RV_TRUE;
        } else {
            *result = VANILLAPDF_RV_FALSE;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_GetBaseObject(ResourceDictionaryHandle* handle, ObjectHandle** result)
{
    ResourceDictionary* obj = reinterpret_cast<ResourceDictionary*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto base_object = obj->GetObject();
        auto ptr = base_object.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_ToUnknown(ResourceDictionaryHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<ResourceDictionary, IUnknown, ResourceDictionaryHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_FromUnknown(IUnknownHandle* handle, ResourceDictionaryHandle** result) {
    return SafeObjectConvert<IUnknown, ResourceDictionary, IUnknownHandle, ResourceDictionaryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_Release(ResourceDictionaryHandle* handle) {
    return ObjectRelease<ResourceDictionary, ResourceDictionaryHandle>(handle);
}
