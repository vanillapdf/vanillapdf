#ifndef _PKCS12_KEY_H
#define _PKCS12_KEY_H

#include "utils/utils_fwd.h"
#include "utils/buffer_array.h"
#include "utils/crypto_utils.h"
#include "utils/encryption_key_interface.h"
#include "utils/signing_key_interface.h"

#include <string_view>

namespace vanillapdf {

class PKCS12Key : public IEncryptionKey, public ISigningKey {
public:
    explicit PKCS12Key(const Buffer& data);
    explicit PKCS12Key(const std::string& path);
    PKCS12Key(const std::string& path, std::string_view password);
    PKCS12Key(const Buffer& data, std::string_view password);

    // IEncryptionKey
    BufferPtr Decrypt(const Buffer& data) override;
    bool ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const override;

    // ISigningKey
    void SignInitialize(MessageDigestAlgorithm algorithm) override;
    void SignUpdate(const Buffer& data) override;
    void SignUpdate(IInputStreamPtr data, types::stream_size length) override;
    BufferPtr SignFinal() override;
    void SignCleanup() override;

    // Certificate access
    BufferPtr GetCertificate() const;

private:
    class PKCS12KeyImpl;
    std::shared_ptr<PKCS12KeyImpl> m_impl;
};

} // vanillapdf

#endif /* _PKCS12_KEY_H */
