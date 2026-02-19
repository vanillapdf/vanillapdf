#include "precompiled.h"

#include "utils/text_string_encoding.h"

#include "vanillapdf/utils/c_text_string_encoding.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;

VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_Detect(string_type data, size_type size, TextStringEncodingType* result)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(data);

    std::string_view view(data, size);
    auto encoding = TextStringEncoding::Detect(view);

    switch (encoding) {
        case TextStringEncoding::Type::PDFDocEncoding:
            *result = TextStringEncodingType_PDFDocEncoding; break;
        case TextStringEncoding::Type::UTF16BE:
            *result = TextStringEncodingType_UTF16BE; break;
        case TextStringEncoding::Type::UTF8:
            *result = TextStringEncodingType_UTF8; break;
        default:
            *result = TextStringEncodingType_Undefined; break;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION PDFDocEncoding_ToUnicode(uint8_t byte, uint32_t* codepoint)
{
    RETURN_ERROR_PARAM_VALUE_IF_NULL(codepoint);

    *codepoint = static_cast<uint32_t>(TextStringEncoding::PDFDocToUnicode(byte));
    return VANILLAPDF_ERROR_SUCCESS;
}
