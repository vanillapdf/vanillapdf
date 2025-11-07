#include "precompiled.h"

#include "vanillapdf/semantics/c_digital_signature_extensions.h"
#include "implementation/c_helper.h"

#include "semantics/extensions/digital_signature_extensions.h"
#include "semantics/objects/digital_signature.h"
#include "semantics/objects/document.h"
#include "utils/trusted_certificate_store.h"
#include "utils/signature_verification_result.h"

using namespace vanillapdf;
using namespace vanillapdf::semantics;

error_type CALLING_CONVENTION DigitalSignatureExtensions_Verify(
    DigitalSignatureHandle* signature,
    DocumentHandle* document,
    TrustedCertificateStoreHandle* trusted_store,
    VerificationFlagType flags,
    SignatureVerificationResultHandle** result
) {
    DigitalSignature* signature_obj = reinterpret_cast<DigitalSignature*>(signature);
    Document* document_obj = reinterpret_cast<Document*>(document);
    TrustedCertificateStore* trusted_store_obj = reinterpret_cast<TrustedCertificateStore*>(trusted_store);

    RETURN_ERROR_PARAM_VALUE_IF_NULL(signature_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(document_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(trusted_store_obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try
    {
        // Convert verification flags from C enum to C++ enum
        auto cpp_flags = static_cast<VerificationFlags>(flags);

        // Call the extension method
        auto verification_result = DigitalSignatureExtensions::Verify(
            signature_obj,
            document_obj,
            trusted_store_obj,
            cpp_flags
        );

        // Increment reference count for output
        auto ptr = verification_result.AddRefGet();

        // Return result
        *result = reinterpret_cast<SignatureVerificationResultHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    }
    CATCH_VANILLAPDF_EXCEPTIONS
}
