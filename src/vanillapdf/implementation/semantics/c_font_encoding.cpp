#include "precompiled.h"
#include "vanillapdf/semantics/c_font_encoding.h"
#include "implementation/c_helper.h"
#include "semantics/objects/font.h"
#include "semantics/objects/character_encoding.h"
#include "syntax/objects/object.h"
#include "syntax/objects/array_object.h"

#include <cstring>
#include <memory>

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

// Font encoding functions
VANILLAPDF_API error_type CALLING_CONVENTION Font_GetEncoding(FontHandle* handle, FontEncodingHandle** result) {
    FontBase* font = reinterpret_cast<FontBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(font);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto encoding = font->GetEncoding();
        
        // Create a new encoding object and transfer ownership
        auto encoding_ptr = encoding.release();
        encoding_ptr->AddRef();
        
        *result = reinterpret_cast<FontEncodingHandle*>(encoding_ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Font_DecodeText(FontHandle* handle, const uint8_t* codes, size_t count, char** result, size_t* result_length) {
    FontBase* font = reinterpret_cast<FontBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(font);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(codes);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result_length);

    if (count == 0) {
        *result = nullptr;
        *result_length = 0;
        return VANILLAPDF_ERROR_SUCCESS;
    }

    try {
        std::string decoded_text = font->DecodeText(codes, count);
        
        // Allocate memory for the result
        char* text_buffer = static_cast<char*>(malloc(decoded_text.length() + 1));
        if (!text_buffer) {
            return VANILLAPDF_ERROR_OUT_OF_MEMORY;
        }
        
        std::memcpy(text_buffer, decoded_text.c_str(), decoded_text.length());
        text_buffer[decoded_text.length()] = '\0';
        
        *result = text_buffer;
        *result_length = decoded_text.length();
        
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Font_DecodeCharacter(FontHandle* handle, uint8_t code, uint32_t* result) {
    FontBase* font = reinterpret_cast<FontBase*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(font);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = font->DecodeCharacter(code);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Font_FreeDecodedText(char* text) {
    if (text) {
        free(text);
    }
    return VANILLAPDF_ERROR_SUCCESS;
}

// FontEncoding functions
VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_CreateFromObject(ObjectHandle* encoding_obj, FontEncodingHandle** result) {
    syntax::Object* obj = reinterpret_cast<syntax::Object*>(encoding_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto encoding = CharacterEncoding::Create(obj);
        
        // Transfer ownership and add reference
        auto encoding_ptr = encoding.release();
        encoding_ptr->AddRef();
        
        *result = reinterpret_cast<FontEncodingHandle*>(encoding_ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_CreateByType(FontEncodingType type, FontEncodingHandle** result) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        CharacterEncoding::Type encoding_type;
        
        switch (type) {
            case FontEncodingType_StandardEncoding:
                encoding_type = CharacterEncoding::Type::StandardEncoding;
                break;
            case FontEncodingType_MacRomanEncoding:
                encoding_type = CharacterEncoding::Type::MacRomanEncoding;
                break;
            case FontEncodingType_WinAnsiEncoding:
                encoding_type = CharacterEncoding::Type::WinAnsiEncoding;
                break;
            case FontEncodingType_PDFDocEncoding:
                encoding_type = CharacterEncoding::Type::PDFDocEncoding;
                break;
            case FontEncodingType_MacExpertEncoding:
                encoding_type = CharacterEncoding::Type::MacExpertEncoding;
                break;
            case FontEncodingType_SymbolEncoding:
                encoding_type = CharacterEncoding::Type::SymbolEncoding;
                break;
            case FontEncodingType_ZapfDingbatsEncoding:
                encoding_type = CharacterEncoding::Type::ZapfDingbatsEncoding;
                break;
            default:
                return VANILLAPDF_ERROR_INVALID_PARAMETER;
        }
        
        auto encoding = CharacterEncoding::Create(encoding_type);
        
        // Transfer ownership and add reference
        auto encoding_ptr = encoding.release();
        encoding_ptr->AddRef();
        
        *result = reinterpret_cast<FontEncodingHandle*>(encoding_ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_GetType(FontEncodingHandle* handle, FontEncodingType* result) {
    CharacterEncoding* encoding = reinterpret_cast<CharacterEncoding*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(encoding);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    CharacterEncoding::Type type = encoding->GetEncodingType();
    
    switch (type) {
        case CharacterEncoding::Type::StandardEncoding:
            *result = FontEncodingType_StandardEncoding;
            break;
        case CharacterEncoding::Type::MacRomanEncoding:
            *result = FontEncodingType_MacRomanEncoding;
            break;
        case CharacterEncoding::Type::WinAnsiEncoding:
            *result = FontEncodingType_WinAnsiEncoding;
            break;
        case CharacterEncoding::Type::PDFDocEncoding:
            *result = FontEncodingType_PDFDocEncoding;
            break;
        case CharacterEncoding::Type::MacExpertEncoding:
            *result = FontEncodingType_MacExpertEncoding;
            break;
        case CharacterEncoding::Type::SymbolEncoding:
            *result = FontEncodingType_SymbolEncoding;
            break;
        case CharacterEncoding::Type::ZapfDingbatsEncoding:
            *result = FontEncodingType_ZapfDingbatsEncoding;
            break;
        case CharacterEncoding::Type::Custom:
            *result = FontEncodingType_Custom;
            break;
        default:
            *result = FontEncodingType_Undefined;
            break;
    }
    
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_CodeToUnicode(FontEncodingHandle* handle, uint8_t code, uint32_t* result) {
    CharacterEncoding* encoding = reinterpret_cast<CharacterEncoding*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(encoding);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = encoding->CodeToUnicode(code);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_CodeToGlyphName(FontEncodingHandle* handle, uint8_t code, char** result) {
    CharacterEncoding* encoding = reinterpret_cast<CharacterEncoding*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(encoding);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        std::string glyph_name = encoding->CodeToGlyphName(code);
        
        // Allocate memory for the result
        char* name_buffer = static_cast<char*>(malloc(glyph_name.length() + 1));
        if (!name_buffer) {
            return VANILLAPDF_ERROR_OUT_OF_MEMORY;
        }
        
        std::strcpy(name_buffer, glyph_name.c_str());
        *result = name_buffer;
        
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_ApplyDifferences(FontEncodingHandle* handle, ArrayObjectHandle* differences) {
    CharacterEncoding* encoding = reinterpret_cast<CharacterEncoding*>(handle);
    syntax::ArrayObject* diff_array = reinterpret_cast<syntax::ArrayObject*>(differences);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(encoding);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(diff_array);

    try {
        encoding->ApplyDifferences(diff_array);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_FreeGlyphName(char* glyph_name) {
    if (glyph_name) {
        free(glyph_name);
    }
    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_ToUnknown(FontEncodingHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<CharacterEncoding, IUnknown, FontEncodingHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_FromUnknown(IUnknownHandle* handle, FontEncodingHandle** result) {
    return SafeObjectConvert<IUnknown, CharacterEncoding, IUnknownHandle, FontEncodingHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_Release(FontEncodingHandle* handle) {
    return ObjectRelease<CharacterEncoding, FontEncodingHandle>(handle);
}

// High-level text decoding function for .NET interop
VANILLAPDF_API error_type CALLING_CONVENTION PDF_DecodeTextWithFont(
    FontHandle* font_handle,
    const uint8_t* byte_data,
    size_t byte_count,
    char** decoded_text,
    size_t* text_length
) {
    RETURN_ERROR_PARAM_VALUE_IF_NULL(font_handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(byte_data);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(decoded_text);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(text_length);

    if (byte_count == 0) {
        *decoded_text = nullptr;
        *text_length = 0;
        return VANILLAPDF_ERROR_SUCCESS;
    }

    // Use the existing Font_DecodeText function
    return Font_DecodeText(font_handle, byte_data, byte_count, decoded_text, text_length);
}

VANILLAPDF_API error_type CALLING_CONVENTION PDF_FreeDecodedText(char* text) {
    return Font_FreeDecodedText(text);
}