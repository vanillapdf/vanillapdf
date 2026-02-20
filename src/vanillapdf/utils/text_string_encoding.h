#ifndef _TEXT_STRING_ENCODING_H
#define _TEXT_STRING_ENCODING_H

#include <cstdint>
#include <string>
#include <string_view>

namespace vanillapdf {

/**
 * @brief Utilities for PDF text string encoding detection and conversion.
 *
 * Detect and PDFDocToUnicode are exposed via the C API.
 * ToUtf8 is internal-only, used where the library needs to interpret
 * text string content (e.g. date parsing).
 */
class TextStringEncoding {
public:
    /**
     * @brief Encoding types for PDF text strings (PDF spec 7.9.2.2)
     *
     * PDF text strings can be encoded as:
     * - PDFDocEncoding (no BOM, single-byte)
     * - UTF-16BE (BOM 0xFEFF)
     * - UTF-8 (BOM 0xEFBBBF, PDF 2.0)
     */
    enum class Type {
        Undefined = 0,
        PDFDocEncoding,
        UTF16BE,
        UTF8
    };

    /**
     * @brief Detect the encoding of a PDF text string from its raw bytes.
     *
     * Checks for UTF-16BE BOM (0xFEFF) and UTF-8 BOM (0xEFBBBF).
     * Returns PDFDocEncoding if no BOM is found.
     */
    static Type Detect(std::string_view data);

    /**
     * @brief Map a single PDFDocEncoding byte to its Unicode code point.
     *
     * Undefined bytes (e.g. 0x00-0x07) map to U+FFFD (replacement character).
     */
    static char32_t PDFDocToUnicode(uint8_t byte);

    /**
     * @brief Decode a PDF text string to a UTF-8 std::string.
     *
     * Detects encoding via BOM, strips BOM, converts to UTF-8:
     * - PDFDocEncoding: maps each byte through Table D.2, encodes as UTF-8
     * - UTF-16BE: decodes surrogate pairs, encodes as UTF-8
     * - UTF-8: strips 3-byte BOM, returns remaining bytes
     *
     * Internal use only — not exposed via C API.
     */
    static std::string ToUtf8(std::string_view data);

    /**
     * @brief Decode a PDF text string to UTF-8 using a pre-detected encoding.
     *
     * Same as ToUtf8(data) but skips BOM detection, using the supplied
     * encoding instead. Throws InvalidParameterException for Type::Undefined.
     *
     * Internal use only — not exposed via C API.
     */
    static std::string ToUtf8(std::string_view data, Type encoding);
};

} // vanillapdf

#endif /* _TEXT_STRING_ENCODING_H */
