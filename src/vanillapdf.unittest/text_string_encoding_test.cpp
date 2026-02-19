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
    ASSERT_EQ(TextStringEncoding_Detect(nullptr, 0, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_PDFDocEncoding);
}

TEST(TextStringEncoding, NullResultParam) {
    const char data[] = "Hello";
    EXPECT_EQ(TextStringEncoding_Detect(data, 5, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// ===== PDFDocEncoding → Unicode =====

TEST(PDFDocEncoding, ASCIIPassthrough) {
    // 0x41 = 'A' → U+0041
    uint32_t cp = 0;
    ASSERT_EQ(PDFDocEncoding_ToUnicode(0x41, &cp), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(cp, 0x0041u);
}

TEST(PDFDocEncoding, HighByteBullet) {
    // 0x80 = U+2022 (bullet)
    uint32_t cp = 0;
    ASSERT_EQ(PDFDocEncoding_ToUnicode(0x80, &cp), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(cp, 0x2022u);
}

TEST(PDFDocEncoding, UndefinedByteIsReplacementChar) {
    // 0x00 is undefined in PDFDocEncoding → U+FFFD
    uint32_t cp = 0;
    ASSERT_EQ(PDFDocEncoding_ToUnicode(0x00, &cp), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(cp, 0xFFFDu);
}

TEST(PDFDocEncoding, NullResultParam) {
    EXPECT_EQ(PDFDocEncoding_ToUnicode(0x41, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

} // namespace text_string_encoding
