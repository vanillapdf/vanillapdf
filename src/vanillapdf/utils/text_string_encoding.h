#ifndef _TEXT_STRING_ENCODING_H
#define _TEXT_STRING_ENCODING_H

#include <cstddef>
#include <cstdint>

namespace vanillapdf {

/**
 * @brief Encoding types for PDF text strings (PDF spec 7.9.2.2)
 *
 * PDF text strings can be encoded as:
 * - PDFDocEncoding (no BOM, single-byte)
 * - UTF-16BE (BOM 0xFEFF)
 * - UTF-8 (BOM 0xEFBBBF, PDF 2.0)
 */
enum class TextStringEncoding {
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
TextStringEncoding DetectTextStringEncoding(const char* data, size_t size);

/**
 * @brief Map a single PDFDocEncoding byte to its Unicode code point.
 *
 * Undefined bytes (e.g. 0x00–0x07) map to U+FFFD (replacement character).
 */
char32_t PDFDocEncodingToUnicode(uint8_t byte);

} // vanillapdf

#endif /* _TEXT_STRING_ENCODING_H */
