#ifndef _SIGNING_KEY_INTERFACE_H
#define _SIGNING_KEY_INTERFACE_H

#include "utils/utils_fwd.h"
#include "utils/buffer.h"
#include "utils/message_digest_algorithm.h"

#include "utils/streams/input_stream_interface.h"

namespace vanillapdf {

class ISigningKey : public virtual IUnknown, public IWeakReferenceable<ISigningKey> {
public:
	virtual void SignInitialize(MessageDigestAlgorithm algorithm) = 0;
	virtual void SignUpdate(const Buffer& data) = 0;
	virtual void SignUpdate(IInputStreamPtr data, types::stream_size length) = 0;
	virtual BufferPtr SignFinal() = 0;
	virtual BufferPtr GetSigningCertificate() const = 0;
};

} // vanillapdf

#endif /* _SIGNING_KEY_INTERFACE_H */
