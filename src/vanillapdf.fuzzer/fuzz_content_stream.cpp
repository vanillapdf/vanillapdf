#include "fuzzer.h"
#include "handle_guard.h"

#include <cstddef>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static bool initialized = (fuzzer_init(), true);
    (void)initialized;

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
    if (ContentParser_Create(nullptr, stream, parser.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<ContentInstructionCollectionHandle, ContentInstructionCollection_Release> instructions;
    ContentParser_ReadInstructionCollection(parser, instructions.out());

    return 0;
}
