#ifndef _C_MD5_DIGEST_H
#define _C_MD5_DIGEST_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_md5_digest.h
    * \brief This file contains the MD5 message digest used by the PDF standard security handler
    */

    /**
    * \class MD5DigestHandle
    * \extends IUnknownHandle
    * \ingroup group_utils
    * \brief MD5 message digest (RFC 1321)
    *
    * MD5 derives the encryption keys of the standard security handler
    * revisions 2 through 4. It is cryptographically broken and provided
    * solely for PDF compatibility; do not use it for anything else.
    *
    * Feed the data with \ref MD5Digest_Update, then obtain the digest
    * with \ref MD5Digest_Final. The digest cannot be updated afterwards.
    */

    /**
    * \memberof MD5DigestHandle
    * @{
    */

    /**
    * \brief Creates a new empty digest
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_Create(
        MD5DigestHandle** result
    );

    /**
    * \brief Appends \p data to the hashed message
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_Update(
        MD5DigestHandle* handle,
        BufferHandle* data
    );

    /**
    * \brief Completes the digest and returns its 16 bytes
    *
    * Fails with \ref VANILLAPDF_ERROR_PARAMETER_VALUE when the digest has already been finalized.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_Final(
        MD5DigestHandle* handle,
        BufferHandle** result
    );

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_ToUnknown(
        MD5DigestHandle* handle,
        IUnknownHandle** result
    );

    /**
    * \brief Convert \ref IUnknownHandle to \ref MD5DigestHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_FromUnknown(
        IUnknownHandle* handle,
        MD5DigestHandle** result
    );

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION MD5Digest_Release(
        MD5DigestHandle* handle
    );

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_MD5_DIGEST_H */
