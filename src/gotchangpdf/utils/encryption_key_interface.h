#ifndef _ENCRYPTION_KEY_INTERFACE_H
#define _ENCRYPTION_KEY_INTERFACE_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"

namespace gotchangpdf {

class IEncryptionKey : public virtual IUnknown, public IWeakReferenceable<IEncryptionKey> {
public:
	virtual BufferPtr Decrypt(const Buffer& data) const = 0;
	virtual bool ContainsPrivateKey(const Buffer& issuer, const Buffer& serial) const = 0;
};

} // gotchangpdf

#endif /* _ENCRYPTION_KEY_INTERFACE_H */
