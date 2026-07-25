#include "precompiled.h"
#include "semantics/objects/resource_dictionary.h"

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

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_ToUnknown(ResourceDictionaryHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<ResourceDictionary, IUnknown, ResourceDictionaryHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_FromUnknown(IUnknownHandle* handle, ResourceDictionaryHandle** result) {
    return SafeObjectConvert<IUnknown, ResourceDictionary, IUnknownHandle, ResourceDictionaryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ResourceDictionary_Release(ResourceDictionaryHandle* handle) {
    return ObjectRelease<ResourceDictionary, ResourceDictionaryHandle>(handle);
}
