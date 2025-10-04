#ifndef _TRUSTED_CERTIFICATE_STORE_H
#define _TRUSTED_CERTIFICATE_STORE_H

#include "utils/utils_fwd.h"
#include "utils/unknown_interface.h"
#include "utils/buffer.h"

#include <string>
#include <memory>

namespace vanillapdf {

/**
* \class TrustedCertificateStore
* \brief Collection of trusted certificates for signature verification
*
* This class manages a collection of trusted X.509 certificates used
* to validate certificate chains during signature verification.
*/
class TrustedCertificateStore : public IUnknown {
public:
    TrustedCertificateStore();
    ~TrustedCertificateStore();

    /**
    * \brief Add a certificate from PEM format
    * \param pem_data PEM-encoded certificate data
    */
    void AddCertificateFromPEM(const std::string& pem_data);

    /**
    * \brief Add a certificate from DER format
    * \param der_data DER-encoded certificate data
    */
    void AddCertificateFromDER(const Buffer& der_data);

    /**
    * \brief Add a certificate from file
    * \param file_path Path to certificate file (PEM or DER, auto-detected)
    */
    void AddCertificateFromFile(const std::string& file_path);

    /**
    * \brief Load certificates from directory
    * \param directory_path Path to directory containing certificate files
    *
    * Loads all certificate files from the specified directory.
    * Commonly used for loading from /etc/ssl/certs on Linux.
    */
    void LoadFromDirectory(const std::string& directory_path);

    /**
    * \brief Load system default trusted certificates
    *
    * On Windows: Uses Windows Certificate Store
    * On Linux/macOS: Uses OpenSSL default paths (/etc/ssl/certs, etc.)
    */
    void LoadSystemDefaults();

    /**
    * \brief Get native handle for OpenSSL operations
    * \return Pointer to X509_STORE (cast to void* for ABI stability)
    *
    * Internal use only - provides access to underlying OpenSSL X509_STORE
    */
    void* GetNativeHandle() const;

private:
    class TrustedCertificateStoreImpl;
    std::shared_ptr<TrustedCertificateStoreImpl> m_impl;
};

} // vanillapdf

#endif /* _TRUSTED_CERTIFICATE_STORE_H */
