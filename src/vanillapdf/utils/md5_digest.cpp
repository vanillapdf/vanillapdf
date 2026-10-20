#include "precompiled.h"

#include "utils/md5_digest.h"
#include "utils/buffer.h"
#include "utils/byte_order.h"
#include "utils/math_utils.h"

#include <algorithm>
#include <cstring>

namespace vanillapdf {

// Per-round shift amounts
static const uint32_t MD5_SHIFTS[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

// Message word used by each step, a fixed permutation per round
static const uint8_t MD5_WORD_INDEXES[64] = {
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
     1,  6, 11,  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12,
     5,  8, 11, 14,  1,  4,  7, 10, 13,  0,  3,  6,  9, 12, 15,  2,
     0,  7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9
};

// floor(abs(sin(i + 1)) * 2^32)
static const uint32_t MD5_CONSTANTS[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// The 0x80 terminator followed by zeroes
static const uint8_t MD5_PADDING[64] = { 0x80 };

// Auxiliary functions of the four rounds, RFC 1321 section 3.4
static uint32_t RoundFunctionF(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); }
static uint32_t RoundFunctionG(uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (y & ~z); }
static uint32_t RoundFunctionH(uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; }
static uint32_t RoundFunctionI(uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); }

using RoundFunction = uint32_t (*)(uint32_t, uint32_t, uint32_t);
static const RoundFunction MD5_ROUND_FUNCTIONS[4] = { RoundFunctionF, RoundFunctionG, RoundFunctionH, RoundFunctionI };

// MD5 is defined on little-endian words. The conversion is its own inverse,
// so it serves both for reading the input and for writing the digest.
template <typename T>
static T ConvertLittleEndian(T value) {
    if constexpr (endian::native == endian::big) {
        return ByteSwap(value);
    }

    return value;
}

void MD5Digest::Update(const Buffer& data) {
    Update(data, data.size());
}

void MD5Digest::Update(const Buffer& data, types::size_type length) {
    if (length > data.size()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "MD5 input length {} exceeds the buffer size {}",
            length, data.size());
    }

    UpdateBytes(reinterpret_cast<const uint8_t*>(data.data()), length);
}

BufferPtr MD5Digest::Final() {
    if (m_finalized) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "MD5 digest has already been finalized");
    }

    // The message length in bits, modulo 2^64, captured before the padding is added
    uint64_t bit_length = ConvertLittleEndian<uint64_t>(m_total_length * 8);

    // Pad until 56 bytes of the last block are filled, leaving room for the length
    types::size_type padding_length = (m_block_length < BLOCK_SIZE - sizeof(bit_length))
        ? (BLOCK_SIZE - sizeof(bit_length) - m_block_length)
        : (2 * BLOCK_SIZE - sizeof(bit_length) - m_block_length);

    UpdateBytes(MD5_PADDING, padding_length);
    UpdateBytes(reinterpret_cast<const uint8_t*>(&bit_length), sizeof(bit_length));
    assert(m_block_length == 0 && "MD5 padding did not complete the last block");

    std::array<uint32_t, 4> digest_words;
    std::transform(m_state.begin(), m_state.end(), digest_words.begin(), ConvertLittleEndian<uint32_t>);

    BufferPtr result = make_deferred_container<Buffer>(DIGEST_LENGTH);
    std::memcpy(result->data(), digest_words.data(), DIGEST_LENGTH);

    m_finalized = true;
    return result;
}

void MD5Digest::UpdateBytes(const uint8_t* data, types::size_type length) {
    if (m_finalized) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "MD5 digest cannot be updated after it has been finalized");
    }

    m_total_length += length;

    // Fill the block as far as the input reaches, transforming each completed block
    while (length > 0) {
        auto copy_length = std::min(length, BLOCK_SIZE - m_block_length);
        std::copy_n(data, copy_length, m_block.begin() + m_block_length);

        m_block_length += copy_length;
        data += copy_length;
        length -= copy_length;

        if (m_block_length == BLOCK_SIZE) {
            Transform();
            m_block_length = 0;
        }
    }
}

void MD5Digest::Transform() {
    std::array<uint32_t, 16> words;
    static_assert(sizeof(words) == BLOCK_SIZE, "MD5 block has to map onto sixteen words");

    std::memcpy(words.data(), m_block.data(), sizeof(words));
    std::transform(words.begin(), words.end(), words.begin(), ConvertLittleEndian<uint32_t>);

    uint32_t a = m_state[0];
    uint32_t b = m_state[1];
    uint32_t c = m_state[2];
    uint32_t d = m_state[3];

    // Four rounds of sixteen steps, each step a = b + ((a + round(b, c, d) + X[k] + T[i]) <<< s)
    for (types::size_type round = 0; round < 4; ++round) {
        auto round_function = MD5_ROUND_FUNCTIONS[round];

        for (types::size_type step = 0; step < 16; ++step) {
            auto i = round * 16 + step;
            auto sum = a + round_function(b, c, d) + words[MD5_WORD_INDEXES[i]] + MD5_CONSTANTS[i];

            a = d;
            d = c;
            c = b;
            b = b + RotateLeft(sum, MD5_SHIFTS[i]);
        }
    }

    m_state[0] += a;
    m_state[1] += b;
    m_state[2] += c;
    m_state[3] += d;
}

} // vanillapdf
