#include "precompiled.h"
#include "semantics/objects/appearance_characteristics.h"
#include "semantics/objects/color.h"

#include "vanillapdf/semantics/c_appearance_characteristics.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_Create(AppearanceCharacteristicsHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        AppearanceCharacteristicsPtr characteristics;
        auto ptr = characteristics.AddRefGet();
        *result = reinterpret_cast<AppearanceCharacteristicsHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetRotation(AppearanceCharacteristicsHandle* handle, bigint_type* result)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        types::big_int rotation = 0;
        bool contains = obj->GetRotation(rotation);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        *result = rotation;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetRotation(AppearanceCharacteristicsHandle* handle, bigint_type data)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try
    {
        obj->SetRotation(data);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetBorderColor(AppearanceCharacteristicsHandle* handle, ColorHandle** result)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputColorPtr color;
        bool contains = obj->GetBorderColor(color);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = color.AddRefGet();
        *result = reinterpret_cast<ColorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetBorderColor(AppearanceCharacteristicsHandle* handle, ColorHandle* value)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    Color* color = reinterpret_cast<Color*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(color);

    try
    {
        obj->SetBorderColor(color);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetBackgroundColor(AppearanceCharacteristicsHandle* handle, ColorHandle** result)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputColorPtr color;
        bool contains = obj->GetBackgroundColor(color);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = color.AddRefGet();
        *result = reinterpret_cast<ColorHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetBackgroundColor(AppearanceCharacteristicsHandle* handle, ColorHandle* value)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    Color* color = reinterpret_cast<Color*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(color);

    try
    {
        obj->SetBackgroundColor(color);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetNormalCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle** result)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr caption;
        bool contains = obj->GetNormalCaption(caption);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = caption.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetNormalCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle* value)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    StringObjectBase* caption = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(caption);

    try
    {
        obj->SetNormalCaption(caption);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetRolloverCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle** result)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr caption;
        bool contains = obj->GetRolloverCaption(caption);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = caption.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetRolloverCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle* value)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    StringObjectBase* caption = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(caption);

    try
    {
        obj->SetRolloverCaption(caption);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetDownCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle** result)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        OutputStringObjectPtr caption;
        bool contains = obj->GetDownCaption(caption);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = caption.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_SetDownCaption(AppearanceCharacteristicsHandle* handle, StringObjectHandle* value)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    StringObjectBase* caption = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(caption);

    try
    {
        obj->SetDownCaption(caption);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_GetBaseObject(AppearanceCharacteristicsHandle* handle, DictionaryObjectHandle** result)
{
    AppearanceCharacteristics* obj = reinterpret_cast<AppearanceCharacteristics*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto base_object = obj->GetObject();
        auto ptr = base_object.AddRefGet();
        *result = reinterpret_cast<DictionaryObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_ToUnknown(AppearanceCharacteristicsHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<AppearanceCharacteristics, IUnknown, AppearanceCharacteristicsHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_FromUnknown(IUnknownHandle* handle, AppearanceCharacteristicsHandle** result) {
    return SafeObjectConvert<IUnknown, AppearanceCharacteristics, IUnknownHandle, AppearanceCharacteristicsHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION AppearanceCharacteristics_Release(AppearanceCharacteristicsHandle* handle) {
    return ObjectRelease<AppearanceCharacteristics, AppearanceCharacteristicsHandle>(handle);
}
