#include "precompiled.h"

#include "utils/rc4_cipher.h"
#include "utils/buffer.h"

#include <numeric>
#include <utility>

namespace vanillapdf {

RC4Cipher::RC4Cipher(const Buffer& key) : RC4Cipher(key, key.size()) {
}

RC4Cipher::RC4Cipher(const Buffer& key, types::size_type key_length) {
    if (key_length < MIN_KEY_LENGTH || key_length > MAX_KEY_LENGTH) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "RC4 key length must be between {} and {} bytes, got {}",
            MIN_KEY_LENGTH, MAX_KEY_LENGTH, key_length);
    }

    if (key_length > key.size()) {
        LOG_ERROR_AND_THROW(InvalidParameterException, "RC4 key length {} exceeds the key buffer size {}",
            key_length, key.size());
    }

    // Key scheduling algorithm
    std::iota(m_state.begin(), m_state.end(), static_cast<uint8_t>(0));

    uint8_t swap_index = 0;
    for (types::size_type i = 0; i < m_state.size(); ++i) {
        auto key_byte = static_cast<uint8_t>(key[i % key_length]);
        swap_index = static_cast<uint8_t>(swap_index + m_state[i] + key_byte);
        std::swap(m_state[i], m_state[swap_index]);
    }
}

BufferPtr RC4Cipher::Process(const Buffer& data) {
    BufferPtr result = make_deferred_container<Buffer>(data.size());

    // Pseudo-random generation algorithm, the keystream is XORed with the data
    for (types::size_type i = 0; i < data.size(); ++i) {
        m_first_index = static_cast<uint8_t>(m_first_index + 1);
        m_second_index = static_cast<uint8_t>(m_second_index + m_state[m_first_index]);
        std::swap(m_state[m_first_index], m_state[m_second_index]);

        auto keystream_index = static_cast<uint8_t>(m_state[m_first_index] + m_state[m_second_index]);
        auto data_byte = static_cast<uint8_t>(data[i]);
        result[i] = static_cast<Buffer::value_type>(data_byte ^ m_state[keystream_index]);
    }

    return result;
}

} // vanillapdf
