#include "precompiled.h"
#include "semantics/objects/destinations.h"

#include "vanillapdf/semantics/c_destinations.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetType(DestinationHandle* handle, DestinationType* result)
{
    DestinationBase* obj = reinterpret_cast<DestinationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        switch (obj->GetType()) {
        case DestinationBase::Type::XYZ:
            *result = DestinationType_XYZ; break;
        case DestinationBase::Type::Fit:
            *result = DestinationType_Fit; break;
        case DestinationBase::Type::FitHorizontal:
            *result = DestinationType_FitHorizontal; break;
        case DestinationBase::Type::FitVertical:
            *result = DestinationType_FitVertical; break;
        case DestinationBase::Type::FitRectangle:
            *result = DestinationType_FitRectangle; break;
        case DestinationBase::Type::FitBoundingBox:
            *result = DestinationType_FitBoundingBox; break;
        case DestinationBase::Type::FitBoundingBoxHorizontal:
            *result = DestinationType_FitBoundingBoxHorizontal; break;
        case DestinationBase::Type::FitBoundingBoxVertical:
            *result = DestinationType_FitBoundingBoxVertical; break;
        default:
            *result = DestinationType_Undefined; break;
        }
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

VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetLeft(DestinationHandle* handle, RealObjectHandle** result)
{
    DestinationBase* obj = reinterpret_cast<DestinationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // For XYZ: index 2 is left, for FitR: index 2 is left
        auto type = obj->GetType();
        if (type == DestinationBase::Type::XYZ || type == DestinationBase::Type::FitRectangle) {
            auto param = obj->GetParameter(2);
            if (syntax::ObjectUtils::IsType<syntax::RealObjectPtr>(param)) {
                auto ptr = param.AddRefGet();
                *result = reinterpret_cast<RealObjectHandle*>(ptr);
                return VANILLAPDF_ERROR_SUCCESS;
            }
        }
        *result = nullptr;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetTop(DestinationHandle* handle, RealObjectHandle** result)
{
    DestinationBase* obj = reinterpret_cast<DestinationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto type = obj->GetType();
        // For XYZ: index 3 is top, for FitH: index 2 is top, for FitR: index 3 is bottom (inverted), for FitBH: index 2 is top
        size_t index = 0;
        if (type == DestinationBase::Type::XYZ) {
            index = 3;
        } else if (type == DestinationBase::Type::FitHorizontal || type == DestinationBase::Type::FitBoundingBoxHorizontal) {
            index = 2;
        } else if (type == DestinationBase::Type::FitRectangle) {
            index = 5; // top is at index 5 for FitR
        } else {
            *result = nullptr;
            return VANILLAPDF_ERROR_SUCCESS;
        }
        
        auto param = obj->GetParameter(index);
        if (syntax::ObjectUtils::IsType<syntax::RealObjectPtr>(param)) {
            auto ptr = param.AddRefGet();
            *result = reinterpret_cast<RealObjectHandle*>(ptr);
            return VANILLAPDF_ERROR_SUCCESS;
        }
        *result = nullptr;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetRight(DestinationHandle* handle, RealObjectHandle** result)
{
    DestinationBase* obj = reinterpret_cast<DestinationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // For FitR: index 4 is right
        auto type = obj->GetType();
        if (type == DestinationBase::Type::FitRectangle) {
            auto param = obj->GetParameter(4);
            if (syntax::ObjectUtils::IsType<syntax::RealObjectPtr>(param)) {
                auto ptr = param.AddRefGet();
                *result = reinterpret_cast<RealObjectHandle*>(ptr);
                return VANILLAPDF_ERROR_SUCCESS;
            }
        }
        *result = nullptr;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetBottom(DestinationHandle* handle, RealObjectHandle** result)
{
    DestinationBase* obj = reinterpret_cast<DestinationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // For FitR: index 3 is bottom
        auto type = obj->GetType();
        if (type == DestinationBase::Type::FitRectangle) {
            auto param = obj->GetParameter(3);
            if (syntax::ObjectUtils::IsType<syntax::RealObjectPtr>(param)) {
                auto ptr = param.AddRefGet();
                *result = reinterpret_cast<RealObjectHandle*>(ptr);
                return VANILLAPDF_ERROR_SUCCESS;
            }
        }
        *result = nullptr;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Destination_GetZoom(DestinationHandle* handle, RealObjectHandle** result)
{
    DestinationBase* obj = reinterpret_cast<DestinationBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // For XYZ: index 4 is zoom
        auto type = obj->GetType();
        if (type == DestinationBase::Type::XYZ) {
            auto param = obj->GetParameter(4);
            if (syntax::ObjectUtils::IsType<syntax::RealObjectPtr>(param)) {
                auto ptr = param.AddRefGet();
                *result = reinterpret_cast<RealObjectHandle*>(ptr);
                return VANILLAPDF_ERROR_SUCCESS;
            }
        }
        *result = nullptr;
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