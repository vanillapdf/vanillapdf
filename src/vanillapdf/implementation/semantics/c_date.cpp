#include "precompiled.h"
#include "semantics/objects/date.h"

#include "vanillapdf/semantics/c_date.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Date_CreateEmpty(DateHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        DatePtr date;
        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_CreateCurrent(DateHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        DatePtr date = Date::GetCurrentDate();
        auto ptr = date.AddRefGet();
        *result = reinterpret_cast<DateHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetYear(DateHandle* handle, integer_type* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetYear();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetYear(DateHandle* handle, integer_type value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->SetYear(value);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMonth(DateHandle* handle, integer_type* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetMonth();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetMonth(DateHandle* handle, integer_type value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->SetMonth(value);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetDay(DateHandle* handle, integer_type* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetDay();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetDay(DateHandle* handle, integer_type value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->SetDay(value);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetHour(DateHandle* handle, integer_type* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetHour();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetHour(DateHandle* handle, integer_type value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->SetHour(value);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMinute(DateHandle* handle, integer_type* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetMinute();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetMinute(DateHandle* handle, integer_type value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->SetMinute(value);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetSecond(DateHandle* handle, integer_type* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetSecond();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetSecond(DateHandle* handle, integer_type value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->SetSecond(value);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetTimezone(DateHandle* handle, TimezoneType* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    switch (obj->GetTimezone())
    {
    case Timezone::UTC:
        *result = TimezoneType_UTC; break;
    case Timezone::Later:
        *result = TimezoneType_Later; break;
    case Timezone::Earlier:
        *result = TimezoneType_Earlier; break;
    default:
        return VANILLAPDF_ERROR_GENERAL;
    }
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetTimezone(DateHandle* handle, TimezoneType value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    switch (value) {
        case TimezoneType_UTC:
            obj->SetTimezone(Timezone::UTC); break;
        case TimezoneType_Later:
            obj->SetTimezone(Timezone::Later); break;
        case TimezoneType_Earlier:
            obj->SetTimezone(Timezone::Earlier); break;
        default:
            return VANILLAPDF_ERROR_GENERAL;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetHourOffset(DateHandle* handle, integer_type* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetHourOffset();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetHourOffset(DateHandle* handle, integer_type value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->SetHourOffset(value);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_GetMinuteOffset(DateHandle* handle, integer_type* result)
{
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    *result = obj->GetMinuteOffset();
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_SetMinuteOffset(DateHandle* handle, integer_type value) {
    Date* obj = reinterpret_cast<Date*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    obj->SetMinuteOffset(value);
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_ToUnknown(DateHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<Date, IUnknown, DateHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_FromUnknown(IUnknownHandle* handle, DateHandle** result) {
    return SafeObjectConvert<IUnknown, Date, IUnknownHandle, DateHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Date_Release(DateHandle* handle) {
    return ObjectRelease<Date, DateHandle>(handle);
}
