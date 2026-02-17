#ifndef _C_PDF_TEXT_STRING_H
#define _C_PDF_TEXT_STRING_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"
#include "vanillapdf/utils/c_text_string_encoding.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_pdf_text_string.h
    * \brief This file contains class definitions for \ref PdfTextStringHandle
    */

    /**
    * \class PdfTextStringHandle
    * \extends IUnknownHandle
    * \ingroup group_semantics
    * \brief Wraps a PDF text string with detected encoding and conversion methods.
    *
    * PDF text strings (used for document info, form fields, annotations, etc.)
    * can be encoded as PDFDocEncoding, UTF-16BE, or UTF-8. This class detects
    * the encoding from BOM markers and provides conversion to common formats.
    */

    /**
    * \memberof PdfTextStringHandle
    * @{
    */

    /**
    * \brief Create from a StringObject (literal or hexadecimal).
    *
    * Auto-detects encoding via BOM. Use this when reading strings from a PDF file.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromStringObject(StringObjectHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Create from raw PDF text string bytes.
    *
    * Auto-detects encoding via BOM (UTF-16BE or UTF-8) and falls back
    * to PDFDocEncoding if no BOM is present. Mirrors \ref PdfTextString_GetStringRaw.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromRaw(BufferHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Create from UTF-8 bytes.
    *
    * Converts to the optimal PDF storage encoding: PDFDocEncoding if all
    * characters fit, otherwise UTF-16BE with BOM.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromUtf8(BufferHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Create from UTF-16BE bytes (without BOM).
    *
    * Prepends the UTF-16BE BOM and stores the result.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromUtf16(BufferHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Get the detected encoding of the original raw bytes.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetEncoding(PdfTextStringHandle* handle, TextStringEncodingType* result);

    /**
    * \brief Get the original raw bytes (no conversion).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetStringRaw(PdfTextStringHandle* handle, BufferHandle** result);

    /**
    * \brief Convert to UTF-8 (fresh conversion each call).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetStringUtf8(PdfTextStringHandle* handle, BufferHandle** result);

    /**
    * \brief Convert to UTF-16BE without BOM (fresh conversion each call).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetStringUtf16(PdfTextStringHandle* handle, BufferHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_ToUnknown(PdfTextStringHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref PdfTextStringHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_FromUnknown(IUnknownHandle* handle, PdfTextStringHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_Release(PdfTextStringHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_PDF_TEXT_STRING_H */
