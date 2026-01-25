#include "precompiled.h"

#include "semantics/objects/name_dictionary.h"
#include "semantics/objects/tree.h"
#include "semantics/objects/destinations.h"

#include "vanillapdf/semantics/c_name_dictionary.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

// Type alias for convenience
using DestinationNameTree = NameTree<DestinationPtr>;
using DestinationNameTreePtr = NameTreePtr<DestinationPtr>;

VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_Create(NameDictionaryHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto name_dict = NameDictionary::Create();
        auto ptr = name_dict.AddRefGet();
        *result = reinterpret_cast<NameDictionaryHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_ContainsDestinations(NameDictionaryHandle* handle, boolean_type* result)
{
    NameDictionary* obj = reinterpret_cast<NameDictionary*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputNameTreePtr<DestinationPtr> dests;
        bool contains = obj->Dests(dests);
        *result = contains ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_GetDestinations(NameDictionaryHandle* handle, DestinationNameTreeHandle** result)
{
    NameDictionary* obj = reinterpret_cast<NameDictionary*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputNameTreePtr<DestinationPtr> dests;
        bool contains = obj->Dests(dests);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = dests.AddRefGet();
        *result = reinterpret_cast<DestinationNameTreeHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_SetDestinations(NameDictionaryHandle* handle, DestinationNameTreeHandle* destinations)
{
    NameDictionary* obj = reinterpret_cast<NameDictionary*>(handle);
    DestinationNameTree* dests_obj = reinterpret_cast<DestinationNameTree*>(destinations);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(dests_obj);

    try
    {
        obj->SetDests(dests_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_ToUnknown(NameDictionaryHandle* handle, IUnknownHandle** result)
{
    return SafeObjectConvert<NameDictionary, IUnknown, NameDictionaryHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_FromUnknown(IUnknownHandle* handle, NameDictionaryHandle** result)
{
    return SafeObjectConvert<IUnknown, NameDictionary, IUnknownHandle, NameDictionaryHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION NameDictionary_Release(NameDictionaryHandle* handle)
{
    return ObjectRelease<NameDictionary, NameDictionaryHandle>(handle);
}
