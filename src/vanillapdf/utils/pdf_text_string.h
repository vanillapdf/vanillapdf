#ifndef _PDF_TEXT_STRING_H
#define _PDF_TEXT_STRING_H

#include "utils/utils_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/text_string_encoding.h"

#include "syntax/utils/syntax_fwd.h"

namespace vanillapdf {

class PdfTextString;
using PdfTextStringPtr = Deferred<PdfTextString>;
using OutputPdfTextStringPtr = PdfTextStringPtr&;

/**
 * @brief Wraps a PDF text string with detected encoding and conversion methods.
 *
 * PDF text strings (used for document info, form fields, annotations, etc.)
 * can be encoded as PDFDocEncoding, UTF-16BE, or UTF-8. This class detects
 * the encoding from BOM markers and provides conversion to common formats.
 *
 * Conversions are computed fresh on each call (no caching).
 */
class PdfTextString : public IUnknown {
public:
    /**
     * @brief Create from a StringObject (literal or hexadecimal).
     *
     * Extracts the decoded value from the string object and detects its encoding.
     */
    static PdfTextStringPtr CreateFromStringObject(syntax::StringObjectPtr obj);

    /**
     * @brief Create from raw bytes with auto-detected encoding.
     */
    static PdfTextStringPtr CreateFromRawBytes(BufferPtr raw_data);

    /**
     * @brief Create from a UTF-8 std::string.
     *
     * The raw bytes are set to the optimal PDF text string encoding
     * (PDFDocEncoding if possible, otherwise UTF-16BE with BOM).
     */
    static PdfTextStringPtr CreateFromUtf8(const std::string& utf8_text);

    /**
     * @brief Get the detected encoding of the original raw bytes.
     */
    TextStringEncoding GetEncoding() const;

    /**
     * @brief Get the original raw bytes (no conversion).
     */
    BufferPtr GetStringRaw() const;

    /**
     * @brief Convert to UTF-8 (fresh conversion each call).
     */
    BufferPtr GetStringUtf8() const;

    /**
     * @brief Convert to UTF-16BE without BOM (fresh conversion each call).
     */
    BufferPtr GetStringUtf16() const;

    PdfTextString() = default;
    explicit PdfTextString(BufferPtr raw_data);

private:
    BufferPtr m_raw_data;
    TextStringEncoding m_encoding = TextStringEncoding::Undefined;
};

} // vanillapdf

#endif /* _PDF_TEXT_STRING_H */
