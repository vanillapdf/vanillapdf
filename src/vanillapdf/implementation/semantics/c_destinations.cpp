#include "precompiled.h"
#include "semantics/objects/destinations.h"

#include "vanillapdf/semantics/c_destinations.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetPageNumber(DestinationHandle* handle, ObjectHandle** result)
{
    DestinationBase* obj = reinterpret_cast<DestinationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto direct = obj->GetPage();
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_Release(DestinationHandle* handle)
{
    return ObjectRelease<DestinationBase, DestinationHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION NamedDestinations_Contains(NamedDestinationsHandle* handle, const NameObjectHandle* name_handle, boolean_type* result)
{
    NamedDestinations* obj = reinterpret_cast<NamedDestinations*>(handle);
    const NameObject* name = reinterpret_cast<const NameObject*>(name_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->Contains(*name);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NamedDestinations_Find(NamedDestinationsHandle* handle, const NameObjectHandle* name_handle, DestinationHandle** result)
{
    NamedDestinations* obj = reinterpret_cast<NamedDestinations*>(handle);
    const NameObject* name = reinterpret_cast<const NameObject*>(name_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto direct = obj->Find(*name);
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}
VANILLAPDF_API error_type CALLING_CONVENTION NamedDestinations_Release(NamedDestinationsHandle* handle)
{
    return ObjectRelease<NamedDestinations, NamedDestinationsHandle>(handle);
}