#include "unittest.h"
#include "handle_guard.h"

#include <string>
#include <cstring>

namespace text_string_encoding {

// Helper to create a buffer from raw bytes
static error_type CreateBufferFromBytes(const char* data, size_t size, BufferHandle** result) {
    return Buffer_CreateFromData(data, size, result);
}

// Helper to extract buffer content as a string
static std::string BufferToString(BufferHandle* handle) {
    string_type data = nullptr;
    size_type len = 0;
    Buffer_GetData(handle, &data, &len);
    return std::string(data, len);
}

// ===== Encoding Detection =====

TEST(TextStringEncoding, DetectPDFDocEncoding) {
    // ASCII text, no BOM
    const char data[] = "Hello";
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 5, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(buf, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_PDFDocEncoding);
}

TEST(TextStringEncoding, DetectUTF16BE) {
    // UTF-16BE BOM: 0xFE 0xFF
    const char data[] = { '\xFE', '\xFF', '\x00', 'H', '\x00', 'i' };
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 6, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(buf, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_UTF16BE);
}

TEST(TextStringEncoding, DetectUTF8) {
    // UTF-8 BOM: 0xEF 0xBB 0xBF
    const char data[] = { '\xEF', '\xBB', '\xBF', 'H', 'e', 'l', 'l', 'o' };
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 8, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(buf, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_UTF8);
}

TEST(TextStringEncoding, DetectEmptyIsPDFDoc) {
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(Buffer_Create(buf.out()), VANILLAPDF_ERROR_SUCCESS);

    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(buf, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_PDFDocEncoding);
}

// ===== PDFDocEncoding → UTF-8 =====

TEST(TextStringEncoding, PDFDocASCIIToUtf8) {
    // Plain ASCII should pass through unchanged
    const char data[] = "Hello World";
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 11, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf8;
    ASSERT_EQ(TextStringEncoding_ToUtf8(buf, utf8.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(BufferToString(utf8), "Hello World");
}

TEST(TextStringEncoding, PDFDocHighByteToUtf8) {
    // 0x80 = bullet U+2022, which in UTF-8 is E2 80 A2
    const char data[] = { '\x80' };
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 1, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf8;
    ASSERT_EQ(TextStringEncoding_ToUtf8(buf, utf8.out()), VANILLAPDF_ERROR_SUCCESS);

    auto result = BufferToString(utf8);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(static_cast<uint8_t>(result[0]), 0xE2u);
    EXPECT_EQ(static_cast<uint8_t>(result[1]), 0x80u);
    EXPECT_EQ(static_cast<uint8_t>(result[2]), 0xA2u);
}

TEST(TextStringEncoding, PDFDoc0x18RangeToUtf8) {
    // 0x18 = U+02D0 (modifier letter triangular colon)
    // UTF-8: CB 90
    const char data[] = { '\x18' };
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 1, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf8;
    ASSERT_EQ(TextStringEncoding_ToUtf8(buf, utf8.out()), VANILLAPDF_ERROR_SUCCESS);

    auto result = BufferToString(utf8);
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(static_cast<uint8_t>(result[0]), 0xCBu);
    EXPECT_EQ(static_cast<uint8_t>(result[1]), 0x90u);
}

// ===== UTF-16BE → UTF-8 =====

TEST(TextStringEncoding, UTF16BEBMPToUtf8) {
    // UTF-16BE BOM + "Hi" = FE FF 00 48 00 69
    const char data[] = { '\xFE', '\xFF', '\x00', '\x48', '\x00', '\x69' };
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 6, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf8;
    ASSERT_EQ(TextStringEncoding_ToUtf8(buf, utf8.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(BufferToString(utf8), "Hi");
}

TEST(TextStringEncoding, UTF16BESurrogatePairToUtf8) {
    // UTF-16BE BOM + U+1F600 (grinning face) = FE FF D8 3D DE 00
    const char data[] = { '\xFE', '\xFF', '\xD8', '\x3D', '\xDE', '\x00' };
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 6, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf8;
    ASSERT_EQ(TextStringEncoding_ToUtf8(buf, utf8.out()), VANILLAPDF_ERROR_SUCCESS);

    auto result = BufferToString(utf8);
    // U+1F600 in UTF-8: F0 9F 98 80
    ASSERT_EQ(result.size(), 4u);
    EXPECT_EQ(static_cast<uint8_t>(result[0]), 0xF0u);
    EXPECT_EQ(static_cast<uint8_t>(result[1]), 0x9Fu);
    EXPECT_EQ(static_cast<uint8_t>(result[2]), 0x98u);
    EXPECT_EQ(static_cast<uint8_t>(result[3]), 0x80u);
}

TEST(TextStringEncoding, UTF16BEBOMOnlyToUtf8) {
    // Just the BOM, no content
    const char data[] = { '\xFE', '\xFF' };
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 2, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf8;
    ASSERT_EQ(TextStringEncoding_ToUtf8(buf, utf8.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(BufferToString(utf8), "");
}

// ===== UTF-8 passthrough =====

TEST(TextStringEncoding, UTF8BOMStrippedOnConvert) {
    // UTF-8 BOM + "ABC"
    const char data[] = { '\xEF', '\xBB', '\xBF', 'A', 'B', 'C' };
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 6, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf8;
    ASSERT_EQ(TextStringEncoding_ToUtf8(buf, utf8.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(BufferToString(utf8), "ABC");
}

// ===== Roundtrip: UTF-8 → TextString → UTF-8 =====

TEST(TextStringEncoding, RoundtripASCII) {
    // ASCII fits in PDFDocEncoding
    const char data[] = "Hello PDF";
    HandleGuard<BufferHandle, Buffer_Release> utf8_in;
    ASSERT_EQ(CreateBufferFromBytes(data, 9, utf8_in.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> text_string;
    ASSERT_EQ(TextStringEncoding_FromUtf8(utf8_in, text_string.out()), VANILLAPDF_ERROR_SUCCESS);

    // Should be PDFDocEncoding (no BOM)
    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(text_string, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_PDFDocEncoding);

    HandleGuard<BufferHandle, Buffer_Release> utf8_out;
    ASSERT_EQ(TextStringEncoding_ToUtf8(text_string, utf8_out.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(BufferToString(utf8_out), "Hello PDF");
}

TEST(TextStringEncoding, RoundtripNonAscii) {
    // CJK character U+4E16 (世) — not in PDFDocEncoding
    // UTF-8: E4 B8 96
    const char data[] = { '\xE4', '\xB8', '\x96' };
    HandleGuard<BufferHandle, Buffer_Release> utf8_in;
    ASSERT_EQ(CreateBufferFromBytes(data, 3, utf8_in.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> text_string;
    ASSERT_EQ(TextStringEncoding_FromUtf8(utf8_in, text_string.out()), VANILLAPDF_ERROR_SUCCESS);

    // Should be UTF-16BE with BOM
    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(TextStringEncoding_Detect(text_string, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_UTF16BE);

    HandleGuard<BufferHandle, Buffer_Release> utf8_out;
    ASSERT_EQ(TextStringEncoding_ToUtf8(text_string, utf8_out.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(BufferToString(utf8_out), std::string(data, 3));
}

// ===== PdfTextString class =====

TEST(PdfTextString, CreateFromBufferAndGetEncoding) {
    const char data[] = "Hello";
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 5, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PdfTextStringHandle, PdfTextString_Release> ts;
    ASSERT_EQ(PdfTextString_CreateFromBuffer(buf, ts.out()), VANILLAPDF_ERROR_SUCCESS);

    TextStringEncodingType encoding = TextStringEncodingType_Undefined;
    ASSERT_EQ(PdfTextString_GetEncoding(ts, &encoding), VANILLAPDF_ERROR_SUCCESS);
    EXPECT_EQ(encoding, TextStringEncodingType_PDFDocEncoding);
}

TEST(PdfTextString, GetStringRaw) {
    const char data[] = "Test";
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 4, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PdfTextStringHandle, PdfTextString_Release> ts;
    ASSERT_EQ(PdfTextString_CreateFromBuffer(buf, ts.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> raw;
    ASSERT_EQ(PdfTextString_GetStringRaw(ts, raw.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(BufferToString(raw), "Test");
}

TEST(PdfTextString, GetStringUtf8) {
    const char data[] = "Test";
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 4, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PdfTextStringHandle, PdfTextString_Release> ts;
    ASSERT_EQ(PdfTextString_CreateFromBuffer(buf, ts.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf8;
    ASSERT_EQ(PdfTextString_GetStringUtf8(ts, utf8.out()), VANILLAPDF_ERROR_SUCCESS);

    EXPECT_EQ(BufferToString(utf8), "Test");
}

TEST(PdfTextString, GetStringUtf16) {
    const char data[] = "A";
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 1, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PdfTextStringHandle, PdfTextString_Release> ts;
    ASSERT_EQ(PdfTextString_CreateFromBuffer(buf, ts.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf16;
    ASSERT_EQ(PdfTextString_GetStringUtf16(ts, utf16.out()), VANILLAPDF_ERROR_SUCCESS);

    auto result = BufferToString(utf16);
    // 'A' in UTF-16BE = 0x00 0x41
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(static_cast<uint8_t>(result[0]), 0x00u);
    EXPECT_EQ(static_cast<uint8_t>(result[1]), 0x41u);
}

TEST(PdfTextString, Conversion) {
    HandleGuard<PdfTextStringHandle, PdfTextString_Release> ts;

    const char data[] = "Test";
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 4, buf.out()), VANILLAPDF_ERROR_SUCCESS);
    ASSERT_EQ(PdfTextString_CreateFromBuffer(buf, ts.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<IUnknownHandle, IUnknown_Release> unknown;
    ASSERT_EQ(PdfTextString_ToUnknown(ts, unknown.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<PdfTextStringHandle, PdfTextString_Release> ts2;
    ASSERT_EQ(PdfTextString_FromUnknown(unknown, ts2.out()), VANILLAPDF_ERROR_SUCCESS);
}

// ===== Null parameter checks =====

TEST(TextStringEncoding, NullChecks) {
    EXPECT_EQ(TextStringEncoding_Detect(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TextStringEncoding_ToUtf8(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TextStringEncoding_ToUtf16(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(TextStringEncoding_FromUtf8(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

TEST(PdfTextString, NullChecks) {
    EXPECT_EQ(PdfTextString_CreateFromStringObject(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PdfTextString_CreateFromBuffer(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PdfTextString_GetEncoding(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PdfTextString_GetStringRaw(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PdfTextString_GetStringUtf8(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PdfTextString_GetStringUtf16(nullptr, nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
    EXPECT_EQ(PdfTextString_Release(nullptr), VANILLAPDF_ERROR_PARAMETER_VALUE);
}

// ===== UTF-16BE conversion =====

TEST(TextStringEncoding, PDFDocToUtf16) {
    const char data[] = "A";
    HandleGuard<BufferHandle, Buffer_Release> buf;
    ASSERT_EQ(CreateBufferFromBytes(data, 1, buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> utf16;
    ASSERT_EQ(TextStringEncoding_ToUtf16(buf, utf16.out()), VANILLAPDF_ERROR_SUCCESS);

    auto result = BufferToString(utf16);
    // 'A' in UTF-16BE = 0x00 0x41
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(static_cast<uint8_t>(result[0]), 0x00u);
    EXPECT_EQ(static_cast<uint8_t>(result[1]), 0x41u);
}

} // namespace text_string_encoding
