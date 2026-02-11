#include "benchmark.h"
#include "handle_guard.h"

#include <string>
#include <vector>

static std::vector<char> GenerateData(size_t size) {
    std::vector<char> data(size);
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<char>(i % 256);
    }
    return data;
}

static void BM_BufferHash(benchmark::State& state, size_t buffer_size) {
    auto data = GenerateData(buffer_size);

    HandleGuard<BufferHandle, Buffer_Release> buffer;
    auto result = Buffer_CreateFromData(data.data(), static_cast<size_type>(data.size()), buffer.out());
    if (result != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("Buffer_CreateFromData failed");
        return;
    }

    for (auto _ : state) {
        size_type hash = 0;
        result = Buffer_Hash(buffer, &hash);
        if (result != VANILLAPDF_ERROR_SUCCESS) {
            state.SkipWithError("Buffer_Hash failed");
            break;
        }
        benchmark::DoNotOptimize(hash);
    }
}

BENCHMARK_CAPTURE(BM_BufferHash, 16B, 16);
BENCHMARK_CAPTURE(BM_BufferHash, 256B, 256);
BENCHMARK_CAPTURE(BM_BufferHash, 4KB, 4096);
BENCHMARK_CAPTURE(BM_BufferHash, 64KB, 65536);
BENCHMARK_CAPTURE(BM_BufferHash, 1MB, 1048576);
