// Benchmark for FlateDecodeFilter — measures zlib compression and
// decompression throughput through the C API, together with the compression
// ratio, which changes with the zlib provider as much as the speed does.
//
// Two kinds of input bracket what PDF documents carry:
//   ContentStream - page content operators, which compress well
//   Image         - noisy RGB pixels, which compress poorly
//
// Encode throughput counts input bytes, decode throughput counts decoded
// bytes, and ratio is the encoded size divided by the input size.
//
// Environment: Windows x64, MSVC 18, Release build, shared library
//   CPU: 16 x 3792 MHz, L1 32 KiB (x8), L2 256 KiB (x8), L3 16384 KiB (x1)
//   Median of 5 repetitions (--benchmark_repetitions=5 --benchmark_min_time=0.5s)
//
// Baseline (zlib 1.3.2, vcpkg x64-windows, Z_DEFAULT_COMPRESSION):
//   Benchmark                                    Time   Throughput    Ratio
//   ----------------------------------------------------------------------
//   BM_FlateEncode/ContentStream/4KB         53219 ns   73.6 MiB/s    0.238
//   BM_FlateEncode/ContentStream/256KB     8599740 ns   29.3 MiB/s    0.163
//   BM_FlateEncode/ContentStream/4MB     146206300 ns   27.2 MiB/s    0.160
//   BM_FlateEncode/Image/4KB                 51061 ns   77.8 MiB/s    1.003
//   BM_FlateEncode/Image/256KB             6491337 ns   38.1 MiB/s    0.976
//   BM_FlateEncode/Image/4MB             112087667 ns   35.7 MiB/s    0.976
//   BM_FlateDecode/ContentStream/4KB          7929 ns  487.0 MiB/s
//   BM_FlateDecode/ContentStream/256KB      512936 ns  484.3 MiB/s
//   BM_FlateDecode/ContentStream/4MB      11377564 ns  356.2 MiB/s
//   BM_FlateDecode/Image/4KB                  3626 ns    1.06 GiB/s
//   BM_FlateDecode/Image/256KB              941569 ns  262.6 MiB/s
//   BM_FlateDecode/Image/4MB              19194435 ns  210.5 MiB/s

#include "benchmark.h"
#include "handle_guard.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

// Deterministic generator, so every run and every provider compresses the
// same bytes
class PseudoRandom {
public:
    explicit PseudoRandom(uint32_t seed) : m_state(seed) {
    }

    uint32_t Next() {
        m_state = m_state * 1664525u + 1013904223u;
        return m_state >> 8;
    }

private:
    uint32_t m_state = 0;
};

static std::vector<char> GenerateContentStream(size_t size) {
    static const char* const WORDS[] = {
        "Lorem", "ipsum", "dolor", "sit", "amet", "consectetur",
        "adipiscing", "elit", "sed", "do", "eiusmod", "tempor"
    };

    const size_t word_count = sizeof(WORDS) / sizeof(WORDS[0]);

    PseudoRandom random(42);
    std::string content;
    content.reserve(size + 256);

    while (content.size() < size) {
        uint32_t font_size = 8 + random.Next() % 10;
        uint32_t x = random.Next() % 500;
        uint32_t y = random.Next() % 700;
        uint32_t first_word = random.Next() % word_count;
        uint32_t second_word = random.Next() % word_count;

        char line[160];
        int line_length = std::snprintf(line, sizeof(line),
            "BT /F1 %u Tf %u.%u %u.%u Td (%s %s) Tj ET\n",
            font_size, x, x % 10, y, y % 10,
            WORDS[first_word], WORDS[second_word]);

        content.append(line, static_cast<size_t>(line_length));
    }

    content.resize(size);
    return std::vector<char>(content.begin(), content.end());
}

static std::vector<char> GenerateImage(size_t size) {
    PseudoRandom random(7);
    std::vector<char> pixels(size);

    for (size_t i = 0; i < size; ++i) {
        // Smooth gradient with a few bits of noise, as in a photograph. The
        // noise comes from the high bits: the low bits of the generator
        // repeat within the deflate window and would compress away.
        uint32_t gradient = static_cast<uint32_t>((i / 3) % 256);
        uint32_t noise = random.Next() >> 20;
        pixels[i] = static_cast<char>((gradient + noise) & 0xFF);
    }

    return pixels;
}

