#ifndef _C_RC4_CIPHER_H
#define _C_RC4_CIPHER_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_rc4_cipher.h
    * \brief This file contains the RC4 stream cipher used by the PDF standard security handler
    */

    /**
    * \class RC4CipherHandle
    * \extends IUnknownHandle
    * \ingroup group_utils
    * \brief RC4 (ARCFOUR) stream cipher
    *
    * RC4 is required to read and write documents protected by the standard
    * security handler revisions 2 through 4. It is cryptographically broken
    * and provided solely for PDF compatibility; do not use it for anything else.
    *
    * The cipher is symmetric, \ref RC4Cipher_Process both encrypts and decrypts.
    * The keystream continues across calls, so processing data in several
    * parts yields the same output as processing it at once.
    */

    /**
    * \memberof RC4CipherHandle
    * @{
    */

    /**
    * \brief Creates a new cipher initialized with \p key
    * \param key the key, 1 to 256 bytes long
    * \param result a pointer to the created cipher on success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_CreateFromKey(
        BufferHandle* key,
        RC4CipherHandle** result
    );

    /**
    * \brief Encrypts or decrypts \p data with the following bytes of the keystream
    * \param handle the cipher
    * \param data the input data
    * \param result a pointer to the output buffer of the same length as \p data on success
    */
    VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_Process(
        RC4CipherHandle* handle,
        BufferHandle* data,
        BufferHandle** result
    );

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_ToUnknown(
        RC4CipherHandle* handle,
        IUnknownHandle** result
    );

    /**
    * \brief Convert \ref IUnknownHandle to \ref RC4CipherHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_FromUnknown(
        IUnknownHandle* handle,
        RC4CipherHandle** result
    );

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION RC4Cipher_Release(
        RC4CipherHandle* handle
    );

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_RC4_CIPHER_H */
