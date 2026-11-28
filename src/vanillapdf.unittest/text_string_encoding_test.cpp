#include "unittest.h"

#include <cstdint>

namespace text_string_encoding {

// ===== Encoding Detection =====

TEST(TextStringEncoding, DetectPDFDocEncoding) {
    // ASCII text, no BOM
    const char data[] = "Hello";
    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(data, 5, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_PDFDocEncoding);
}

TEST(TextStringEncoding, DetectUTF16BE) {
    // UTF-16BE BOM: 0xFE 0xFF
    const char data[] = { '\xFE', '\xFF', '\x00', 'H', '\x00', 'i' };
    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(data, 6, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_UTF16BE);
}

TEST(TextStringEncoding, DetectUTF8) {
    // UTF-8 BOM: 0xEF 0xBB 0xBF
    const char data[] = { '\xEF', '\xBB', '\xBF', 'H', 'e', 'l', 'l', 'o' };
    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(data, 8, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_UTF8);
}

TEST(TextStringEncoding, DetectEmptyIsPDFDoc) {
    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect("", 0, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_PDFDocEncoding);
}

TEST(TextStringEncoding, NullDataParam) {
    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    EXPECT_EQ(TextStringEncoding_Detect(nullptr, 0, &encoding), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(TextStringEncoding, NullResultParam) {
    const char data[] = "Hello";
    EXPECT_EQ(TextStringEncoding_Detect(data, 5, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// ===== PDFDocEncoding → Unicode (TextStringEncoding_PDFDocEncodingByteToUnicode) =====

TEST(TextStringEncoding, PDFDocEncodingASCIIPassthrough) {
    // 0x41 = 'A' → U+0041
    uint32_t cp = 0;
    ASSERT_EQ(TextStringEncoding_PDFDocEncodingByteToUnicode(0x41, &cp), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(cp, 0x0041u);
}

TEST(TextStringEncoding, PDFDocEncodingHighByteBullet) {
    // 0x80 = U+2022 (bullet)
    uint32_t cp = 0;
    ASSERT_EQ(TextStringEncoding_PDFDocEncodingByteToUnicode(0x80, &cp), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(cp, 0x2022u);
}

TEST(TextStringEncoding, PDFDocEncodingUndefinedByteIsReplacementChar) {
    // 0x00 is undefined in PDFDocEncoding → U+FFFD
    uint32_t cp = 0;
    ASSERT_EQ(TextStringEncoding_PDFDocEncodingByteToUnicode(0x00, &cp), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(cp, 0xFFFDu);
}

static uint32_t PDFDocEncodingByteToUnicode(uint8_t byte) {
    uint32_t codepoint = 0;
    EXPECT_EQ(TextStringEncoding_PDFDocEncodingByteToUnicode(byte, &codepoint), VANILLAPDF_ERROR_SUCCESS);
    return codepoint;
}

TEST(TextStringEncoding, PDFDocEncodingControlCharacters) {
    // ISO 32000 Table D.2: TAB, LF and CR are the only defined codes below 0x18
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x09), 0x0009u);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x0A), 0x000Au);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x0D), 0x000Du);
}

TEST(TextStringEncoding, PDFDocEncodingUndefinedControlCode) {
    // 0x08 and 0x0F are undefined in PDFDocEncoding → U+FFFD
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x08), 0xFFFDu);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x0F), 0xFFFDu);
}

TEST(TextStringEncoding, PDFDocEncodingSpacingAccents) {
    // ISO 32000 Table D.2: 0x18-0x1F are the spacing accents
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x18), 0x02D8u);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x19), 0x02C7u);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x1A), 0x02C6u);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x1B), 0x02D9u);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x1C), 0x02DDu);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x1D), 0x02DBu);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x1E), 0x02DAu);
    EXPECT_EQ(PDFDocEncodingByteToUnicode(0x1F), 0x02DCu);
}

TEST(TextStringEncoding, PDFDocEncodingNullResultParam) {
    EXPECT_EQ(TextStringEncoding_PDFDocEncodingByteToUnicode(0x41, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} // namespace text_string_encoding
