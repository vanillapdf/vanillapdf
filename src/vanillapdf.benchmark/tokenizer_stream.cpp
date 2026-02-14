// Tokenizer / stream reading benchmarks
//
// Measures File_OpenStream + File_Initialize throughput, which exercises the
// full tokenizer pipeline: ReadToken, ReadUnknown (numbers, names, keywords),
// ReadHexadecimalString, ReadLiteralString, ReadComment, and Readline.
//
// Environment:
//   Compiler: MSVC 18 (Visual Studio 2026), Release (/O2)
//   CPU: 16x 3792 MHz (8C/16T), L1d 32 KiB x8, L2 256 KiB x8, L3 16 MiB
//   Repetitions: 10 per benchmark
//
// Before (redundant Eof+Peek, Peek+Get loops):
//   Benchmark                                   Time             CPU   Iterations
//   BM_TokenizerParse100Objects_mean       303520 ns       302825 ns           10
//   BM_TokenizerParse100Objects_median     299657 ns       294874 ns           10
//   BM_TokenizerParse100Objects_stddev      27525 ns        29144 ns           10
//   BM_TokenizerParse100Objects_cv           9.07 %          9.62 %            10
//   BM_TokenizerParse500Objects_mean      1367969 ns      1344672 ns           10
//   BM_TokenizerParse500Objects_median    1346753 ns      1340483 ns           10
//   BM_TokenizerParse500Objects_stddev     181397 ns       192927 ns           10
//   BM_TokenizerParse500Objects_cv          13.26 %         14.35 %            10
//   BM_FileSaveParse_mean                   56855 ns        56641 ns           10
//   BM_FileSaveParse_median                 56459 ns        56501 ns           10
//   BM_FileSaveParse_stddev                  1005 ns         1348 ns           10
//   BM_FileSaveParse_cv                      1.77 %          2.38 %            10
//
// After (Get-only loops, single EOF check, Peek+Ignore instead of Peek+Get):
//   Benchmark                                   Time             CPU   Iterations
//   BM_TokenizerParse100Objects_mean       254122 ns       252988 ns           10
//   BM_TokenizerParse100Objects_median     250435 ns       251105 ns           10
//   BM_TokenizerParse100Objects_stddev       9342 ns        11480 ns           10
//   BM_TokenizerParse100Objects_cv           3.68 %          4.54 %            10
//   BM_TokenizerParse500Objects_mean      1066293 ns      1057129 ns           10
//   BM_TokenizerParse500Objects_median    1063288 ns      1062012 ns           10
//   BM_TokenizerParse500Objects_stddev      33731 ns        28308 ns           10
//   BM_TokenizerParse500Objects_cv           3.16 %          2.68 %            10
//   BM_FileSaveParse_mean                   53546 ns        53571 ns           10
//   BM_FileSaveParse_median                 53235 ns        53013 ns           10
//   BM_FileSaveParse_stddev                  1035 ns          975 ns           10
//   BM_FileSaveParse_cv                      1.93 %          1.82 %            10
//
// Summary (median wall time):
//   BM_TokenizerParse100Objects:  299657 -> 250435 ns  (-16.4%)
//   BM_TokenizerParse500Objects: 1346753 -> 1063288 ns (-21.0%)
//   BM_FileSaveParse:              56459 ->   53235 ns  (-5.7%)

#include "benchmark.h"
#include "handle_guard.h"

#include <string>
#include <sstream>

