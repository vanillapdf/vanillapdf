#include "precompiled.h"
#include "semantics/objects/destinations.h"

#include "vanillapdf/semantics/c_destinations.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Destination_CreateFromArray(ArrayObjectHandle* array_handle, DestinationHandle** result)
{
    MixedArrayObject* array_obj = reinterpret_cast<MixedArrayObject*>(array_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(array_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto destination = DestinationBase::CreateFromArray(array_obj);
        auto ptr = destination.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_CreateFromDictionary(DictionaryObjectHandle* dict_handle, DestinationHandle** result)
{
    DictionaryObject* dict_obj = reinterpret_cast<DictionaryObject*>(dict_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(dict_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto destination = DestinationBase::CreateFromDictionary(dict_obj);
        auto ptr = destination.AddRefGet();
        *result = reinterpret_cast<DestinationHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetDestinationType(DestinationHandle* handle, DestinationType* result)
{
    DestinationBase* obj = reinterpret_cast<DestinationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto type = obj->GetType();
        *result = static_cast<DestinationType>(type);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

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

VANILLAPDF_API error_type CALLING_CONVENTION Destination_ToUnknown(DestinationHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<DestinationBase, IUnknown, DestinationHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_FromUnknown(IUnknownHandle* handle, DestinationHandle** result) {
    return SafeObjectConvert<IUnknown, DestinationBase, IUnknownHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_Release(DestinationHandle* handle)
{
    return ObjectRelease<DestinationBase, DestinationHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_FromDestination(DestinationHandle* handle, XYZDestinationHandle** result)
{
    return SafeObjectConvert<DestinationBase, XYZDestination, DestinationHandle, XYZDestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_ToDestination(XYZDestinationHandle* handle, DestinationHandle** result)
{
    return SafeObjectConvert<XYZDestination, DestinationBase, XYZDestinationHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_GetLeft(XYZDestinationHandle* handle, ObjectHandle** result)
{
    XYZDestination* obj = reinterpret_cast<XYZDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetLeft(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_GetTop(XYZDestinationHandle* handle, ObjectHandle** result)
{
    XYZDestination* obj = reinterpret_cast<XYZDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetTop(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_GetZoom(XYZDestinationHandle* handle, ObjectHandle** result)
{
    XYZDestination* obj = reinterpret_cast<XYZDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetZoom(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION XYZDestination_Release(XYZDestinationHandle* handle)
{
    return ObjectRelease<XYZDestination, XYZDestinationHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitDestination_FromDestination(DestinationHandle* handle, FitDestinationHandle** result)
{
    return SafeObjectConvert<DestinationBase, FitDestination, DestinationHandle, FitDestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitDestination_ToDestination(FitDestinationHandle* handle, DestinationHandle** result)
{
    return SafeObjectConvert<FitDestination, DestinationBase, FitDestinationHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitDestination_Release(FitDestinationHandle* handle)
{
    return ObjectRelease<FitDestination, FitDestinationHandle>(handle);
}

// FitHorizontalDestination functions
VANILLAPDF_API error_type CALLING_CONVENTION FitHorizontalDestination_FromDestination(DestinationHandle* handle, FitHorizontalDestinationHandle** result)
{
    return SafeObjectConvert<DestinationBase, FitHorizontalDestination, DestinationHandle, FitHorizontalDestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitHorizontalDestination_ToDestination(FitHorizontalDestinationHandle* handle, DestinationHandle** result)
{
    return SafeObjectConvert<FitHorizontalDestination, DestinationBase, FitHorizontalDestinationHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitHorizontalDestination_Release(FitHorizontalDestinationHandle* handle)
{
    return ObjectRelease<FitHorizontalDestination, FitHorizontalDestinationHandle>(handle);
}

// FitVerticalDestination functions
VANILLAPDF_API error_type CALLING_CONVENTION FitVerticalDestination_FromDestination(DestinationHandle* handle, FitVerticalDestinationHandle** result)
{
    return SafeObjectConvert<DestinationBase, FitVerticalDestination, DestinationHandle, FitVerticalDestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitVerticalDestination_ToDestination(FitVerticalDestinationHandle* handle, DestinationHandle** result)
{
    return SafeObjectConvert<FitVerticalDestination, DestinationBase, FitVerticalDestinationHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitVerticalDestination_Release(FitVerticalDestinationHandle* handle)
{
    return ObjectRelease<FitVerticalDestination, FitVerticalDestinationHandle>(handle);
}

// FitRectangleDestination functions
VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_FromDestination(DestinationHandle* handle, FitRectangleDestinationHandle** result)
{
    return SafeObjectConvert<DestinationBase, FitRectangleDestination, DestinationHandle, FitRectangleDestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_ToDestination(FitRectangleDestinationHandle* handle, DestinationHandle** result)
{
    return SafeObjectConvert<FitRectangleDestination, DestinationBase, FitRectangleDestinationHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_Release(FitRectangleDestinationHandle* handle)
{
    return ObjectRelease<FitRectangleDestination, FitRectangleDestinationHandle>(handle);
}

// FitBoundingBoxDestination functions
VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxDestination_FromDestination(DestinationHandle* handle, FitBoundingBoxDestinationHandle** result)
{
    return SafeObjectConvert<DestinationBase, FitBoundingBoxDestination, DestinationHandle, FitBoundingBoxDestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxDestination_ToDestination(FitBoundingBoxDestinationHandle* handle, DestinationHandle** result)
{
    return SafeObjectConvert<FitBoundingBoxDestination, DestinationBase, FitBoundingBoxDestinationHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxDestination_Release(FitBoundingBoxDestinationHandle* handle)
{
    return ObjectRelease<FitBoundingBoxDestination, FitBoundingBoxDestinationHandle>(handle);
}

// FitBoundingBoxHorizontalDestination functions
VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxHorizontalDestination_FromDestination(DestinationHandle* handle, FitBoundingBoxHorizontalDestinationHandle** result)
{
    return SafeObjectConvert<DestinationBase, FitBoundingBoxHorizontalDestination, DestinationHandle, FitBoundingBoxHorizontalDestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxHorizontalDestination_ToDestination(FitBoundingBoxHorizontalDestinationHandle* handle, DestinationHandle** result)
{
    return SafeObjectConvert<FitBoundingBoxHorizontalDestination, DestinationBase, FitBoundingBoxHorizontalDestinationHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxHorizontalDestination_Release(FitBoundingBoxHorizontalDestinationHandle* handle)
{
    return ObjectRelease<FitBoundingBoxHorizontalDestination, FitBoundingBoxHorizontalDestinationHandle>(handle);
}

// FitBoundingBoxVerticalDestination functions
VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxVerticalDestination_FromDestination(DestinationHandle* handle, FitBoundingBoxVerticalDestinationHandle** result)
{
    return SafeObjectConvert<DestinationBase, FitBoundingBoxVerticalDestination, DestinationHandle, FitBoundingBoxVerticalDestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxVerticalDestination_ToDestination(FitBoundingBoxVerticalDestinationHandle* handle, DestinationHandle** result)
{
    return SafeObjectConvert<FitBoundingBoxVerticalDestination, DestinationBase, FitBoundingBoxVerticalDestinationHandle, DestinationHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxVerticalDestination_Release(FitBoundingBoxVerticalDestinationHandle* handle)
{
    return ObjectRelease<FitBoundingBoxVerticalDestination, FitBoundingBoxVerticalDestinationHandle>(handle);
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
VANILLAPDF_API error_type CALLING_CONVENTION FitHorizontalDestination_GetTop(FitHorizontalDestinationHandle* handle, ObjectHandle** result)
{
    FitHorizontalDestination* obj = reinterpret_cast<FitHorizontalDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetTop(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FitVerticalDestination_GetLeft(FitVerticalDestinationHandle* handle, ObjectHandle** result)
{
    FitVerticalDestination* obj = reinterpret_cast<FitVerticalDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetLeft(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_GetLeft(FitRectangleDestinationHandle* handle, ObjectHandle** result)
{
    FitRectangleDestination* obj = reinterpret_cast<FitRectangleDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetLeft(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_GetBottom(FitRectangleDestinationHandle* handle, ObjectHandle** result)
{
    FitRectangleDestination* obj = reinterpret_cast<FitRectangleDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetBottom(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_GetRight(FitRectangleDestinationHandle* handle, ObjectHandle** result)
{
    FitRectangleDestination* obj = reinterpret_cast<FitRectangleDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetRight(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FitRectangleDestination_GetTop(FitRectangleDestinationHandle* handle, ObjectHandle** result)
{
    FitRectangleDestination* obj = reinterpret_cast<FitRectangleDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetTop(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxHorizontalDestination_GetTop(FitBoundingBoxHorizontalDestinationHandle* handle, ObjectHandle** result)
{
    FitBoundingBoxHorizontalDestination* obj = reinterpret_cast<FitBoundingBoxHorizontalDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetTop(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FitBoundingBoxVerticalDestination_GetLeft(FitBoundingBoxVerticalDestinationHandle* handle, ObjectHandle** result)
{
    FitBoundingBoxVerticalDestination* obj = reinterpret_cast<FitBoundingBoxVerticalDestination*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        ObjectPtr direct;
        auto contains = obj->GetLeft(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION NamedDestinations_Release(NamedDestinationsHandle* handle)
{
    return ObjectRelease<NamedDestinations, NamedDestinationsHandle>(handle);
}