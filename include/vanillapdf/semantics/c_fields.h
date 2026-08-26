#ifndef _C_FIELDS_H
#define _C_FIELDS_H

#include "vanillapdf/c_export.h"
#include "vanillapdf/c_handles.h"
#include "vanillapdf/c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
    * \file c_fields.h
    * \brief This file contains class definitions for interactive form fields
    */

    /**
    * \class FieldCollectionHandle
    * \extends IUnknownHandle
    * \ingroup group_fields
    * \brief Collection of \ref FieldHandle
    * \deprecated
    * The raw /Fields array models dictionary nodes rather than logical
    * fields. Use the field hierarchy from \ref InteractiveForm_GetFieldTree
    * instead - \ref FieldTree_GetFieldCount and \ref FieldTree_GetField
    * for the resolved terminal fields, \ref FieldTree_GetRootChild with
    * \ref Field_GetChild for the structure.
    */

    /**
    * \class FieldHandle
    * \extends IUnknownHandle
    * \ingroup group_fields
    * \brief
    * Base class for all fields.
    *
    * Fields form a hierarchy (12.7.3): the root /Fields array holds the
    * top-level fields, non-terminal fields group other fields through /Kids
    * for naming and attribute inheritance, and terminal fields are the
    * logical fields a user interacts with. A field is a view over its own
    * dictionary; the level above the top-level fields is the
    * \ref FieldTreeHandle.
    */

    /**
    * \class NonTerminalFieldHandle
    * \extends FieldHandle
    * \ingroup group_fields
    * \brief
    * Many field attributes are inheritable,
    * meaning that if they are not explicitly specified for a given field,
    * their values are taken from those of its parent in the field hierarchy.
    */

    /**
    * \class ButtonFieldHandle
    * \extends FieldHandle
    * \ingroup group_fields
    * \brief
    * A button field represents an interactive control on the screen
    * that the user can manipulate with the mouse.
    */

    /**
    * \class TextFieldHandle
    * \extends FieldHandle
    * \ingroup group_fields
    * \brief
    * A text field is a box or space for text
    * fill-in data typically entered from a keyboard.
    */

    /**
    * \class ChoiceFieldHandle
    * \extends FieldHandle
    * \ingroup group_fields
    * \brief
    * A choice field contains several text items, one or more
    * of which shall be selected as the field value.
    */

    /**
    * \class SignatureFieldHandle
    * \extends FieldHandle
    * \ingroup group_fields
    * \brief
    * A signature field (PDF 1.3) is a form field that contains a digital signature.
    * \see \ref DigitalSignatureHandle
    */

    /**
    * \brief Available types of fields
    * \ingroup group_fields
    */
    typedef enum {
        FieldType_Undefined = 0,

        /**
        * \copybrief NonTerminalFieldHandle
        * \see \ref NonTerminalFieldHandle
        */
        FieldType_NonTerminal,

        /**
        * \copybrief ButtonFieldHandle
        * \see \ref ButtonFieldHandle
        */
        FieldType_Button,

        /**
        * \copybrief TextFieldHandle
        * \see \ref TextFieldHandle
        */
        FieldType_Text,

        /**
        * \copybrief ChoiceFieldHandle
        * \see \ref ChoiceFieldHandle
        */
        FieldType_Choice,

        /**
        * \copybrief SignatureFieldHandle
        * \see \ref SignatureFieldHandle
        */
        FieldType_Signature,
    } FieldType;

    /**
    * \brief Bit flags common to all field types (PDF 32000-1:2008, Table 221)
    * \ingroup group_fields
    */
    typedef enum {
        FieldFlags_None = 0,

        /** \brief The user may not change the value of the field */
        FieldFlags_ReadOnly = (1 << 0),

        /** \brief The field shall have a value before the form can be submitted */
        FieldFlags_Required = (1 << 1),

        /** \brief The field shall not be exported by a submit-form action */
        FieldFlags_NoExport = (1 << 2),

        // Button field flags (Table 226)

        /** \brief The field is a set of radio buttons (otherwise checkbox if button type) */
        FieldFlags_Radio = (1 << 15),

        /** \brief The field is a push button that does not retain a permanent value */
        FieldFlags_PushButton = (1 << 16),

        // Text field flags (Table 228)

        /** \brief The field may contain multiple lines of text */
        FieldFlags_Multiline = (1 << 12),

        /** \brief The field is intended for entering a secure password */
        FieldFlags_Password = (1 << 13),

        // Choice field flags (Table 230)

        /** \brief The choice field is a combo box (otherwise list box) */
        FieldFlags_Combo = (1 << 17),

        /** \brief The combo box includes an editable text box */
        FieldFlags_Edit = (1 << 18),

        /** \brief The field options shall be sorted alphabetically */
        FieldFlags_Sort = (1 << 19),
    } FieldFlags;

    /**
    * \brief Form of quadding (justification) of variable text fields (PDF 32000-1:2008, Table 222)
    * \ingroup group_fields
    */
    typedef enum {
        /** \brief Text is left-justified */
        QuaddingType_LeftJustified = 0,

        /** \brief Text is centered */
        QuaddingType_Centered = 1,

        /** \brief Text is right-justified */
        QuaddingType_RightJustified = 2,
    } QuaddingType;

    /**
    * \memberof FieldCollectionHandle
    * @{
    */

    /**
    * \brief Get size of field collection
    * \deprecated Use \ref FieldTree_GetFieldCount instead
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_GetSize(FieldCollectionHandle* handle, size_type* result);

    /**
    * \brief
    * Get single field from array at specific position
    * \deprecated Use \ref FieldTree_GetField instead
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_At(FieldCollectionHandle* handle, size_type at, FieldHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    * \deprecated
    * Retained for releasing collections obtained through the deprecated
    * accessors - new code has no \ref FieldCollectionHandle to release.
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_Release(FieldCollectionHandle* handle);

    /** @} */

    /**
    * \memberof FieldHandle
    * @{
    */

    /**
    * \brief
    * Create a field from a dictionary object.
    * The field type is determined by the /FT entry in the dictionary,
    * resolved through the /Parent chain.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_CreateFromDictionary(DictionaryObjectHandle* dictionary, FieldHandle** result);

    /**
    * \brief
    * Return type of field.
    * Result can be used to convert to derived type.
    *
    * The /FT entry is inheritable and is resolved through the /Parent chain.
    * A non-terminal field without a resolvable type reports
    * \ref FieldType_NonTerminal.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetType(FieldHandle* handle, FieldType* result);

    /**
    * \brief Get the partial field name (/T entry).
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is not present.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetName(FieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief
    * Set the partial field name (/T entry).
    *
    * Because the PERIOD is used as a separator for fully qualified names,
    * a partial name shall not contain a PERIOD character.
    * \returns \ref VANILLAPDF_ERROR_PARAMETER_VALUE if the name contains a PERIOD.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_SetName(FieldHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the alternate field name (/TU entry), used as tooltip text.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is not present.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetAlternateName(FieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the alternate field name (/TU entry), used as tooltip text.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_SetAlternateName(FieldHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the field flags (/Ff entry), resolved through the /Parent chain.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is present on
    * neither the field nor any ancestor.
    * \see FieldFlags
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetFieldFlags(FieldHandle* handle, FieldFlags* result);

    /**
    * \brief Set the field flags (/Ff entry) in this field's own dictionary.
    * \see FieldFlags
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_SetFieldFlags(FieldHandle* handle, FieldFlags value);

    /**
    * \brief
    * Determine whether this is a terminal field.
    *
    * A terminal field has no children other than widget annotations - it is
    * a logical field the user interacts with. A non-terminal field groups
    * other fields for naming and attribute inheritance. A /Kids entry is a
    * child field when it carries /T, /Kids or /FT, and a widget annotation
    * otherwise; a field merged with its widget annotation carries both /T
    * and /Subtype /Widget and is a child field.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_IsTerminal(FieldHandle* handle, boolean_type* result);

    /**
    * \brief
    * Get the number of child fields.
    *
    * Child fields are the /Kids entries that are fields - widget annotations
    * are not children and are not counted. Zero for a terminal field. The
    * top-level fields are enumerated by \ref FieldTree_GetRootChildCount.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetChildCount(FieldHandle* handle, size_type* result);

    /**
    * \brief
    * Get the child field at the given zero-based index, in /Kids order.
    *
    * Together with \ref Field_GetChildCount this walks the hierarchy down
    * from the top-level fields of \ref FieldTree_GetRootChild.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING when the index is out
    * of range.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetChild(FieldHandle* handle, size_type index, FieldHandle** result);

    /**
    * \brief
    * Get the parent field.
    *
    * A nested field reports its /Parent, which is a valid parent for
    * \ref FieldTree_AddChild.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING for a top-level field,
    * which has no /Parent (Table 220 requires it for /Kids entries only) -
    * a sibling of it is added with \ref FieldTree_AddRootChild.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetParent(FieldHandle* handle, FieldHandle** result);

    /**
    * \brief
    * Get the fully qualified field name.
    *
    * Partial field names (/T entries) joined with '.' from the root of the
    * field hierarchy down to this field. Levels without a /T entry do not
    * contribute a segment. Each partial name is a text string and is
    * normalized to UTF-8, so the result is UTF-8 encoded regardless of how
    * the names are stored in the document. This is the form
    * \ref FieldTree_FindField accepts.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetQualifiedName(FieldHandle* handle, BufferHandle** result);

    /**
    * \brief
    * Get the field value (/V entry) as a raw object, resolved through the
    * /Parent chain.
    *
    * The object type depends on the field type: a name for button fields,
    * a text string for text fields, a text string or an array of text
    * strings for choice fields, a signature dictionary for signature
    * fields. The typed accessors such as \ref TextField_GetValue expose the
    * same entry with its type.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is present on
    * neither the field nor any ancestor.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetValue(FieldHandle* handle, ObjectHandle** result);

    /**
    * \brief
    * Get the default field value (/DV entry) as a raw object, resolved
    * through the /Parent chain.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is present on
    * neither the field nor any ancestor.
    * \see \ref Field_GetValue
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetDefaultValue(FieldHandle* handle, ObjectHandle** result);

    /**
    * \brief
    * Get the default appearance string (/DA entry) for variable text fields,
    * resolved through the /Parent chain.
    *
    * This is the field's own entry and its ancestors' only. The
    * document-wide default lives in the interactive form dictionary;
    * \ref InteractiveForm_ResolveDefaultAppearance performs the full lookup,
    * form default included.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is present on
    * neither the field nor any ancestor.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetDefaultAppearance(FieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief
    * Set the default appearance string (/DA entry) in this field's own
    * dictionary, overriding any inherited value. The document default is
    * set with \ref InteractiveForm_SetDefaultAppearance.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_SetDefaultAppearance(FieldHandle* handle, StringObjectHandle* value);

    /**
    * \brief
    * Get the quadding (/Q entry) - the text justification of variable text
    * fields - resolved through the /Parent chain.
    *
    * This is the field's own entry and its ancestors' only. The
    * document-wide default lives in the interactive form dictionary;
    * \ref InteractiveForm_ResolveQuadding performs the full lookup, form
    * default and specification default included.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is present on
    * neither the field nor any ancestor.
    * \see QuaddingType
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetQuadding(FieldHandle* handle, QuaddingType* result);

    /**
    * \brief
    * Set the quadding (/Q entry) in this field's own dictionary, overriding
    * any inherited value. The document default is set with
    * \ref InteractiveForm_SetQuadding.
    * \see QuaddingType
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_SetQuadding(FieldHandle* handle, QuaddingType value);

    /**
    * \brief
    * Get the underlying field dictionary.
    *
    * This is the escape hatch for anything the field API does not model -
    * the raw hierarchy is reachable through the dictionary's /Parent and
    * /Kids entries.
    *
    * The field hierarchy caches its flat view and cannot observe edits made
    * through the dictionary - call \ref FieldTree_Invalidate after changing
    * /Fields, /Kids or /Parent this way.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetBaseObject(FieldHandle* handle, DictionaryObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref ButtonFieldHandle.
    * \deprecated Use ButtonField_FromField instead
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION Field_ToButton(FieldHandle* handle, ButtonFieldHandle** result);

    /**
    * \brief Reinterpret current object as \ref TextFieldHandle.
    * \deprecated Use TextField_FromField instead
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION Field_ToText(FieldHandle* handle, TextFieldHandle** result);

    /**
    * \brief Reinterpret current object as \ref ChoiceFieldHandle.
    * \deprecated Use ChoiceField_FromField instead
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION Field_ToChoice(FieldHandle* handle, ChoiceFieldHandle** result);

    /**
    * \brief Reinterpret current object as \ref SignatureFieldHandle.
    * \deprecated Use SignatureField_FromField instead
    */
    VANILLAPDF_DEPRECATED VANILLAPDF_API error_type CALLING_CONVENTION Field_ToSignature(FieldHandle* handle, SignatureFieldHandle** result);

    /**
    * \brief Reinterpret current object as \ref IUnknownHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_ToUnknown(FieldHandle* handle, IUnknownHandle** result);

    /**
    * \brief Convert \ref IUnknownHandle to \ref FieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_FromUnknown(IUnknownHandle* handle, FieldHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_Release(FieldHandle* handle);

    /** @} */

    /**
    * \memberof ButtonFieldHandle
    * @{
    */

    /**
    * \brief Get the button field value (/V entry) as a name object.
    * For checkboxes, typically /Yes or /Off. For radio buttons, the selected option name.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_GetValue(ButtonFieldHandle* handle, NameObjectHandle** result);

    /**
    * \brief Set the button field value (/V entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_SetValue(ButtonFieldHandle* handle, NameObjectHandle* value);

    /**
    * \brief Reinterpret current object as \ref FieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_ToField(ButtonFieldHandle* handle, FieldHandle** result);

    /**
    * \brief Convert \ref FieldHandle to \ref ButtonFieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_FromField(FieldHandle* handle, ButtonFieldHandle** result);

    /**
    * \copydoc Field_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_Release(ButtonFieldHandle* handle);

    /** @} */

    /**
    * \memberof TextFieldHandle
    * @{
    */

    /**
    * \brief Get the text field value (/V entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetValue(TextFieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the text field value (/V entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_SetValue(TextFieldHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the default value (/DV entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetDefaultValue(TextFieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the maximum length of the text field (/MaxLen entry).
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is not present.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetMaxLength(TextFieldHandle* handle, IntegerObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref FieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_ToField(TextFieldHandle* handle, FieldHandle** result);

    /**
    * \brief Convert \ref FieldHandle to \ref TextFieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_FromField(FieldHandle* handle, TextFieldHandle** result);

    /**
    * \copydoc Field_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_Release(TextFieldHandle* handle);

    /** @} */

    /**
    * \memberof ChoiceFieldHandle
    * @{
    */

    /**
    * \brief Get the selected value (/V entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_GetValue(ChoiceFieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief Set the selected value (/V entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_SetValue(ChoiceFieldHandle* handle, StringObjectHandle* value);

    /**
    * \brief Get the number of available options (/Opt entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_GetOptionCount(ChoiceFieldHandle* handle, size_type* result);

    /**
    * \brief Get the option at the given index.
    *
    * Each element is either a text string (serving as both export value and display text)
    * or a two-element array where the first element is the export value
    * and the second element is the display text (Table 231).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_GetOptionAt(ChoiceFieldHandle* handle, size_type index, ObjectHandle** result);

    /**
    * \brief Reinterpret current object as \ref FieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_ToField(ChoiceFieldHandle* handle, FieldHandle** result);

    /**
    * \brief Convert \ref FieldHandle to \ref ChoiceFieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_FromField(FieldHandle* handle, ChoiceFieldHandle** result);

    /**
    * \copydoc Field_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_Release(ChoiceFieldHandle* handle);

    /** @} */

    /**
    * \memberof SignatureFieldHandle
    * @{
    */

    /**
    * \brief
    * A signature dictionary containing the signature and
    * specifying various attributes of the signature field.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_GetValue(SignatureFieldHandle* handle, DigitalSignatureHandle** result);

    /**
    * \brief Reinterpret current object as \ref FieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_ToField(SignatureFieldHandle* handle, FieldHandle** result);

    /**
    * \brief Convert \ref FieldHandle to \ref SignatureFieldHandle
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_FromField(FieldHandle* handle, SignatureFieldHandle** result);

    /**
    * \copydoc Field_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_Release(SignatureFieldHandle* handle);

    /** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FIELDS_H */
