#include "precompiled.h"
#include "semantics/objects/color.h"

#include "vanillapdf/semantics/c_color.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Color_CreateTransparent(ColorHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto color = Color::CreateTransparent();
        auto ptr = color.AddRefGet();
        *result = reinterpret_cast<ColorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_CreateGray(real_type gray, ColorHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto color = Color::CreateGray(gray);
        auto ptr = color.AddRefGet();
        *result = reinterpret_cast<ColorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_CreateRGB(real_type red, real_type green, real_type blue, ColorHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto color = Color::CreateRGB(red, green, blue);
        auto ptr = color.AddRefGet();
        *result = reinterpret_cast<ColorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_CreateCMYK(real_type cyan, real_type magenta, real_type yellow, real_type black, ColorHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto color = Color::CreateCMYK(cyan, magenta, yellow, black);
        auto ptr = color.AddRefGet();
        *result = reinterpret_cast<ColorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetColorSpace(ColorHandle* handle, ColorSpaceType* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        switch (obj->GetColorSpace()) {
        case Color::ColorSpace::Transparent:
            *result = ColorSpace_Transparent;
            break;
        case Color::ColorSpace::DeviceGray:
            *result = ColorSpace_DeviceGray;
            break;
        case Color::ColorSpace::DeviceRGB:
            *result = ColorSpace_DeviceRGB;
            break;
        case Color::ColorSpace::DeviceCMYK:
            *result = ColorSpace_DeviceCMYK;
            break;
        default:
            return VANILLAPDF_ERROR_GENERAL;
        }
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetRed(ColorHandle* handle, real_type* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetRed();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetGreen(ColorHandle* handle, real_type* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetGreen();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetBlue(ColorHandle* handle, real_type* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetBlue();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetGray(ColorHandle* handle, real_type* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetGray();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetCyan(ColorHandle* handle, real_type* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetCyan();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetMagenta(ColorHandle* handle, real_type* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetMagenta();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetYellow(ColorHandle* handle, real_type* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetYellow();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_GetBlack(ColorHandle* handle, real_type* result)
{
    Color* obj = reinterpret_cast<Color*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        *result = obj->GetBlack();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_ToUnknown(ColorHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<Color, IUnknown, ColorHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_FromUnknown(IUnknownHandle* handle, ColorHandle** result) {
    return SafeObjectConvert<IUnknown, Color, IUnknownHandle, ColorHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Color_Release(ColorHandle* handle) {
    return ObjectRelease<Color, ColorHandle>(handle);
}
