#ifndef _C_FIELDS_H
#define _C_FIELDS_H

#include "c_export.h"
#include "c_handles.h"
#include "c_values.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	* \file c_fields.h
	* This file contains class definitions for interactive form fields
	*/

	/**
	* \class FieldCollectionHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief Collection of FieldHandle
	*/

	/**
	* \class FieldHandle
	* \extends IUnknownHandle
	* \ingroup Documents
	* \brief Represents document's authenticated digital signature
	*/

	/**
	* \class ButtonFieldHandle
	* \extends FieldHandle
	* \ingroup Documents
	* \brief
	* A button field represents an interactive control on the screen
	* that the user can manipulate with the mouse.
	*/

	/**
	* \class TextFieldHandle
	* \extends FieldHandle
	* \ingroup Documents
	* \brief
	* A text field is a box or space for text
	* fill-in data typically entered from a keyboard.
	*/

	/**
	* \class ChoiceFieldHandle
	* \extends FieldHandle
	* \ingroup Documents
	* \brief
	* A choice field contains several text items, one or more
	* of which shall be selected as the field value.
	*/

	/**
	* \class SignatureFieldHandle
	* \extends FieldHandle
	* \ingroup Documents
	* \brief
	* A signature field (PDF 1.3) is a form field that contains
	* a digital signature (see 12.8, "Digital Signatures").
	*/

	/**
	* \brief
	* Available types of fields
	*/
	typedef enum {
		/**
		* \copydoc ButtonFieldHandle
		* \see ButtonFieldHandle
		*/
		FieldType_Button = 0,

		/**
		* \copydoc TextFieldHandle
		* \see TextFieldHandle
		*/
		FieldType_Text,

		/**
		* \copydoc ChoiceFieldHandle
		* \see ChoiceFieldHandle
		*/
		FieldType_Choice,

		/**
		* \copydoc SignatureFieldHandle
		* \see SignatureFieldHandle
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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FieldCollection_Size(FieldCollectionHandle handle, size_type* result);

	/**
	* \brief
	* Get single field from array at specific position
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FieldCollection_At(FieldCollectionHandle handle, size_type at, FieldHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION FieldCollection_Release(FieldCollectionHandle handle);

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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_GetType(FieldHandle handle, FieldType* result);

	/**
	* \brief Reinterpret current object as ButtonFieldHandle.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_ToButton(FieldHandle handle, ButtonFieldHandle* result);

	/**
	* \brief Reinterpret current object as TextFieldHandle.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_ToText(FieldHandle handle, TextFieldHandle* result);

	/**
	* \brief Reinterpret current object as ChoiceFieldHandle.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_ToChoice(FieldHandle handle, ChoiceFieldHandle* result);

	/**
	* \brief Reinterpret current object as SignatureFieldHandle.
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_ToSignature(FieldHandle handle, SignatureFieldHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_Release(FieldHandle handle);

	/** @} */

	/**
	* \memberof ButtonFieldHandle
	* @{
	*/

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ButtonField_Release(ButtonFieldHandle handle);

	/** @} */

	/**
	* \memberof TextFieldHandle
	* @{
	*/

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION TextField_Release(TextFieldHandle handle);

	/** @} */

	/**
	* \memberof ChoiceFieldHandle
	* @{
	*/

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION ChoiceField_Release(ChoiceFieldHandle handle);

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
	GOTCHANG_PDF_API error_type CALLING_CONVENTION SignatureField_GetValue(SignatureFieldHandle handle, DigitalSignatureHandle* result);

	/**
	* \copydoc IUnknown_Release
	* \see IUnknown_Release
	*/
	GOTCHANG_PDF_API error_type CALLING_CONVENTION SignatureField_Release(SignatureFieldHandle handle);

	/** @} */

#ifdef __cplusplus
};
#endif

#endif /* _C_FIELDS_H */
