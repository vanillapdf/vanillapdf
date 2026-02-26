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

    HandleGuard<InputOutputStreamHandle, InputOutputStream_Release> stream;
    if (InputOutputStream_CreateFromMemory(stream.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    if (InputOutputStream_WriteBuffer(stream, buffer) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    if (InputOutputStream_SetInputPosition(stream, 0) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<FileHandle, File_Release> file;
    if (File_OpenStream(stream, "fuzz_input.pdf", file.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    File_Initialize(file);

    return 0;
}
