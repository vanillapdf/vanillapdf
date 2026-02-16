#ifndef _TEXT_STRING_ENCODING_H
#define _TEXT_STRING_ENCODING_H

#include "utils/utils_fwd.h"

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
TextStringEncoding DetectTextStringEncoding(const Buffer& value);

/**
 * @brief Convert a PDF text string (any encoding) to UTF-8.
 *
 * Detects encoding via BOM, strips BOM, and converts to UTF-8.
 * If already UTF-8, strips BOM and returns content.
 */
BufferPtr TextStringToUtf8(const Buffer& value);

/**
 * @brief Convert a PDF text string (any encoding) to UTF-16BE.
 *
 * Detects encoding via BOM, strips BOM, and converts to UTF-16BE.
 * The result does NOT include a BOM prefix.
 */
BufferPtr TextStringToUtf16BE(const Buffer& value);

/**
 * @brief Convert a UTF-8 string to a PDF text string.
 *
 * If all characters fit in PDFDocEncoding, returns PDFDocEncoding bytes (no BOM).
 * Otherwise returns UTF-16BE with BOM prefix.
 */
BufferPtr Utf8ToTextString(const Buffer& utf8_value);

} // vanillapdf

#endif /* _TEXT_STRING_ENCODING_H */
