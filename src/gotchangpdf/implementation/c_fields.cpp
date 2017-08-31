#include "precompiled.h"

#include "semantics/objects/fields.h"

#include "gotchangpdf/c_fields.h"
#include "c_helper.h"

using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_GetType(FieldHandle handle, FieldType* result) {
	Field* obj = reinterpret_cast<Field*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	switch (obj->GetType()) {
		case Field::Type::Button:
			*result = FieldType_Button; break;
		case Field::Type::Text:
			*result = FieldType_Text; break;
		case Field::Type::Choice:
			*result = FieldType_Choice; break;
		case Field::Type::Signature:
			*result = FieldType_Signature; break;
		default:
			return GOTCHANG_PDF_ERROR_GENERAL;
	}

	return GOTCHANG_PDF_ERROR_SUCCESS;
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FieldCollection_Size(FieldCollectionHandle handle, size_type* result) {
	FieldCollection* collection = reinterpret_cast<FieldCollection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		*result = collection->Size();
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FieldCollection_At(FieldCollectionHandle handle, size_type at, FieldHandle* result) {
	FieldCollection* collection = reinterpret_cast<FieldCollection*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto direct = collection->At(at);
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<FieldHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION SignatureField_GetValue(SignatureFieldHandle handle, DigitalSignatureHandle* result) {
	SignatureField* field = reinterpret_cast<SignatureField*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(field);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		OuputDigitalSignaturePtr direct;
		bool contains = field->Value(direct);
		if (!contains) return GOTCHANG_PDF_ERROR_OBJECT_MISSING;
		auto ptr = direct.AddRefGet();
		*result = reinterpret_cast<DigitalSignatureHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCESS;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_ToButton(FieldHandle handle, ButtonFieldHandle* result) {
	return SafeObjectConvert<Field, ButtonField, FieldHandle, ButtonFieldHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_ToText(FieldHandle handle, TextFieldHandle* result) {
	return SafeObjectConvert<Field, TextField, FieldHandle, TextFieldHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_ToChoice(FieldHandle handle, ChoiceFieldHandle* result) {
	return SafeObjectConvert<Field, ChoiceField, FieldHandle, ChoiceFieldHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_ToSignature(FieldHandle handle, SignatureFieldHandle* result) {
	return SafeObjectConvert<Field, SignatureField, FieldHandle, SignatureFieldHandle>(handle, result);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION FieldCollection_Release(FieldCollectionHandle handle) {
	return ObjectRelease<FieldCollection, FieldCollectionHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION Field_Release(FieldHandle handle) {
	return ObjectRelease<Field, FieldHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ButtonField_Release(ButtonFieldHandle handle) {
	return ObjectRelease<ButtonField, ButtonFieldHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION TextField_Release(TextFieldHandle handle) {
	return ObjectRelease<TextField, TextFieldHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION ChoiceField_Release(ChoiceFieldHandle handle) {
	return ObjectRelease<ChoiceField, ChoiceFieldHandle>(handle);
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION SignatureField_Release(SignatureFieldHandle handle) {
	return ObjectRelease<SignatureField, SignatureFieldHandle>(handle);
}
