#ifndef _PKCS12_KEY_H
#define _PKCS12_KEY_H

#include "syntax/utils/syntax_fwd.h"
#include "syntax/utils/encryption.h"

namespace gotchangpdf {
namespace syntax {

class PKCS12Key : public IEncryptionKey {
public:
	explicit PKCS12Key(const Buffer& data);
	explicit PKCS12Key(const std::string& path);
	PKCS12Key(const std::string& path, const Buffer& password);
	PKCS12Key(const Buffer& data, const Buffer& password);
	BufferPtr Decrypt(const Buffer& data) const override;
	bool ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const override;

private:
	class PKCS12KeyImpl;
	std::shared_ptr<PKCS12KeyImpl> m_impl;
};

} // syntax
} // gotchangpdf

#endif /* _PKCS12_KEY_H */
