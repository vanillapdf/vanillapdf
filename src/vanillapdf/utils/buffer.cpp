#include "precompiled.h"

#include "utils/buffer.h"
#include "utils/streams/memory_buffer_input_stream.h"

#include <cstring>

#include <fmt/core.h>

namespace vanillapdf {

/**
 * @brief Compute hash using FNV-1a algorithm.
 *
 * @see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
 *
 * The previous implementation used XOR accumulation of per-byte std::hash<char>
 * values, which lacks positional encoding — buffers containing the same bytes
 * in different order (e.g. "ab" vs "ba") produced identical hashes.
 *
 * FNV-1a multiplies after each XOR, making the hash dependent on byte order.
 * This provides better avalanche properties and reduces collision rates in
 * hash-based containers at the cost of ~1.9x slower raw throughput, which is
 * negligible for the small buffers typical in PDF name and string objects.
 */
size_t Buffer::Hash() const {
    size_t hash = constant::FNV1A_OFFSET_BASIS;
    for (auto byte : m_data) {
        hash ^= static_cast<unsigned char>(byte);
        hash *= constant::FNV1A_PRIME;
    }
    return hash;
}

std::string Buffer::ToHexString(void) const {
    fmt::memory_buffer buf;
    for (const auto& v : m_data) {
        fmt::format_to(std::back_inserter(buf), "{:02x}", static_cast<unsigned char>(v));
    }
    return fmt::to_string(buf);
}

// Measured via BM_StringGetValue_Literal (LiteralStringObject_GetValue → GetRawValueDecoded → ToInputStream)
//
// stringstream (InputStream)
// ------------------------------------------------------------------------------------------
// Benchmark                                                Time             CPU   Iterations
// ------------------------------------------------------------------------------------------
// BM_StringGetValue_Literal / string_empty              1120 ns         1123 ns       448000
// BM_StringGetValue_Literal / string_basic              2544 ns         2539 ns       298667
// BM_StringGetValue_Literal / string_octal              2843 ns         2813 ns       248889
//
// MemoryBufferInputStream
// ------------------------------------------------------------------------------------------
// Benchmark                                                Time             CPU   Iterations
// ------------------------------------------------------------------------------------------
// BM_StringGetValue_Literal / string_empty               651 ns          639 ns       448000
// BM_StringGetValue_Literal / string_basic              1455 ns         1454 ns       298667
// BM_StringGetValue_Literal / string_octal              1813 ns         1814 ns       248889
IInputStreamPtr Buffer::ToInputStream(void) const {
    auto buffer = std::make_shared<fmt::memory_buffer>();
    buffer->append(std::string_view(m_data.data(), m_data.size()));
    return make_deferred<MemoryBufferInputStream>(buffer);
}

bool Buffer::Equals(const Buffer& other) const {
    return (m_data == other.m_data);
}

bool Buffer::LessThan(const Buffer& other) const {
    return (m_data < other.m_data);
}


bool operator==(const char * left, const Buffer& right) {
    assert(nullptr != left);
    return (0 == std::strncmp(left, reinterpret_cast<const char*>(right.data()), right.std_size()));
}

bool operator==(const Buffer& left, const char * right) {
    assert(nullptr != right);
    return (0 == std::strncmp(right, reinterpret_cast<const char*>(left.data()), left.std_size()));
}

bool operator!=(const char * left, const Buffer& right) {
    assert(nullptr != left);
    return (0 != std::strncmp(left, reinterpret_cast<const char*>(right.data()), right.std_size()));
}

bool operator!=(const Buffer& left, const char * right) {
    assert(nullptr != right);
    return (0 != std::strncmp(right, reinterpret_cast<const char*>(left.data()), left.std_size()));
}

std::ostream& operator<<(std::ostream& os, const Buffer& value) {
    os.write(value.data(), value.size());
    return os;
}

} // vanillapdf
