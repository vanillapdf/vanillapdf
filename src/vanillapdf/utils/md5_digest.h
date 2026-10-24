#ifndef _MD5_DIGEST_H
#define _MD5_DIGEST_H

#include "utils/utils_fwd.h"

#include <array>

namespace vanillapdf {

/**
 * @brief MD5 message digest (RFC 1321).
 *
 * Required by the PDF standard security handler revisions 2 through 4
 * for encryption key derivation. Implemented in-house so that these
 * documents do not depend on the OpenSSL configuration: FIPS mode and
 * restrictive system crypto policies (RHEL, Rocky, Fedora) refuse MD5.
 *
 * Not suitable for any security purpose other than PDF compatibility.
 */
class MD5Digest : public IUnknown {
public:
    static constexpr types::size_type DIGEST_LENGTH = 16;

    void Update(const Buffer& data);

    /**
     * @brief Hashes only the first \p length bytes of \p data.
     */
    void Update(const Buffer& data, types::size_type length);

    /**
     * @brief Applies the padding and returns the DIGEST_LENGTH byte digest.
     *
     * The object cannot be updated or finalized again afterwards.
     */
    BufferPtr Final();

private:
    static constexpr types::size_type BLOCK_SIZE = 64;

    void UpdateBytes(const uint8_t* data, types::size_type length);
    void Transform();

    std::array<uint32_t, 4> m_state = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
    std::array<uint8_t, BLOCK_SIZE> m_block = {};
    types::size_type m_block_length = 0;
    uint64_t m_total_length = 0;
    bool m_finalized = false;
};

} // vanillapdf

#endif /* _MD5_DIGEST_H */
