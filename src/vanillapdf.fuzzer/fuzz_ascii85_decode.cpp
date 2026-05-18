#include "fuzzer.h"
#include "handle_guard.h"

#include <cstddef>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static bool initialized = (fuzzer_init(), true);
    (void)initialized;

    HandleGuard<BufferHandle, Buffer_Release> input;
    if (Buffer_CreateFromData(reinterpret_cast<const char*>(data),
                              static_cast<size_type>(size),
                              input.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<ASCII85DecodeFilterHandle, ASCII85DecodeFilter_Release> filter;
    if (ASCII85DecodeFilter_Create(filter.out()) != VANILLAPDF_ERROR_SUCCESS) {
        return 0;
    }

    HandleGuard<BufferHandle, Buffer_Release> output;
    ASCII85DecodeFilter_Decode(filter, input, output.out());

    return 0;
}
