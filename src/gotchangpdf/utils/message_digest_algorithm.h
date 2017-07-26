#ifndef _MESSAGE_DIGEST_ALGORITHM_H
#define _MESSAGE_DIGEST_ALGORITHM_H

namespace gotchangpdf {

enum class MessageDigestAlgorithm {
	None = 0,
	MDNULL,
	MD2,
	MD4,
	MD5,
	SHA,
	SHA1,
	DSS,
	DSS1,
	ECDSA,
	SHA224,
	SHA256,
	SHA384,
	SHA512,
	MDC2,
	RIPEMD160,
	WHIRLPOOL
};

} // gotchangpdf

#endif /* _MESSAGE_DIGEST_ALGORITHM_H */
