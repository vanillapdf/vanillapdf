#ifndef _C_INTERACTIVE_FORMS_H
#define _C_INTERACTIVE_FORMS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"
#include "vanillapdf/semantics/c_fields.h"

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
    * \deprecated
    * Enumerate the resolved terminal fields with
    * \ref InteractiveForm_GetFieldCount and \ref InteractiveForm_GetField
    * instead.
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFields(InteractiveFormHandle* handle, FieldCollectionHandle** result);

    /**
    * \brief
    * Set the array of references to the document's root fields.
    * \deprecated
    * Append fields with \ref InteractiveForm_AddField, which creates the
    * /Fields array on demand.
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetFields(InteractiveFormHandle* handle, FieldCollectionHandle* value);

    /**
    * \brief
    * Append a field to the root /Fields array, creating the array when the
    * form does not have one yet.
    *
    * The array holds indirect references, so the field's underlying
    * dictionary shall be registered as an indirect object within the
    * document, for example through \ref File_AllocateNewEntry.
    * \returns \ref VANILLAPDF_ERROR_PARAMETER_VALUE when the field's
    * dictionary is a direct object.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_AddField(InteractiveFormHandle* handle, FieldHandle* value);

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
    * Get the number of resolved terminal fields in the document.
    *
    * Terminal fields are the logical fields a user interacts with. The field
    * hierarchy's grouping nodes exist only for naming and attribute
    * inheritance and are not included - the same way the page tree hides its
    * interior nodes. A radio button group is a single terminal field with one
    * value, regardless of how many widget annotations represent it on the
    * page.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFieldCount(InteractiveFormHandle* handle, size_type* result);

    /**
    * \brief
    * Get the resolved terminal field at the given zero-based index, in
    * document order.
    *
    * \see \ref InteractiveForm_GetFieldCount
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetField(InteractiveFormHandle* handle, size_type index, FieldHandle** result);

    /**
    * \brief
    * Find a terminal field by its fully qualified name - the partial field
    * names (/T entries) joined with '.', UTF-8 encoded.
    *
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING when no terminal field has
    * that name.
    * \see \ref Field_GetQualifiedName
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_FindField(InteractiveFormHandle* handle, string_type qualified_name, FieldHandle** result);

    /**
    * \brief
    * Get the document-wide default appearance string (/DA entry).
    *
    * Fields resolve /DA through their /Parent chain only - when
    * \ref Field_GetDefaultAppearance reports
    * \ref VANILLAPDF_ERROR_OBJECT_MISSING, fall back to this document
    * default.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetDefaultAppearance(InteractiveFormHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the document-wide default appearance string (/DA entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetDefaultAppearance(InteractiveFormHandle* handle, StringObjectHandle* value);

    /**
    * \brief
    * Get the document-wide default quadding (/Q entry) - the text
    * justification of variable text fields.
    *
    * Fields resolve /Q through their /Parent chain only - when
    * \ref Field_GetQuadding reports \ref VANILLAPDF_ERROR_OBJECT_MISSING,
    * fall back to this document default. When this entry is missing as well,
    * the specification default is \ref QuaddingType_LeftJustified.
    * \see QuaddingType
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetQuadding(InteractiveFormHandle* handle, QuaddingType* result);

    /**
    * \brief Set the document-wide default quadding (/Q entry).
    * \see QuaddingType
    */
    VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetQuadding(InteractiveFormHandle* handle, QuaddingType value);

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
