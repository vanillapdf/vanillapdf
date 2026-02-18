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
    *
    * Backed by a StringObject, so mutations via Set methods propagate to
    * the underlying PDF string object.
    */

    /**
    * \memberof PdfTextStringHandle
    * @{
    */

    /**
    * \brief Create from a StringObject (literal or hexadecimal).
    *
    * Wraps the given string object directly. Mutations via Set methods
    * will propagate back to the original object.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromStringObject(StringObjectHandle* handle, PdfTextStringHandle** result);

    /**
    * \brief Create from raw PDF text string bytes.
    *
    * Creates a new LiteralStringObject from the data and auto-detects
    * encoding via BOM (UTF-16BE or UTF-8), falling back to PDFDocEncoding
    * if no BOM is present. Mirrors \ref PdfTextString_GetStringRaw.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromRaw(string_type data, size_type size, PdfTextStringHandle** result);

    /**
    * \brief Create from UTF-8 bytes.
    *
    * Converts to the optimal PDF storage encoding: PDFDocEncoding if all
    * characters fit, otherwise UTF-16BE with BOM.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromUtf8(string_type data, size_type size, PdfTextStringHandle** result);

    /**
    * \brief Create from UTF-16BE bytes (without BOM).
    *
    * Prepends the UTF-16BE BOM (0xFE 0xFF) and stores the result.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_CreateFromUtf16(string_type data, size_type size, PdfTextStringHandle** result);

    /**
    * \brief Get the detected encoding of the raw bytes.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetEncoding(PdfTextStringHandle* handle, TextStringEncodingType* result);

    /**
    * \brief Get the raw bytes (no conversion).
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
    * \brief Set from raw PDF text string bytes.
    *
    * Replaces the backing StringObject value. Encoding is re-detected.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_SetStringRaw(PdfTextStringHandle* handle, string_type data, size_type size);

    /**
    * \brief Set from UTF-8 bytes.
    *
    * Converts to optimal PDF storage encoding and updates the backing
    * StringObject.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_SetStringUtf8(PdfTextStringHandle* handle, string_type data, size_type size);

    /**
    * \brief Set from UTF-16BE bytes (without BOM).
    *
    * Prepends BOM and updates the backing StringObject.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_SetStringUtf16(PdfTextStringHandle* handle, string_type data, size_type size);

    /**
    * \brief Get the backing StringObject.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_GetStringObject(PdfTextStringHandle* handle, StringObjectHandle** result);

    /**
    * \brief Replace the backing StringObject.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION PdfTextString_SetStringObject(PdfTextStringHandle* handle, StringObjectHandle* value);

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
