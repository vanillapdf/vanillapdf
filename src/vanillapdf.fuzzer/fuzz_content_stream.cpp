#include "fuzzer.h"
#include "handle_guard.h"

#include <cstddef>
#include <cstdint>

// A content stream is parsed in the context of the file it belongs to. The parser
// stamps that file onto every object it produces and reads it back for diagnostics,
// which is why ContentParser_Create rejects a null file, and why both callers inside
// the library pass the file the stream came from.
//
// This target used to pass nullptr, so the call failed on every input and it returned
// before the parser ran. It reported 69 edges after 34 million executions, all of them
// in buffer creation.
//
// The document is not what is being fuzzed, so an empty file over a memory stream
// stands in for it. It carries no objects, which is all the parser needs to resolve
// nothing against.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static bool initialized = (fuzzer_init(), true);
    (void)initialized;

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> document_stream;
    if (InputOutputStream_CreateFromMemory(document_stream.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<FileHandle, File_Release> file;
    if (File_CreateStream(document_stream, "fuzz_content_stream.pdf", file.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<BufferHandle, Buffer_Release> buffer;
    if (Buffer_CreateFromData(reinterpret_cast<const char*>(data),
                              static_cast<size_type>(size),
                              buffer.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<InputStreamHandle, InputStream_Release> stream;
    if (InputStream_CreateFromBuffer(buffer, stream.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<ContentParserHandle, ContentParser_Release> parser;
    if (ContentParser_Create(file, stream, parser.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release> instructions;
    ContentParser_ReadInstructionCollection(parser, instructions.out());

    return 0;
}
