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
		/**
		* \copybrief ButtonFieldHandle
		* \see \ref ButtonFieldHandle
		*/
		FieldType_Button = 0,

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
	* Return type of field.
	* Result can be used to convert to derived type.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Field_GetType(FieldHandle* handle, FieldType* result);

	/**
	* \brief Reinterpret current object as \ref ButtonFieldHandle.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Field_ToButton(FieldHandle* handle, ButtonFieldHandle** result);

	/**
	* \brief Reinterpret current object as \ref TextFieldHandle.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Field_ToText(FieldHandle* handle, TextFieldHandle** result);

	/**
	* \brief Reinterpret current object as \ref ChoiceFieldHandle.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Field_ToChoice(FieldHandle* handle, ChoiceFieldHandle** result);

	/**
	* \brief Reinterpret current object as \ref SignatureFieldHandle.
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION Field_ToSignature(FieldHandle* handle, SignatureFieldHandle** result);

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
	* \copydoc Field_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_Release(ButtonFieldHandle* handle);

	/** @} */

	/**
	* \memberof TextFieldHandle
	* @{
	*/

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
	* \copydoc Field_Release
	*/
	VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_Release(SignatureFieldHandle* handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FIELDS_H */
