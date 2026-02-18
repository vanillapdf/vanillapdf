#include "precompiled.h"

#include "utils/buffer.h"
#include "semantics/utils/pdf_text_string.h"

#include "syntax/objects/string_object.h"

#include "vanillapdf/semantics/c_pdf_text_string.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromStringObject(StringObjectHandle* handle, PdfTextStringHandle** result)
{
    StringObjectBase* obj = reinterpret_cast<StringObjectBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        StringObjectPtr str_obj(obj);
        auto text_string = PdfTextString::CreateFromStringObject(str_obj);
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromRaw(string_type data, size_type size, PdfTextStringHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto text_string = PdfTextString::CreateFromRaw(std::string_view(data, size));
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromUtf8(string_type data, size_type size, PdfTextStringHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto text_string = PdfTextString::CreateFromUtf8(std::string_view(data, size));
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromUtf16(string_type data, size_type size, PdfTextStringHandle** result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto text_string = PdfTextString::CreateFromUtf16(std::string_view(data, size));
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetEncoding(PdfTextStringHandle* handle, TextStringEncodingType* result)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto encoding = obj->GetEncoding();

        switch (encoding) {
            case TextStringEncoding::PDFDocEncoding:
                *result = TextStringEncodingType_PDFDocEncoding; break;
            case TextStringEncoding::UTF16BE:
                *result = TextStringEncodingType_UTF16BE; break;
            case TextStringEncoding::UTF8:
                *result = TextStringEncodingType_UTF8; break;
            default:
                *result = TextStringEncodingType_Undefined; break;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetStringRaw(PdfTextStringHandle* handle, BufferHandle** result)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto raw = obj->GetStringRaw();
        auto ptr = raw.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetStringUtf8(PdfTextStringHandle* handle, BufferHandle** result)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto utf8 = obj->GetStringUtf8();
        auto ptr = utf8.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetStringUtf16(PdfTextStringHandle* handle, BufferHandle** result)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto utf16 = obj->GetStringUtf16();
        auto ptr = utf16.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_SetStringRaw(PdfTextStringHandle* handle, string_type data, size_type size)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    try
    {
        obj->SetStringRaw(std::string_view(data, size));
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_SetStringUtf8(PdfTextStringHandle* handle, string_type data, size_type size)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    try
    {
        obj->SetStringUtf8(std::string_view(data, size));
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_SetStringUtf16(PdfTextStringHandle* handle, string_type data, size_type size)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    try
    {
        obj->SetStringUtf16(std::string_view(data, size));
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetStringObject(PdfTextStringHandle* handle, StringObjectHandle** result)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto str_obj = obj->GetStringObject();
        auto ptr = str_obj.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_SetStringObject(PdfTextStringHandle* handle, StringObjectHandle* value)
{
    PdfTextString* obj = reinterpret_cast<PdfTextString*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(value);

    try
    {
        StringObjectBase* str_base = reinterpret_cast<StringObjectBase*>(value);
        StringObjectPtr str_obj(str_base);
        obj->SetStringObject(str_obj);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_ToUnknown(PdfTextStringHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<PdfTextString, IUnknown, PdfTextStringHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_FromUnknown(IUnknownHandle* handle, PdfTextStringHandle** result) {
    return SafeObjectConvert<IUnknown, PdfTextString, IUnknownHandle, PdfTextStringHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_Release(PdfTextStringHandle* handle) {
    return ObjectRelease<PdfTextString, PdfTextStringHandle>(handle);
}
