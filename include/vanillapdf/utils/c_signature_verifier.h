#ifndef _C_SIGNATURE_VERIFIER_H
#define _C_SIGNATURE_VERIFIER_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_signature_verification.h
    * \brief This file contains APIs for PDF signature verification
    */

    /**
    * \class TrustedCertificateStoreHandle
    * \extends IUnknownHandle
    * \ingroup group_utils
    * \brief Collection of trusted certificates for signature verification
    */

    /**
    * \class SignatureVerificationResultHandle
    * \extends IUnknownHandle
    * \ingroup group_utils
    * \brief Result of signature verification operation
    */


    /**
    * \brief Overall status of signature verification
    * \ingroup group_utils
    */
    typedef enum {
        SignatureStatus_Undefined = 0,          /**< Status not set (uninitialized) */
        SignatureStatus_Valid,                  /**< Signature is cryptographically valid */
        SignatureStatus_Invalid,                /**< Signature verification failed */
        SignatureStatus_CertificateExpired,     /**< Certificate has expired */
        SignatureStatus_CertificateNotYetValid, /**< Certificate not yet valid */
        SignatureStatus_CertificateRevoked,     /**< Certificate has been revoked */
        SignatureStatus_CertificateUntrusted,   /**< Certificate chain not trusted */
        SignatureStatus_DocumentModified,       /**< Document modified after signing */
        SignatureStatus_WeakAlgorithm,          /**< Weak digest algorithm (MD5, SHA1) */
        SignatureStatus_MissingCertificate,     /**< Signature missing certificate */
        SignatureStatus_Unknown                 /**< Unable to determine status */
    } SignatureVerificationStatusType;

    /**
    * \memberof TrustedCertificateStoreHandle
    * @{
    */

    /**
    * \brief Create an empty trusted certificate store
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_Create(
        TrustedCertificateStoreHandle** result
    );

    /**
    * \brief Add a certificate from PEM format
    * \param handle The certificate store
    * \param pem_data PEM-encoded certificate data (null-terminated UTF-8 string)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_AddCertificateFromPEM(
        TrustedCertificateStoreHandle* handle,
        const BufferHandle* pem_data
    );

    /**
    * \brief Add a certificate from DER format
    * \param handle The certificate store
    * \param der_data DER-encoded certificate data
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_AddCertificateFromDER(
        TrustedCertificateStoreHandle* handle,
        const BufferHandle* der_data
    );

    /**
    * \brief Load certificates from directory (e.g., /etc/ssl/certs)
    * \param handle The certificate store
    * \param directory_path Path to directory containing certificates (UTF-8 encoded)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_LoadFromDirectory(
        TrustedCertificateStoreHandle* handle,
        string_type directory_path
    );

    /**
    * \brief Load system default trusted certificates
    * \param handle The certificate store
    *
    * On Windows: Uses Windows Certificate Store
    * On Linux/macOS: Uses OpenSSL default paths
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_LoadSystemDefaults(
        TrustedCertificateStoreHandle* handle
    );

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TrustedCertificateStore_Release(
        TrustedCertificateStoreHandle* handle
    );

    /** @} */

    /**
    * \memberof SignatureVerifier
    * @{
    */

    /**
    * \brief Verify digital signature (low-level API)
    * \param signed_data The raw bytes that were signed
    * \param signature_contents The PKCS#7 signature blob
    * \param trusted_store Trusted certificate store (required)
    * \param settings Verification settings (optional, can be NULL for defaults)
    * \param result Output verification result
    *
    * This is a low-level API that operates on raw bytes extracted from PDF.
    * If settings is NULL, default settings are used (all flags disabled).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerifier_Verify(
        const BufferHandle* signed_data,
        const BufferHandle* signature_contents,
        TrustedCertificateStoreHandle* trusted_store,
        SignatureVerificationSettingsHandle* settings,
        SignatureVerificationResultHandle** result
    );

    /** @} */

    /**
    * \memberof SignatureVerificationResultHandle
    * @{
    */

    /**
    * \brief Get overall verification status
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetStatus(
        SignatureVerificationResultHandle* handle,
        SignatureVerificationStatusType* result
    );

    /**
    * \brief Get human-readable message describing verification result
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetMessage(
        SignatureVerificationResultHandle* handle,
        BufferHandle** result
    );

    /**
    * \brief Check if signature is cryptographically valid
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_IsSignatureValid(
        SignatureVerificationResultHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Check if document bytes are intact (ByteRange matches)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_IsDocumentIntact(
        SignatureVerificationResultHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Check if certificate chain is trusted
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_IsCertificateTrusted(
        SignatureVerificationResultHandle* handle,
        boolean_type* result
    );

    /**
    * \brief Get signer's certificate (DER-encoded)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetSignerCertificate(
        SignatureVerificationResultHandle* handle,
        BufferHandle** result
    );

    /**
    * \brief Get number of certificates in the chain
    * \param handle The verification result
    * \param result Output certificate count (0 = signer, 1+ = intermediate/root)
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetCertificateChainCount(
        SignatureVerificationResultHandle* handle,
        size_type* result
    );

    /**
    * \brief Get certificate from chain at index
    * \param handle The verification result
    * \param index Certificate index (0 = signer, 1+ = intermediate/root)
    * \param result Output DER-encoded certificate
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetCertificateChainAt(
        SignatureVerificationResultHandle* handle,
        size_type index,
        BufferHandle** result
    );

    /**
    * \brief Get signer's common name from certificate
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_GetSignerCommonName(
        SignatureVerificationResultHandle* handle,
        BufferHandle** result
    );

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureVerificationResult_Release(
        SignatureVerificationResultHandle* handle
    );

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_SIGNATURE_VERIFIER_H */
