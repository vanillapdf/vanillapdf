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
    * Create an interactive form from an existing low-level dictionary object.
    *
    * Use this overload to wrap a dictionary that is already registered in the
    * document, for example an indirect object obtained through
    * \ref File_AllocateNewEntry. Prefer
    * \ref InteractiveForm_CreateFromDocument otherwise.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateFromDictionary(DictionaryObjectHandle* handle, InteractiveFormHandle** result);

    /**
    * \brief
    * Create an empty interactive form registered as an indirect object
    * within the document.
    *
    * The form does not take effect until it is attached with
    * \ref Catalog_SetAcroForm. Creating and attaching are deliberately
    * separate steps, so that reading a document never modifies it.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateFromDocument(DocumentHandle* handle, InteractiveFormHandle** result);

    /**
    * \brief
    * An array of references to the document's root fields.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFields(InteractiveFormHandle* handle, FieldCollectionHandle** result);

    /**
    * \brief
    * Set the array of references to the document's root fields.
    *
    * Obtain an empty collection from \ref FieldCollection_Create for a form
    * that does not have any fields yet.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetFields(InteractiveFormHandle* handle, FieldCollectionHandle* value);

    /**
    * \brief
    * A set of flags specifying various document-level characteristics
    * related to signature fields.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetSignatureFlags(InteractiveFormHandle* handle, SignatureFlagsHandle** result);

    /**
    * \brief
    * Set the flags specifying document-level characteristics related to
    * signature fields.
    *
    * Obtain a cleared set of flags from \ref SignatureFlags_Create for a form
    * that does not have any yet.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetSignatureFlags(InteractiveFormHandle* handle, SignatureFlagsHandle* value);

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
