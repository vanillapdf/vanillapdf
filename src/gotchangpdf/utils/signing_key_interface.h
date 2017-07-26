#ifndef _SIGNING_KEY_INTERFACE_H
#define _SIGNING_KEY_INTERFACE_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"
#include "utils/message_digest_algorithm.h"

namespace gotchangpdf {

class ISigningKey : public virtual IUnknown, public IWeakReferenceable<ISigningKey> {
public:
	virtual BufferPtr Sign(const Buffer& data, MessageDigestAlgorithm algorithm) const = 0;
};

} // gotchangpdf

#endif /* _SIGNING_KEY_INTERFACE_H */
