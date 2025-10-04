#ifndef _SIGNATURE_VERIFIER_INTERFACE_H
#define _SIGNATURE_VERIFIER_INTERFACE_H

#include "utils/utils_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/message_digest_algorithm.h"

namespace vanillapdf {

/**
* \class ISignatureVerifier
* \brief Interface for signature verification operations
*
* Similar to ISigningKey but for verification operations.
* Follows Initialize → Update → Verify pattern.
*/
class ISignatureVerifier : public IUnknown {
public:
    /**
    * \brief Initialize verification engine using selected digest algorithm
    * \param algorithm The message digest algorithm to use for verification
    */
    virtual void VerifyInitialize(MessageDigestAlgorithm algorithm) = 0;

    /**
    * \brief Insert data into engine for digest calculation
    * \param data Buffer containing data to verify
    *
    * This function can be called multiple times if there is more data
    * to verify than would fit in a single buffer.
    */
    virtual void VerifyUpdate(BufferPtr data) = 0;

    /**
    * \brief Insert data from stream into engine for digest calculation
    * \param data Input stream containing data to verify
    * \param length Number of bytes to read from the stream
    */
    virtual void VerifyUpdate(IInputStreamPtr data, types::stream_size length) = 0;

    /**
    * \brief Finish the digest calculation and verify the signature
    * \param signature_contents The signature value to verify against
    * \return true if signature is valid, false otherwise
    *
    * This function should calculate the hash value according to the algorithm
    * specified in VerifyInitialize and verify it against the provided signature.
    */
    virtual bool VerifyFinal(const Buffer& signature_contents) = 0;

    /**
    * \brief Cleanup all dependencies after the verification process
    *
    * This function is called when verification is complete.
    */
    virtual void VerifyCleanup() = 0;
};

} // vanillapdf

#endif /* _SIGNATURE_VERIFIER_INTERFACE_H */
