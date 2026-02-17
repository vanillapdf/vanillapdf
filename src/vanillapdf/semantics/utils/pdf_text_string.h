#ifndef _PDF_TEXT_STRING_H
#define _PDF_TEXT_STRING_H

#include "utils/utils_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/text_string_encoding.h"

#include "syntax/utils/syntax_fwd.h"

namespace vanillapdf {
namespace semantics {

class PdfTextString;
using PdfTextStringPtr = Deferred<PdfTextString>;

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
     * Extracts the decoded value from the string object and auto-detects
     * its encoding via BOM. Use this when reading strings from a PDF file.
     */
    static PdfTextStringPtr CreateFromStringObject(syntax::StringObjectPtr obj);

    /**
     * @brief Create from raw PDF text string bytes.
     *
     * Auto-detects encoding via BOM (UTF-16BE or UTF-8) and falls back
     * to PDFDocEncoding if no BOM is present. Mirrors \ref GetStringRaw.
     */
    static PdfTextStringPtr CreateFromRaw(BufferPtr bytes);

    /**
     * @brief Create from UTF-8 bytes.
     *
     * Converts the input to the optimal PDF storage encoding:
     * PDFDocEncoding if all characters fit, otherwise UTF-16BE with BOM.
     */
    static PdfTextStringPtr CreateFromUtf8(BufferPtr utf8_bytes);

    /**
     * @brief Create from UTF-16BE bytes (without BOM).
     *
     * Prepends the UTF-16BE BOM (0xFE 0xFF) and stores the result.
     */
    static PdfTextStringPtr CreateFromUtf16(BufferPtr utf16be_bytes);

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
    PdfTextString(BufferPtr raw_data, TextStringEncoding encoding);

private:
    BufferPtr m_raw_data;
    TextStringEncoding m_encoding = TextStringEncoding::Undefined;
};

} // semantics
} // vanillapdf

#endif /* _PDF_TEXT_STRING_H */
