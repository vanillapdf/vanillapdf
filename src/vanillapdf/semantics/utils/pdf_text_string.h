#ifndef _PDF_TEXT_STRING_H
#define _PDF_TEXT_STRING_H

#include "utils/utils_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/text_string_encoding.h"

#include "syntax/utils/syntax_fwd.h"

#include <string_view>

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
 * Backed by a StringObjectPtr so mutations propagate to the underlying
 * PDF string object.
 *
 * Conversions are computed fresh on each call (no caching).
 */
class PdfTextString : public IUnknown {
public:
    /**
     * @brief Create from a StringObject (literal or hexadecimal).
     *
     * Wraps the given string object directly. Use this when reading
     * strings from a PDF file — mutations via Set methods will
     * propagate back to the original object.
     */
    static PdfTextStringPtr CreateFromStringObject(syntax::StringObjectPtr obj);

    /**
     * @brief Create from raw PDF text string bytes.
     *
     * Creates a new LiteralStringObject from the given data and
     * auto-detects encoding via BOM (UTF-16BE or UTF-8), falling
     * back to PDFDocEncoding if no BOM is present.
     * Mirrors \ref GetStringRaw.
     */
    static PdfTextStringPtr CreateFromRaw(std::string_view data);

    /**
     * @brief Create from UTF-8 bytes.
     *
     * Converts the input to the optimal PDF storage encoding:
     * PDFDocEncoding if all characters fit, otherwise UTF-16BE with BOM.
     */
    static PdfTextStringPtr CreateFromUtf8(std::string_view data);

    /**
     * @brief Create from UTF-16BE bytes (without BOM).
     *
     * Prepends the UTF-16BE BOM (0xFE 0xFF) and stores the result.
     */
    static PdfTextStringPtr CreateFromUtf16(std::string_view data);

    /**
     * @brief Get the detected encoding of the raw bytes.
     */
    TextStringEncoding GetEncoding() const;

    /**
     * @brief Get the raw bytes (no conversion).
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

    /**
     * @brief Set from raw PDF text string bytes.
     *
     * Replaces the backing StringObject value and re-detects encoding.
     */
    void SetStringRaw(std::string_view data);

    /**
     * @brief Set from UTF-8 bytes.
     *
     * Converts to optimal PDF storage encoding and updates the backing
     * StringObject.
     */
    void SetStringUtf8(std::string_view data);

    /**
     * @brief Set from UTF-16BE bytes (without BOM).
     *
     * Prepends BOM and updates the backing StringObject.
     */
    void SetStringUtf16(std::string_view data);

    /**
     * @brief Get the backing StringObject.
     */
    syntax::StringObjectPtr GetStringObject() const;

    /**
     * @brief Replace the backing StringObject.
     */
    void SetStringObject(syntax::StringObjectPtr obj);

    PdfTextString() = default;
    explicit PdfTextString(syntax::StringObjectPtr obj);

private:
    syntax::StringObjectPtr m_obj;
};

} // semantics
} // vanillapdf

#endif /* _PDF_TEXT_STRING_H */
