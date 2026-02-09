#include "benchmark.h"
#include "handle_guard.h"

#include <string>

// Minimal PDF with a Type1 font containing a ToUnicode CMap stream.
// Object 4 is the font dictionary, object 5 is the ToUnicode CMap stream.
// The CMap maps codespace <0020>-<007E> with bfrange <0020> <007E> <0020>.
static const std::string CMAP_PDF = []() {
    // Build the CMap stream content
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

    // obj 1: Catalog
    std::string obj1 = "1 0 obj\n<</Type /Catalog /Pages 2 0 R>>\nendobj\n";
    auto off1 = std::to_string(pdf.size());
    pdf += obj1;

    // obj 2: Pages
    std::string obj2 = "2 0 obj\n<</Type /Pages /Kids [3 0 R] /Count 1>>\nendobj\n";
    auto off2 = std::to_string(pdf.size());
    pdf += obj2;

    // obj 3: Page with font resource
    std::string obj3 = "3 0 obj\n<</Type /Page /Parent 2 0 R /Resources <</Font <</F1 4 0 R>>>>>>\nendobj\n";
    auto off3 = std::to_string(pdf.size());
    pdf += obj3;

    // obj 4: Font dictionary
    std::string obj4 = "4 0 obj\n<</Type /Font /Subtype /Type1 /BaseFont /TestFont /ToUnicode 5 0 R>>\nendobj\n";
    auto off4 = std::to_string(pdf.size());
    pdf += obj4;

    // obj 5: ToUnicode CMap stream
    std::string obj5 = "5 0 obj\n<</Length " + len + ">>\nstream\n" + cmap_content + "endstream\nendobj\n";
    auto off5 = std::to_string(pdf.size());
    pdf += obj5;

    // xref
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

// Benchmark UnicodeCharacterMap_GetMappedValue end-to-end:
// loads a PDF with a ToUnicode CMap, obtains the unicode map, and queries it.
static void BM_UnicodeCharacterMap_GetMappedValue(benchmark::State& state) {
    // Load the PDF
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
    HandleGuard<FileHandle, File_Release> file;

    InputOutputStream_CreateFromMemory(io_stream.out());
    InputOutputStream_WriteString(io_stream, CMAP_PDF.c_str());
    File_OpenStream(io_stream, "cmap_bench", file.out());
    File_Initialize(file);

    // Get font dictionary (object 4)
    HandleGuard<ObjectHandle, Object_Release> font_obj;
    File_GetIndirectObject(file, 4, 0, font_obj.out());

    HandleGuard<DictionaryObjectHandle, DictionaryObject_Release> font_dict;
    DictionaryObject_FromObject(font_obj, font_dict.out());

    // Create font and get unicode map
    HandleGuard<FontHandle, Font_Release> font;
    Font_CreateFromObject(font_dict, font.out());

    HandleGuard<UnicodeCharacterMapHandle, UnicodeCharacterMap_Release> unicode_map;
    Font_GetUnicodeMap(font, unicode_map.out());

    // Create key buffer <0041> ('A')
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    HexadecimalStringObject_CreateFromEncodedString("0041", key_hex.out());
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    HexadecimalStringObject_GetValue(key_hex, key_buf.out());

    for (auto _ : state) {
        HandleGuard<BufferHandle, Buffer_Release> result;
        UnicodeCharacterMap_GetMappedValue(unicode_map, key_buf, result.out());
        benchmark::DoNotOptimize(result.get());
    }
}

BENCHMARK(BM_UnicodeCharacterMap_GetMappedValue);

// Benchmark BaseFontRange_Contains: checks if a key is within a range
// using ValueEqualLessThan byte-by-byte comparison
static void BM_BaseFontRange_Contains(benchmark::State& state) {
    // Setup: range <0020> to <007E> (printable ASCII)
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    BaseFontRange_Create(range.out());

    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> low;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> high;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> dest;

    HexadecimalStringObject_CreateFromEncodedString("0020", low.out());
    HexadecimalStringObject_CreateFromEncodedString("007E", high.out());
    HexadecimalStringObject_CreateFromEncodedString("0020", dest.out());

    BaseFontRange_SetRangeLow(range, low);
    BaseFontRange_SetRangeHigh(range, high);

    HandleGuard<StringObjectHandle, StringObject_Release> dest_str;
    HandleGuard<ObjectHandle, Object_Release> dest_obj;
    HexadecimalStringObject_ToStringObject(dest, dest_str.out());
    StringObject_ToObject(dest_str, dest_obj.out());
    BaseFontRange_SetDestination(range, dest_obj);

    // Create key buffer <0041> ('A', middle of range)
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    HexadecimalStringObject_CreateFromEncodedString("0041", key_hex.out());
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    HexadecimalStringObject_GetValue(key_hex, key_buf.out());

    for (auto _ : state) {
        boolean_type result = VANILLAPDF_RV_FALSE;
        BaseFontRange_Contains(range, key_buf, &result);
        benchmark::DoNotOptimize(result);
    }
}

BENCHMARK(BM_BaseFontRange_Contains);

// Benchmark BaseFontRange_GetMappedValue: computes dest + (key - low)
// using custom Difference (bit-by-bit subtraction) and Increment (byte-by-byte addition)
static void BM_BaseFontRange_GetMappedValue(benchmark::State& state) {
    // Setup: range <0020> to <007E> with destination <0020>
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    BaseFontRange_Create(range.out());

    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> low;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> high;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> dest;

    HexadecimalStringObject_CreateFromEncodedString("0020", low.out());
    HexadecimalStringObject_CreateFromEncodedString("007E", high.out());
    HexadecimalStringObject_CreateFromEncodedString("0020", dest.out());

    BaseFontRange_SetRangeLow(range, low);
    BaseFontRange_SetRangeHigh(range, high);

    HandleGuard<StringObjectHandle, StringObject_Release> dest_str;
    HandleGuard<ObjectHandle, Object_Release> dest_obj;
    HexadecimalStringObject_ToStringObject(dest, dest_str.out());
    StringObject_ToObject(dest_str, dest_obj.out());
    BaseFontRange_SetDestination(range, dest_obj);

    // Create key buffer <0041> ('A', middle of range)
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    HexadecimalStringObject_CreateFromEncodedString("0041", key_hex.out());
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    HexadecimalStringObject_GetValue(key_hex, key_buf.out());

    for (auto _ : state) {
        HandleGuard<BufferHandle, Buffer_Release> result;
        BaseFontRange_GetMappedValue(range, key_buf, result.out());
        benchmark::DoNotOptimize(result.get());
    }
}

BENCHMARK(BM_BaseFontRange_GetMappedValue);

// Benchmark with 4-byte CMap codes (CJK font scenario)
static void BM_BaseFontRange_GetMappedValue_4byte(benchmark::State& state) {
    // Setup: range <00004E00> to <00009FFF> (CJK Unified Ideographs block)
    HandleGuard<BaseFontRangeHandle, BaseFontRange_Release> range;
    BaseFontRange_Create(range.out());

    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> low;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> high;
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> dest;

    HexadecimalStringObject_CreateFromEncodedString("00004E00", low.out());
    HexadecimalStringObject_CreateFromEncodedString("00009FFF", high.out());
    HexadecimalStringObject_CreateFromEncodedString("00004E00", dest.out());

    BaseFontRange_SetRangeLow(range, low);
    BaseFontRange_SetRangeHigh(range, high);

    HandleGuard<StringObjectHandle, StringObject_Release> dest_str;
    HandleGuard<ObjectHandle, Object_Release> dest_obj;
    HexadecimalStringObject_ToStringObject(dest, dest_str.out());
    StringObject_ToObject(dest_str, dest_obj.out());
    BaseFontRange_SetDestination(range, dest_obj);

    // Key in the middle of CJK range
    HandleGuard<HexadecimalStringObjectHandle, HexadecimalStringObject_Release> key_hex;
    HexadecimalStringObject_CreateFromEncodedString("00006F22", key_hex.out());
    HandleGuard<BufferHandle, Buffer_Release> key_buf;
    HexadecimalStringObject_GetValue(key_hex, key_buf.out());

    for (auto _ : state) {
        HandleGuard<BufferHandle, Buffer_Release> result;
        BaseFontRange_GetMappedValue(range, key_buf, result.out());
        benchmark::DoNotOptimize(result.get());
    }
}

BENCHMARK(BM_BaseFontRange_GetMappedValue_4byte);
