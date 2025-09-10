#ifndef _C_FONT_ENCODING_H
#define _C_FONT_ENCODING_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_font_encoding.h
    * \brief This file contains class definitions for font encoding and text decoding functionality.
    */

    /**
    * \class FontEncodingHandle
    * \extends IUnknownHandle
    * \ingroup group_fonts
    * \brief
    * Represents a character encoding used by PDF fonts for text decoding.
    */

    /**
    * \brief Types of font encodings
    * \ingroup group_fonts
    */
    typedef enum {
        FontEncodingType_Undefined = 0,
        FontEncodingType_StandardEncoding,
        FontEncodingType_MacRomanEncoding,
        FontEncodingType_WinAnsiEncoding,
        FontEncodingType_PDFDocEncoding,
        FontEncodingType_MacExpertEncoding,
        FontEncodingType_SymbolEncoding,
        FontEncodingType_ZapfDingbatsEncoding,
        FontEncodingType_Custom
    } FontEncodingType;

    /**
    * \memberof FontHandle
    * @{
    */

    /**
    * \brief Get the character encoding for a font
    * \param handle Font handle
    * \param result Output parameter for the encoding handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_GetEncoding(FontHandle* handle, FontEncodingHandle** result);

    /**
    * \brief Decode character codes to Unicode text using font encoding
    * \param handle Font handle
    * \param codes Array of character codes to decode
    * \param count Number of character codes
    * \param result Output parameter for the decoded UTF-8 text string
    * \param result_length Output parameter for the length of the result string
    * 
    * \note The caller is responsible for freeing the result string using Font_FreeDecodedText
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_DecodeText(FontHandle* handle, const uint8_t* codes, size_t count, char** result, size_t* result_length);

    /**
    * \brief Decode a single character code to Unicode
    * \param handle Font handle
    * \param code Character code to decode
    * \param result Output parameter for the Unicode code point
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_DecodeCharacter(FontHandle* handle, uint8_t code, uint32_t* result);

    /**
    * \brief Free memory allocated by Font_DecodeText
    * \param text Text string to free
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Font_FreeDecodedText(char* text);

    /** @} */

    /**
    * \memberof FontEncodingHandle
    * @{
    */

    /**
    * \brief Create encoding from PDF object
    * \param encoding_obj PDF encoding object (name or dictionary)
    * \param result Output parameter for the encoding handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_CreateFromObject(ObjectHandle* encoding_obj, FontEncodingHandle** result);

    /**
    * \brief Create encoding by type
    * \param type Encoding type
    * \param result Output parameter for the encoding handle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_CreateByType(FontEncodingType type, FontEncodingHandle** result);

    /**
    * \brief Get encoding type
    * \param handle Encoding handle
    * \param result Output parameter for the encoding type
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_GetType(FontEncodingHandle* handle, FontEncodingType* result);

    /**
    * \brief Convert character code to Unicode
    * \param handle Encoding handle
    * \param code Character code (0-255)
    * \param result Output parameter for the Unicode code point
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_CodeToUnicode(FontEncodingHandle* handle, uint8_t code, uint32_t* result);

    /**
    * \brief Convert character code to glyph name
    * \param handle Encoding handle
    * \param code Character code (0-255)
    * \param result Output parameter for the glyph name (caller must free)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_CodeToGlyphName(FontEncodingHandle* handle, uint8_t code, char** result);

    /**
    * \brief Apply differences array to modify the encoding
    * \param handle Encoding handle
    * \param differences Array object containing encoding differences
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_ApplyDifferences(FontEncodingHandle* handle, ArrayObjectHandle* differences);

    /**
    * \brief Free glyph name string allocated by FontEncoding_CodeToGlyphName
    * \param glyph_name Glyph name string to free
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_FreeGlyphName(char* glyph_name);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_ToUnknown(FontEncodingHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref FontEncodingHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_FromUnknown(IUnknownHandle* handle, FontEncodingHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FontEncoding_Release(FontEncodingHandle* handle);

    /** @} */

    /**
    * \brief High-level text decoding function for .NET interop
    * 
    * This function provides a simple interface for .NET applications to decode
    * PDF text content using the appropriate font and encoding.
    * 
    * \param font_handle Font handle from PDF
    * \param byte_data Raw byte data from PDF content stream
    * \param byte_count Number of bytes in the data
    * \param decoded_text Output parameter for decoded UTF-8 text (caller must free)
    * \param text_length Output parameter for length of decoded text
    * 
    * \return Error code indicating success or failure
    * 
    * \note This is the primary function that .NET applications should use for text decoding.
    *       It handles all the complexity of font encoding, differences, and Unicode conversion.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PDF_DecodeTextWithFont(
        FontHandle* font_handle,
        const uint8_t* byte_data,
        size_t byte_count,
        char** decoded_text,
        size_t* text_length
    );

    /**
    * \brief Free text allocated by PDF_DecodeTextWithFont
    * \param text Text string to free
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PDF_FreeDecodedText(char* text);

#ifdef __cplusplus
};
#endif

#endif /* _C_FONT_ENCODING_H */