// Generate a synthetic PDF with `object_count` indirect objects.
// Each object is a dictionary with several name/number/string entries,
// designed to exercise the tokenizer's hot loops:
//   - Name reading (/Key tokens)
//   - Integer and real number reading
//   - Literal string reading
//   - Hexadecimal string reading
//   - Comment skipping
//   - Whitespace skipping
//   - Dictionary delimiters (<< >>)
static std::string GenerateSyntheticPdf(int object_count) {
    std::ostringstream pdf;
    pdf << "%PDF-1.7\n";
    pdf << "% This is a comment to exercise comment skipping\n";

    // Object 1: Catalog
    pdf << "1 0 obj\n";
    pdf << "<</Type /Catalog /Pages 2 0 R>>\n";
    pdf << "endobj\n";

    // Object 2: Pages
    pdf << "2 0 obj\n";
    pdf << "<</Type /Pages /Count 0>>\n";
    pdf << "endobj\n";

    // Object 3: Info dictionary
    pdf << "3 0 obj\n";
    pdf << "<</Producer (Benchmark Generator) /CreationDate (D:20250101120000Z)>>\n";
    pdf << "endobj\n";

    // Objects 4..N: dictionaries with mixed content
    for (int i = 4; i <= object_count + 3; i += 1) {
        pdf << "% comment before object " << i << "\n";
        pdf << i << " 0 obj\n";
        pdf << "<<\n";
        pdf << "  /Type /BenchmarkObj\n";
        pdf << "  /Index " << i << "\n";
        pdf << "  /RealValue " << (i * 3.14159) << "\n";
        pdf << "  /NegativeInt -" << (i * 17) << "\n";
        pdf << "  /Name /SomeLongNameValue" << i << "\n";
        pdf << "  /LiteralStr (This is literal string number " << i << " with \\(escapes\\))\n";
        pdf << "  /HexStr <48656C6C6F" << std::hex << i << std::dec << ">\n";
        pdf << "  /Boolean " << ((i % 2 == 0) ? "true" : "false") << "\n";
        pdf << "  /NullEntry null\n";
        pdf << "  /ArrayEntry [1 2 3 " << i << " 5.5 /Name]\n";
        pdf << "  /Ref " << ((i % 3) + 1) << " 0 R\n";
        pdf << ">>\n";
        pdf << "endobj\n";
    }

    int total_objects = object_count + 3;

    // Cross-reference table
    // Compute byte offsets by regenerating (simplified: use placeholder offsets)
    // For benchmarking purposes, we need valid xref offsets.
    // Re-generate with tracked offsets.
    std::ostringstream pdf2;
    pdf2 << "%PDF-1.7\n";
    pdf2 << "% This is a comment to exercise comment skipping\n";

    std::vector<std::streampos> offsets(total_objects + 1, 0);

    offsets[1] = pdf2.tellp();
    pdf2 << "1 0 obj\n";
    pdf2 << "<</Type /Catalog /Pages 2 0 R>>\n";
    pdf2 << "endobj\n";

    offsets[2] = pdf2.tellp();
    pdf2 << "2 0 obj\n";
    pdf2 << "<</Type /Pages /Count 0>>\n";
    pdf2 << "endobj\n";

    offsets[3] = pdf2.tellp();
    pdf2 << "3 0 obj\n";
    pdf2 << "<</Producer (Benchmark Generator) /CreationDate (D:20250101120000Z)>>\n";
    pdf2 << "endobj\n";

    for (int i = 4; i <= total_objects; i += 1) {
        offsets[i] = pdf2.tellp();
        pdf2 << "% comment before object " << i << "\n";
        pdf2 << i << " 0 obj\n";
        pdf2 << "<<\n";
        pdf2 << "  /Type /BenchmarkObj\n";
        pdf2 << "  /Index " << i << "\n";
        pdf2 << "  /RealValue " << (i * 3.14159) << "\n";
        pdf2 << "  /NegativeInt -" << (i * 17) << "\n";
        pdf2 << "  /Name /SomeLongNameValue" << i << "\n";
        pdf2 << "  /LiteralStr (This is literal string number " << i << " with \\(escapes\\))\n";
        pdf2 << "  /HexStr <48656C6C6F" << std::hex << i << std::dec << ">\n";
        pdf2 << "  /Boolean " << ((i % 2 == 0) ? "true" : "false") << "\n";
        pdf2 << "  /NullEntry null\n";
        pdf2 << "  /ArrayEntry [1 2 3 " << i << " 5.5 /Name]\n";
        pdf2 << "  /Ref " << ((i % 3) + 1) << " 0 R\n";
        pdf2 << ">>\n";
        pdf2 << "endobj\n";
    }

    auto xref_offset = pdf2.tellp();
    pdf2 << "xref\n";
    pdf2 << "0 " << (total_objects + 1) << "\n";
    pdf2 << "0000000000 65535 f \n";
    for (int i = 1; i <= total_objects; i += 1) {
        char buf[21];
        snprintf(buf, sizeof(buf), "%010lld 00000 n \n", static_cast<long long>(offsets[i]));
        pdf2 << buf;
    }

    pdf2 << "trailer\n";
    pdf2 << "<</Info 3 0 R /Root 1 0 R /Size " << (total_objects + 1) << ">>\n";
    pdf2 << "startxref\n";
    pdf2 << xref_offset << "\n";
    pdf2 << "%%EOF\n";

    return pdf2.str();
}

// Benchmark: parse a synthetic PDF with 100 objects (~1200 tokens)
static void BM_TokenizerParse100Objects(benchmark::State& state) {
    auto pdf_content = GenerateSyntheticPdf(100);

    for (auto _ : state) {
        HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
        HandleGuard<FileHandle, File_Release> test_file;

        InputOutputStream_CreateFromMemory(io_stream.out());
        InputOutputStream_WriteString(io_stream, pdf_content.c_str());

        File_OpenStream(io_stream, "bench100", test_file.out());
        File_Initialize(test_file);
    }
}

BENCHMARK(BM_TokenizerParse100Objects);

// Benchmark: parse a synthetic PDF with 500 objects (~6000 tokens)
static void BM_TokenizerParse500Objects(benchmark::State& state) {
    auto pdf_content = GenerateSyntheticPdf(500);

    for (auto _ : state) {
        HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
        HandleGuard<FileHandle, File_Release> test_file;

        InputOutputStream_CreateFromMemory(io_stream.out());
        InputOutputStream_WriteString(io_stream, pdf_content.c_str());

        File_OpenStream(io_stream, "bench500", test_file.out());
        File_Initialize(test_file);
    }
}

BENCHMARK(BM_TokenizerParse500Objects);
