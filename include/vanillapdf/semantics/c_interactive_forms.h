#ifndef _C_INTERACTIVE_FORMS_H
#define _C_INTERACTIVE_FORMS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_interactive_forms.h
    * \brief This file contains class definitions for \ref InteractiveFormHandle
    */

    /**
    * \class InteractiveFormHandle
    * \extends IUnknownHandle
    * \ingroup group_documents
    * \brief
    * An interactive form (PDF 1.2) - sometimes referred to as an AcroForm - is
    * a collection of fields for gathering information interactively from the user.
    *
    * For more details please visit [section 12.7 - Interactive Forms](PDF32000_2008.pdf#G11.2110737).
    */

    /**
    * \memberof InteractiveFormHandle
    * @{
    */

    /**
    * \brief
    * Create an empty interactive form.
    *
    * Documents that already contain an AcroForm expose it through
    * \ref Catalog_GetAcroForm. This function is the entry point for documents
    * that do not have one yet - attach the resulting dictionary to the
    * document catalog under the /AcroForm key.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_Create(InteractiveFormHandle** result);

    /**
    * \brief
    * Create an interactive form from an existing low-level dictionary object.
    *
    * Use this overload to wrap a dictionary that is already registered
    * in the document, for example an indirect object obtained
    * through \ref File_AllocateNewEntry.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateFromDictionary(DictionaryObjectHandle* handle, InteractiveFormHandle** result);

    /**
    * \brief
    * An array of references to the document's root fields.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFields(InteractiveFormHandle* handle, FieldCollectionHandle** result);

    /**
    * \brief
    * An array of references to the document's root fields,
    * created as an empty array when the form does not have any yet.
    *
    * Unlike \ref InteractiveForm_GetFields this function never reports
    * \ref VANILLAPDF_ERROR_OBJECT_MISSING.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateFields(InteractiveFormHandle* handle, FieldCollectionHandle** result);

    /**
    * \brief
    * A set of flags specifying various document-level characteristics
    * related to signature fields.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetSignatureFlags(InteractiveFormHandle* handle, SignatureFlagsHandle** result);

    /**
    * \brief
    * A set of flags specifying various document-level characteristics related to
    * signature fields, created with all flags cleared when the form does not have any yet.
    *
    * Unlike \ref InteractiveForm_GetSignatureFlags this function never reports
    * \ref VANILLAPDF_ERROR_OBJECT_MISSING.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateSignatureFlags(InteractiveFormHandle* handle, SignatureFlagsHandle** result);

    /**
    * \brief
    * Get the NeedAppearances flag.
    * When true, the viewer application should regenerate field appearances.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetNeedAppearances(InteractiveFormHandle* handle, boolean_type* result);

    /**
    * \brief
    * Set the NeedAppearances flag.
    * Set to \ref VANILLAPDF_RV_TRUE to instruct the viewer to regenerate all field appearances from their values.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetNeedAppearances(InteractiveFormHandle* handle, boolean_type value);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_ToUnknown(InteractiveFormHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref InteractiveFormHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_FromUnknown(IUnknownHandle* handle, InteractiveFormHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_Release(InteractiveFormHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_INTERACTIVE_FORMS_H */
