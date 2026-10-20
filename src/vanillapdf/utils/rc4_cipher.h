#ifndef _RC4_CIPHER_H
#define _RC4_CIPHER_H

#include "utils/utils_fwd.h"

#include <array>

namespace vanillapdf {

/**
 * @brief RC4 (ARCFOUR) stream cipher.
 *
 * Required by the PDF standard security handler revisions 2 through 4.
 * Implemented in-house because OpenSSL 3 moved RC4 into its legacy provider,
 * a separately loaded module whose location cannot be resolved reliably
 * on every platform.
 *
 * RC4 is symmetric: Process both encrypts and decrypts. The keystream
 * continues across Process calls, so splitting the data into several
 * calls yields the same output as processing it at once.
 */
class RC4Cipher : public IUnknown {
public:
    static constexpr types::size_type MIN_KEY_LENGTH = 1;
    static constexpr types::size_type MAX_KEY_LENGTH = 256;

    explicit RC4Cipher(const Buffer& key);

    /**
     * @brief Uses only the first \p key_length bytes of \p key.
     */
    RC4Cipher(const Buffer& key, types::size_type key_length);

    BufferPtr Process(const Buffer& data);

private:
    std::array<uint8_t, 256> m_state;
    uint8_t m_first_index = 0;
    uint8_t m_second_index = 0;
};

} // vanillapdf

#endif /* _RC4_CIPHER_H */
