// Benchmark for Buffer::Hash() — measures raw hashing throughput.
//
// Environment: Windows x64, MSVC 17, Release build
//   CPU: 16 x 3792 MHz, L1 32 KiB (x8), L2 256 KiB (x8), L3 16384 KiB (x1)
//
// Baseline (XOR per-byte with std::hash<char>):
//   Benchmark                   Time             CPU   Iterations
//   -------------------------------------------------------------
//   BM_BufferHash/16B        8.39 ns         8.37 ns     74666667
//   BM_BufferHash/256B        126 ns          127 ns      6400000
//   BM_BufferHash/4KB        1913 ns         1925 ns       373333
//   BM_BufferHash/64KB      29677 ns        29820 ns        23579
//   BM_BufferHash/1MB      478257 ns       475779 ns         1445
//
// FNV-1a (XOR + multiply per byte):
//   Benchmark                   Time             CPU   Iterations
//   -------------------------------------------------------------
//   BM_BufferHash/16B        12.7 ns         12.9 ns     44800000
//   BM_BufferHash/256B        220 ns          215 ns      3200000
//   BM_BufferHash/4KB        3546 ns         3589 ns       213333
//   BM_BufferHash/64KB      56607 ns        57199 ns        11200
//   BM_BufferHash/1MB      906200 ns       899431 ns          747
//
// FNV-1a is ~1.9x slower due to the per-byte multiply, but provides
// significantly better hash quality (avalanche, positional sensitivity).
// The old XOR-only hash had trivial collisions (e.g. "ab" == "ba").

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