static bool CreateBuffer(const std::vector<char>& data, BufferHandle** result) {
    auto data_size = static_cast<size_type>(data.size());
    return Buffer_CreateFromData(data.data(), data_size, result) == VANILLAPDF_ERROR_SUCCESS;
}

static void BM_FlateEncode(benchmark::State& state, std::vector<char> (*generate)(size_t), size_t data_size) {
    auto data = generate(data_size);

    HandleGuard<BufferHandle, Buffer_Release> input_buffer;
    if (!CreateBuffer(data, input_buffer.out())) {
        state.SkipWithError("Buffer_CreateFromData failed");
        return;
    }

    HandleGuard<FlateDecodeFilterHandle, FlateDecodeFilter_Release> filter;
    if (FlateDecodeFilter_Create(filter.out()) != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("FlateDecodeFilter_Create failed");
        return;
    }

    size_type encoded_size = 0;
    for (auto _ : state) {
        HandleGuard<BufferHandle, Buffer_Release> encoded;
        auto result = FlateDecodeFilter_Encode(filter, input_buffer, encoded.out());
        if (result != VANILLAPDF_ERROR_SUCCESS) {
            state.SkipWithError("FlateDecodeFilter_Encode failed");
            break;
        }

        string_type encoded_data = nullptr;
        Buffer_GetData(encoded, &encoded_data, &encoded_size);
    }

    auto input_size = static_cast<int64_t>(data.size());
    state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * input_size);
    state.counters["ratio"] = static_cast<double>(encoded_size) / static_cast<double>(data.size());
}

static void BM_FlateDecode(benchmark::State& state, std::vector<char> (*generate)(size_t), size_t data_size) {
    auto data = generate(data_size);

    HandleGuard<BufferHandle, Buffer_Release> input_buffer;
    if (!CreateBuffer(data, input_buffer.out())) {
        state.SkipWithError("Buffer_CreateFromData failed");
        return;
    }

    HandleGuard<FlateDecodeFilterHandle, FlateDecodeFilter_Release> filter;
    if (FlateDecodeFilter_Create(filter.out()) != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("FlateDecodeFilter_Create failed");
        return;
    }

    HandleGuard<BufferHandle, Buffer_Release> encoded;
    if (FlateDecodeFilter_Encode(filter, input_buffer, encoded.out()) != VANILLAPDF_ERROR_SUCCESS) {
        state.SkipWithError("FlateDecodeFilter_Encode failed");
        return;
    }

    for (auto _ : state) {
        HandleGuard<BufferHandle, Buffer_Release> decoded;
        auto result = FlateDecodeFilter_Decode(filter, encoded, decoded.out());
        if (result != VANILLAPDF_ERROR_SUCCESS) {
            state.SkipWithError("FlateDecodeFilter_Decode failed");
            break;
        }
    }

    // Throughput is counted in decoded bytes, the size the caller asked for
    auto output_size = static_cast<int64_t>(data.size());
    state.SetBytesProcessed(static_cast<int64_t>(state.iterations()) * output_size);
}

BENCHMARK_CAPTURE(BM_FlateEncode, ContentStream/4KB, GenerateContentStream, 4096);
BENCHMARK_CAPTURE(BM_FlateEncode, ContentStream/256KB, GenerateContentStream, 262144);
BENCHMARK_CAPTURE(BM_FlateEncode, ContentStream/4MB, GenerateContentStream, 4194304);
BENCHMARK_CAPTURE(BM_FlateEncode, Image/4KB, GenerateImage, 4096);
BENCHMARK_CAPTURE(BM_FlateEncode, Image/256KB, GenerateImage, 262144);
BENCHMARK_CAPTURE(BM_FlateEncode, Image/4MB, GenerateImage, 4194304);

BENCHMARK_CAPTURE(BM_FlateDecode, ContentStream/4KB, GenerateContentStream, 4096);
BENCHMARK_CAPTURE(BM_FlateDecode, ContentStream/256KB, GenerateContentStream, 262144);
BENCHMARK_CAPTURE(BM_FlateDecode, ContentStream/4MB, GenerateContentStream, 4194304);
BENCHMARK_CAPTURE(BM_FlateDecode, Image/4KB, GenerateImage, 4096);
BENCHMARK_CAPTURE(BM_FlateDecode, Image/256KB, GenerateImage, 262144);
BENCHMARK_CAPTURE(BM_FlateDecode, Image/4MB, GenerateImage, 4194304);
