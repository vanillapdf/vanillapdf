#include "precompiled.h"

#include "utils/buffer.h"
#include "utils/text_string_encoding.h"

#include "vanillapdf/utils/c_text_string_encoding.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_Detect(BufferHandle* handle, TextStringEncodingType* result)
{
    Buffer* obj = reinterpret_cast<Buffer*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto encoding = DetectTextStringEncoding(*obj);

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

VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_ToUtf8(BufferHandle* handle, BufferHandle** result)
{
    Buffer* obj = reinterpret_cast<Buffer*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto utf8 = TextStringToUtf8(*obj);
        auto ptr = utf8.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_ToUtf16(BufferHandle* handle, BufferHandle** result)
{
    Buffer* obj = reinterpret_cast<Buffer*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto utf16 = TextStringToUtf16BE(*obj);
        auto ptr = utf16.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_FromUtf8(BufferHandle* handle, BufferHandle** result)
{
    Buffer* obj = reinterpret_cast<Buffer*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        auto text_string = Utf8ToTextString(*obj);
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<BufferHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}
