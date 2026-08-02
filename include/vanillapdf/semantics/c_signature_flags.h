#ifndef _C_SIGNATURE_FLAGS_H
#define _C_SIGNATURE_FLAGS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_signature_flags.h
    * \brief This file contains class definitions for \ref SignatureFlagsHandle
    */

    /**
    * \class SignatureFlagsHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * A set of flags specifying various document-level characteristics
    * related to signature fields.
    *
    * For more details please visit [section 12.7.2 - Interactive Form Dictionary](PDF32000_2008.pdf#G11.2110762).
    * \see \ref InteractiveFormHandle
    */

    /**
    * \memberof SignatureFlagsHandle
    * @{
    */

    /**
    * \brief
    * Create a set of signature flags with every flag cleared.
    *
    * Attach it to an interactive form with
    * \ref InteractiveForm_SetSignatureFlags.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_Create(SignatureFlagsHandle** result);

    /**
    * \brief
    * If \ref VANILLAPDF_RV_TRUE, the document contains at least one signature field.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_GetSignaturesExist(SignatureFlagsHandle* handle, boolean_type* result);

    /**
    * \copydoc SignatureFlags_GetSignaturesExist
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_SetSignaturesExist(SignatureFlagsHandle* handle, boolean_type value);

    /**
    * \brief
    * If \ref VANILLAPDF_RV_TRUE, the document contains signatures that may be
    * invalidated when the file is saved in a way that alters its previous contents,
    * as opposed to an incremental update.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_GetAppendOnly(SignatureFlagsHandle* handle, boolean_type* result);

    /**
    * \copydoc SignatureFlags_GetAppendOnly
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_SetAppendOnly(SignatureFlagsHandle* handle, boolean_type value);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_ToUnknown(SignatureFlagsHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref SignatureFlagsHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_FromUnknown(IUnknownHandle* handle, SignatureFlagsHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureFlags_Release(SignatureFlagsHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_SIGNATURE_FLAGS_H */
