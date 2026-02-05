#include "benchmark.h"
#include "handle_guard.h"

static const char SAMPLE_CONTENT_STREAM[] = R"(q
0.000008871 0 595.32 841.92 re
W* n
BT
/F1 9 Tf
1 0 0 1 362.83 799.8 Tm
0 g
0 G
[( )] TJ
ET
Q
q
0.000008871 0 595.32 841.92 re
W* n
BT
/F1 9 Tf
1 0 0 1 458.35 799.8 Tm
0 g
0 G
[( )-2( )-2( )-2( )-2( )-2( )10( )] TJ
ET)";

static void BM_Contents_ParseContentStream(benchmark::State& state) {
    HandleGuard<FileHandle, File_Release> test_file;
    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> file_stream;

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> content_io_stream;
    HandleGuard<InputStreamHandle, InputStream_Release> content_input_stream;

    InputOutputStream_CreateFromMemory(file_stream.out());
    File_OpenStream(file_stream, "temp", test_file.out());

    InputOutputStream_CreateFromMemory(content_io_stream.out());
    InputOutputStream_WriteString(content_io_stream, SAMPLE_CONTENT_STREAM);
    InputOutputStream_ToInputStream(content_io_stream, content_input_stream.out());

    for (auto _ : state) {
        HandleGuard<ContentParserHandle, ContentParser_Release> content_parser;
        HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release> content_instruction_collection;

        InputStream_SetInputPosition(content_input_stream, 0);

        ContentParser_Create(test_file, content_input_stream, content_parser.out());
        ContentParser_ReadInstructionCollection(content_parser, content_instruction_collection.out());
    }
}

BENCHMARK(BM_Contents_ParseContentStream);

static const char MINIMALIST_DOCUMENT[] = R"(%PDF-1.7
1 0 obj
<</Pages 2 0 R /Type /Catalog>>
endobj
2 0 obj
<</Count 0 /Type /Pages>>
endobj
3 0 obj
<</CreationDate (D:20170831161944Z) /Producer (I am the producer)>>
endobj

xref
0 4
0000000000 65535 f
0000000009 00000 n
0000000056 00000 n
0000000097 00000 n
trailer
<</Info 3 0 R /Root 1 0 R /Size 4>>
startxref
181
%%EOF
)";

static void BM_FileSaveParse(benchmark::State& state) {
    for (auto _ : state) {
        HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> io_stream;
        HandleGuard<FileHandle, File_Release> test_file;

        InputOutputStream_CreateFromMemory(io_stream.out());
        InputOutputStream_WriteString(io_stream, MINIMALIST_DOCUMENT);

        File_OpenStream(io_stream, "temp", test_file.out());
        File_Initialize(test_file);
    }
}

BENCHMARK(BM_FileSaveParse);

int main(int argc, char** argv) {

    ::benchmark::Initialize(&argc, argv);

    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    }

    // Disable logging for benchmarking
    auto severity_result = Logging_SetSeverity(LoggingSeverity_Off);
    if (severity_result != VANILLAPDF_ERROR_SUCCESS) {
        return 255;
    }

    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();

    return 0;
}
