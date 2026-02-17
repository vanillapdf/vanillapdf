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
        auto value = obj->GetValue();
        auto encoding = DetectTextStringEncoding(*value);
        auto text_string = make_deferred<PdfTextString>(std::move(value), encoding);
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromRaw(BufferHandle* handle, PdfTextStringHandle** result)
{
    Buffer* obj = reinterpret_cast<Buffer*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto cloned = obj->Clone();
        auto text_string = PdfTextString::CreateFromRaw(std::move(cloned));
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromUtf8(BufferHandle* handle, PdfTextStringHandle** result)
{
    Buffer* obj = reinterpret_cast<Buffer*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto cloned = obj->Clone();
        auto text_string = PdfTextString::CreateFromUtf8(std::move(cloned));
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromUtf16(BufferHandle* handle, PdfTextStringHandle** result)
{
    Buffer* obj = reinterpret_cast<Buffer*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto cloned = obj->Clone();
        auto text_string = PdfTextString::CreateFromUtf16(std::move(cloned));
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

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_ToUnknown(PdfTextStringHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<PdfTextString, IUnknown, PdfTextStringHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_FromUnknown(IUnknownHandle* handle, PdfTextStringHandle** result) {
    return SafeObjectConvert<IUnknown, PdfTextString, IUnknownHandle, PdfTextStringHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_Release(PdfTextStringHandle* handle) {
    return ObjectRelease<PdfTextString, PdfTextStringHandle>(handle);
}
