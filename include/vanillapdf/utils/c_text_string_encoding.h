#ifndef _C_TEXT_STRING_ENCODING_H
#define _C_TEXT_STRING_ENCODING_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_text_string_encoding.h
    * \brief Utilities for PDF text string encoding detection and PDFDocEncoding lookup.
    */

    /**
    * \brief Encoding types for PDF text strings (PDF spec 7.9.2.2).
    *
    * PDF text strings can be encoded as PDFDocEncoding (no BOM),
    * UTF-16BE (BOM 0xFEFF), or UTF-8 (BOM 0xEFBBBF).
    */
    typedef enum {
        TextStringEncodingType_Undefined = 0,

        /**
        * \brief Single-byte encoding defined in PDF spec Table D.2.
        * No byte order mark is present.
        */
        TextStringEncodingType_PDFDocEncoding,

        /**
        * \brief UTF-16 big-endian encoding.
        * Indicated by byte order mark 0xFEFF at the start of the string.
        */
        TextStringEncodingType_UTF16BE,

        /**
        * \brief UTF-8 encoding (PDF 2.0).
        * Indicated by byte order mark 0xEFBBBF at the start of the string.
        */
        TextStringEncodingType_UTF8
    } TextStringEncodingType;

    /**
    * \brief Detect the encoding of a PDF text string from its raw bytes.
    * \param[in] data  Pointer to the raw PDF text string bytes.
    * \param[in] size  Number of bytes in \p data.
    * \param[out] result The detected encoding type.
    * \ingroup group_utils
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_Detect(string_type data, size_type size, TextStringEncodingType* result);

    /**
    * \brief Map a single PDFDocEncoding byte to its Unicode code point.
    * \param[in]  byte      The PDFDocEncoding byte value (0x00–0xFF).
    * \param[out] codepoint The corresponding Unicode code point.
    *             Undefined bytes (e.g. 0x00–0x07) map to U+FFFD (replacement character).
    * \ingroup group_utils
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_PDFDocToUnicode(uint8_t byte, uint32_t* codepoint);

#ifdef __cplusplus
};
#endif

#endif /* _C_TEXT_STRING_ENCODING_H */
