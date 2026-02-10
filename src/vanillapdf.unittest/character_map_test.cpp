#include "unittest.h"
#include "handle_guard.h"

#include <string>

// Minimal PDF with a Type1 font containing a ToUnicode CMap stream.
// Object 4 is the font dictionary, object 5 is the ToUnicode CMap stream.
// The CMap maps codespace <0020>-<007E> with bfrange <0020> <007E> <0020>.
static const std::string CMAP_PDF = []() {
    std::string cmap_content =
        "/CIDInit /ProcSet findresource begin\n"
        "12 dict begin\n"
        "begincmap\n"
        "/CMapType 2 def\n"
        "/CMapName /TestCMap def\n"
        "/CIDSystemInfo << /Registry (Adobe) /Ordering (Identity) /Supplement 0 >> def\n"
        "1 begincodespacerange\n"
        "<0020> <007E>\n"
        "endcodespacerange\n"
        "1 beginbfrange\n"
        "<0020> <007E> <0020>\n"
        "endbfrange\n"
        "endcmap\n"
        "CMapName currentdict /CMap defineresource pop\n"
        "end\n"
        "end\n";

    auto len = std::to_string(cmap_content.size());

    std::string pdf;
    pdf += "%PDF-1.7\n";

    std::string obj1 = "1 0 obj\n<</Type /Catalog /Pages 2 0 R>>\nendobj\n";
    auto off1 = std::to_string(pdf.size());
    pdf += obj1;

    std::string obj2 = "2 0 obj\n<</Type /Pages /Kids [3 0 R] /Count 1>>\nendobj\n";
    auto off2 = std::to_string(pdf.size());
    pdf += obj2;

    std::string obj3 = "3 0 obj\n<</Type /Page /Parent 2 0 R /Resources <</Font <</F1 4 0 R>>>>>>\nendobj\n";
    auto off3 = std::to_string(pdf.size());
    pdf += obj3;

    std::string obj4 = "4 0 obj\n<</Type /Font /Subtype /Type1 /BaseFont /TestFont /ToUnicode 5 0 R>>\nendobj\n";
    auto off4 = std::to_string(pdf.size());
    pdf += obj4;

    std::string obj5 = "5 0 obj\n<</Length " + len + ">>\nstream\n" + cmap_content + "endstream\nendobj\n";
    auto off5 = std::to_string(pdf.size());
    pdf += obj5;

    auto xref_off = std::to_string(pdf.size());
    pdf += "xref\n0 6\n";
    pdf += "0000000000 65535 f \n";

    auto pad = [](const std::string& s) { return std::string(10 - s.size(), '0') + s; };
    pdf += pad(off1) + " 00000 n \n";
    pdf += pad(off2) + " 00000 n \n";
    pdf += pad(off3) + " 00000 n \n";
    pdf += pad(off4) + " 00000 n \n";
    pdf += pad(off5) + " 00000 n \n";

    pdf += "trailer\n<</Size 6 /Root 1 0 R>>\n";
    pdf += "startxref\n" + xref_off + "\n%%EOF\n";

    return pdf;
}();

class UnicodeCharacterMapTest : public ::testing::Test {
protected:
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;
    HandleGuard<FontHandle, Font_Release> font;
    HandleGuard<UnicodeCharacterMapHandle, UnicodeCharacterMap_Release> unicode_map;

    void SetUp() override {
        ASSERT_EQ(InputOutputStream_CreateFromMemory(io_stream.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(InputOutputStream_WriteString(io_stream, CMAP_PDF.c_str()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_OpenStream(io_stream, "cmap_test", file.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(File_Initialize(file), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<ObjectHandle, Object_Release> font_obj;
        ASSERT_EQ(File_GetIndirectObject(file, 4, 0, font_obj.out()), VANILLAPDF_ERROR_SUCCESS);

        HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> font_dict;
        ASSERT_EQ(DictionaryObject_FromObject(font_obj, font_dict.out()), VANILLAPDF_ERROR_SUCCESS);

        ASSERT_EQ(Font_CreateFromObject(font_dict, font.out()), VANILLAPDF_ERROR_SUCCESS);
        ASSERT_EQ(Font_GetUnicodeMap(font, unicode_map.out()), VANILLAPDF_ERROR_SUCCESS);
    }
};

TEST_F(UnicodeCharacterMapTest, GetMappedValueIdentityMapping) {
    // Identity mapping: <0041> ('A') -> <0041>
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0041", key_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(key_hex, key_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> result;
    ASSERT_EQ(UnicodeCharacterMap_GetMappedValue(unicode_map, key_buf, result.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type result_data = nullptr;
    size_type result_len = 0;
    ASSERT_EQ(Buffer_GetData(result, &result_data, &result_len), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(result_len, 2u);
    EXPECT_EQ(static_cast<uint8_t>(result_data[0]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(result_data[1]), 0x41);
}

TEST_F(UnicodeCharacterMapTest, GetMappedValueBoundaryLow) {
    // Low boundary: <0020> -> <0020>
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("0020", key_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(key_hex, key_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> result;
    ASSERT_EQ(UnicodeCharacterMap_GetMappedValue(unicode_map, key_buf, result.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type result_data = nullptr;
    size_type result_len = 0;
    ASSERT_EQ(Buffer_GetData(result, &result_data, &result_len), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(result_len, 2u);
    EXPECT_EQ(static_cast<uint8_t>(result_data[0]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(result_data[1]), 0x20);
}

TEST_F(UnicodeCharacterMapTest, GetMappedValueBoundaryHigh) {
    // High boundary: <007E> -> <007E>
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("007E", key_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(key_hex, key_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> result;
    ASSERT_EQ(UnicodeCharacterMap_GetMappedValue(unicode_map, key_buf, result.out()), VANILLAPDF_ERROR_SUCCESS);

    string_type result_data = nullptr;
    size_type result_len = 0;
    ASSERT_EQ(Buffer_GetData(result, &result_data, &result_len), VANILLAPDF_ERROR_SUCCESS);

    ASSERT_EQ(result_len, 2u);
    EXPECT_EQ(static_cast<uint8_t>(result_data[0]), 0x00);
    EXPECT_EQ(static_cast<uint8_t>(result_data[1]), 0x7E);
}

TEST_F(UnicodeCharacterMapTest, GetMappedValueOutOfCodespace) {
    // Out of codespace: <00FF> should fail
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    ASSERT_EQ(HexadecimalStringObject_CreateFromEncodedString("00FF", key_hex.out()), VANILLAPDF_ERROR_SUCCESS);
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    ASSERT_EQ(HexadecimalStringObject_GetValue(key_hex, key_buf.out()), VANILLAPDF_ERROR_SUCCESS);

    HandleGuard<BufferHandle, Buffer_Release> result;
    EXPECT_NE(UnicodeCharacterMap_GetMappedValue(unicode_map, key_buf, result.out()), VANILLAPDF_ERROR_SUCCESS);
}
