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
    * \brief This file contains definitions for PDF text string encoding detection and conversion.
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
    * \param[in] handle Buffer containing raw PDF text string bytes.
    * \param[out] result The detected encoding type.
    * \ingroup group_utils
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_Detect(BufferHandle* handle, TextStringEncodingType* result);

    /**
    * \brief Convert a PDF text string (any encoding) to UTF-8.
    * \param[in] handle Buffer containing raw PDF text string bytes.
    * \param[out] result New buffer with UTF-8 encoded content (BOM stripped).
    * \ingroup group_utils
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_ToUtf8(BufferHandle* handle, BufferHandle** result);

    /**
    * \brief Convert a PDF text string (any encoding) to UTF-16BE.
    * \param[in] handle Buffer containing raw PDF text string bytes.
    * \param[out] result New buffer with UTF-16BE encoded content (no BOM).
    * \ingroup group_utils
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_ToUtf16(BufferHandle* handle, BufferHandle** result);

    /**
    * \brief Convert UTF-8 bytes to a PDF text string.
    *
    * If all characters fit in PDFDocEncoding, returns PDFDocEncoding bytes.
    * Otherwise returns UTF-16BE with BOM prefix.
    *
    * \param[in] handle Buffer containing UTF-8 encoded content.
    * \param[out] result New buffer with PDF text string bytes.
    * \ingroup group_utils
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextStringEncoding_FromUtf8(BufferHandle* handle, BufferHandle** result);

#ifdef __cplusplus
};
#endif

#endif /* _C_TEXT_STRING_ENCODING_H */
