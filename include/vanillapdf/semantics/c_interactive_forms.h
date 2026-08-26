#ifndef _C_INTERACTIVE_FORMS_H
#define _C_INTERACTIVE_FORMS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"
#include "vanillapdf/semantics/c_fields.h"
#include "vanillapdf/semantics/c_field_tree.h"

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
    * The form exposes its document-level attributes; the fields themselves
    * live in the field hierarchy obtained from
    * \ref InteractiveForm_GetFieldTree.
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
    * Get the field hierarchy of the form - its /Fields entry.
    *
    * Reading never creates the entry. The same hierarchy instance is
    * returned for the lifetime of this form handle, so the flat view cache
    * it owns is shared by everyone holding the form.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING when the form has no
    * /Fields entry - attach one with \ref InteractiveForm_SetFieldTree.
    * \see \ref FieldTreeHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFieldTree(InteractiveFormHandle* handle, FieldTreeHandle** result);

    /**
    * \brief
    * Attach a field hierarchy to the form as its /Fields entry, replacing
    * any existing one.
    *
    * Obtain an empty hierarchy from \ref FieldTree_CreateFromDocument for
    * a form that does not have any fields yet. The attached instance
    * becomes the one \ref InteractiveForm_GetFieldTree returns, so the
    * caller's handle and the form share a single cache.
    *
    * \returns \ref VANILLAPDF_ERROR_PARAMETER_VALUE when the hierarchy was
    * created for a different document than the form belongs to - its
    * references would serialize as dangling object numbers here.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetFieldTree(InteractiveFormHandle* handle, FieldTreeHandle* value);

    /**
    * \brief
    * An array of references to the document's root fields.
    * \deprecated
    * The raw /Fields array models dictionary nodes rather than logical
    * fields. Use the field hierarchy from \ref InteractiveForm_GetFieldTree
    * instead - \ref FieldTree_GetFieldCount and \ref FieldTree_GetField
    * for the resolved terminal fields, \ref FieldTree_GetRootChild with
    * \ref Field_GetChild for the structure.
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFields(InteractiveFormHandle* handle, FieldCollectionHandle** result);

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
    * \brief
    * Get the document-wide default appearance string (/DA entry).
    *
    * This is the form's own entry. Fields resolve /DA through their /Parent
    * chain only; \ref InteractiveForm_ResolveDefaultAppearance performs the
    * full lookup for a field, this default included.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is not present.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetDefaultAppearance(InteractiveFormHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the document-wide default appearance string (/DA entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetDefaultAppearance(InteractiveFormHandle* handle, StringObjectHandle* value);

    /**
    * \brief
    * Get the default appearance string (/DA entry) a field is rendered
    * with: the field's own entry, then its ancestors through /Parent, then
    * this form's document default (12.7.3.3).
    *
    * \ref Field_GetDefaultAppearance stops at the field hierarchy; the form
    * owns the last step because it owns the entry. /DA is required for
    * variable text fields but has no further default, so it can be missing
    * everywhere. The field is taken as given - it is not checked to belong
    * to this form's hierarchy.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is present on
    * neither the field, any of its ancestors, nor the form.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_ResolveDefaultAppearance(InteractiveFormHandle* handle, FieldHandle* field, StringObjectHandle** result);

    /**
    * \brief
    * Get the document-wide default quadding (/Q entry) - the text
    * justification of variable text fields.
    *
    * This is the form's own entry. Fields resolve /Q through their /Parent
    * chain only; \ref InteractiveForm_ResolveQuadding performs the full
    * lookup for a field, this default included.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is not present.
    * \see QuaddingType
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetQuadding(InteractiveFormHandle* handle, QuaddingType* result);

    /**
    * \brief Set the document-wide default quadding (/Q entry).
    * \see QuaddingType
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetQuadding(InteractiveFormHandle* handle, QuaddingType value);

    /**
    * \brief
    * Get the quadding (/Q entry) a field is rendered with: the field's own
    * entry, then its ancestors through /Parent, then this form's document
    * default, and finally the specification default
    * \ref QuaddingType_LeftJustified (Table 222) - so it always resolves.
    *
    * \ref Field_GetQuadding stops at the field hierarchy; the form owns the
    * last step because it owns the entry. The field is taken as given - it
    * is not checked to belong to this form's hierarchy.
    * \see QuaddingType
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_ResolveQuadding(InteractiveFormHandle* handle, FieldHandle* field, QuaddingType* result);

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
