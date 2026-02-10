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
    */

    /**
    * \class FieldHandle
    * \extends IUnknownHandle
    * \ingroup group_fields
    * \brief Base class for all fields
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
    * \memberof FieldCollectionHandle
    * @{
    */

    /**
    * \brief Get size of field collection
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_GetSize(FieldCollectionHandle* handle, size_type* result);

    /**
    * \brief
    * Get single field from array at specific position
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_At(FieldCollectionHandle* handle, size_type at, FieldHandle** result);

    /**
    * \copydoc IUnknown_Release
    * \see \ref IUnknown_Release
    */
    VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_Release(FieldCollectionHandle* handle);

    /** @} */

    /**
    * \memberof FieldHandle
    * @{
    */

    /**
    * \brief
    * Create a field from a dictionary object.
    * The field type is determined by the /FT entry in the dictionary.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_CreateFromDictionary(DictionaryObjectHandle* dictionary, FieldHandle** result);

    /**
    * \brief
    * Return type of field.
    * Result can be used to convert to derived type.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetType(FieldHandle* handle, FieldType* result);

    /**
    * \brief Get the partial field name (/T entry).
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is not present.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetName(FieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the alternate field name (/TU entry), used as tooltip text.
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is not present.
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetAlternateName(FieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the field flags (/Ff entry).
    * \returns \ref VANILLAPDF_ERROR_OBJECT_MISSING if the entry is not present.
    * \see FieldFlags
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_GetFieldFlags(FieldHandle* handle, integer_type* result);

    /**
    * \brief Set the field flags (/Ff entry).
    * \see FieldFlags
    */
    VANILLAPDF_API error_type CALLING_CONVENTION Field_SetFieldFlags(FieldHandle* handle, integer_type value);

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
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_SetValue(TextFieldHandle* handle, LiteralStringObjectHandle* value);

    /**
    * \brief Get the default value (/DV entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetDefaultValue(TextFieldHandle* handle, StringObjectHandle** result);

    /**
    * \brief Get the maximum length of the text field (/MaxLen entry).
    */
    VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetMaxLength(TextFieldHandle* handle, integer_type* result);

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
    VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_SetValue(ChoiceFieldHandle* handle, LiteralStringObjectHandle* value);

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
