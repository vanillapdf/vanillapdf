#include "precompiled.h"
#include "semantics/utils/pdf_text_string.h"

#include "semantics/objects/fields.h"
#include "syntax/objects/dictionary_object.h"

#include "vanillapdf/semantics/c_fields.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION Field_CreateFromDictionary(DictionaryObjectHandle* dictionary, FieldHandle** result) {
    DictionaryObject* dict = reinterpret_cast<DictionaryObject*>(dictionary);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(dict);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto field = Field::Create(dict);
        auto ptr = field.AddRefGet();
        *result = reinterpret_cast<FieldHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_GetType(FieldHandle* handle, FieldType* result) {
    Field* obj = reinterpret_cast<Field*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    switch (obj->GetFieldType()) {
        case Field::Type::NonTerminal:
            *result = FieldType_NonTerminal; break;
        case Field::Type::Button:
            *result = FieldType_Button; break;
        case Field::Type::Text:
            *result = FieldType_Text; break;
        case Field::Type::Choice:
            *result = FieldType_Choice; break;
        case Field::Type::Signature:
            *result = FieldType_Signature; break;
        default:
            return VANILLAPDF_ERROR_GENERAL;
    }

    return VANILLAPDF_ERROR_SUCCESS;
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_GetName(FieldHandle* handle, StringObjectHandle** result) {
    Field* obj = reinterpret_cast<Field*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = obj->GetName(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_GetAlternateName(FieldHandle* handle, StringObjectHandle** result) {
    Field* obj = reinterpret_cast<Field*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = obj->GetAlternateName(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_GetNameText(FieldHandle* handle, PdfTextStringHandle** result) {
    Field* obj = reinterpret_cast<Field*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        PdfTextStringPtr text_string;
        if (!obj->GetNameText(text_string)) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_GetAlternateNameText(FieldHandle* handle, PdfTextStringHandle** result) {
    Field* obj = reinterpret_cast<Field*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        PdfTextStringPtr text_string;
        if (!obj->GetAlternateNameText(text_string)) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_GetFieldFlags(FieldHandle* handle, FieldFlags* result) {
    Field* obj = reinterpret_cast<Field*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        types::big_int flags;
        bool contains = obj->GetFieldFlags(flags);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        *result = static_cast<FieldFlags>(flags);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_SetFieldFlags(FieldHandle* handle, FieldFlags value) {
    Field* obj = reinterpret_cast<Field*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);

    try {
        obj->SetFieldFlags(static_cast<types::big_int>(value));
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_GetValue(ButtonFieldHandle* handle, NameObjectHandle** result) {
    ButtonField* obj = reinterpret_cast<ButtonField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputNameObjectPtr direct;
        bool contains = obj->GetValue(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<NameObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_SetValue(ButtonFieldHandle* handle, NameObjectHandle* value) {
    ButtonField* obj = reinterpret_cast<ButtonField*>(handle);
    NameObject* name = reinterpret_cast<NameObject*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(name);

    try {
        obj->SetValue(name);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetValue(TextFieldHandle* handle, StringObjectHandle** result) {
    TextField* obj = reinterpret_cast<TextField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = obj->GetValue(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_SetValue(TextFieldHandle* handle, StringObjectHandle* value) {
    TextField* obj = reinterpret_cast<TextField*>(handle);
    StringObjectBase* str = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(str);

    try {
        obj->SetValue(str);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetDefaultValue(TextFieldHandle* handle, StringObjectHandle** result) {
    TextField* obj = reinterpret_cast<TextField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = obj->GetDefaultValue(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetValueText(TextFieldHandle* handle, PdfTextStringHandle** result) {
    TextField* obj = reinterpret_cast<TextField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        PdfTextStringPtr text_string;
        if (!obj->GetValueText(text_string)) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetDefaultValueText(TextFieldHandle* handle, PdfTextStringHandle** result) {
    TextField* obj = reinterpret_cast<TextField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        PdfTextStringPtr text_string;
        if (!obj->GetDefaultValueText(text_string)) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_SetValueText(TextFieldHandle* handle, PdfTextStringHandle* value) {
    TextField* obj = reinterpret_cast<TextField*>(handle);
    PdfTextString* text = reinterpret_cast<PdfTextString*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(text);

    try {
        obj->SetValueText(text);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_GetMaxLength(TextFieldHandle* handle, IntegerObjectHandle** result) {
    TextField* obj = reinterpret_cast<TextField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputIntegerObjectPtr direct;
        bool contains = obj->GetMaxLength(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<IntegerObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_GetValue(ChoiceFieldHandle* handle, StringObjectHandle** result) {
    ChoiceField* obj = reinterpret_cast<ChoiceField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr direct;
        bool contains = obj->GetValue(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_SetValue(ChoiceFieldHandle* handle, StringObjectHandle* value) {
    ChoiceField* obj = reinterpret_cast<ChoiceField*>(handle);
    StringObjectBase* str = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(str);

    try {
        obj->SetValue(str);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_GetValueText(ChoiceFieldHandle* handle, PdfTextStringHandle** result) {
    ChoiceField* obj = reinterpret_cast<ChoiceField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        PdfTextStringPtr text_string;
        if (!obj->GetValueText(text_string)) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = text_string.AddRefGet();
        *result = reinterpret_cast<PdfTextStringHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_SetValueText(ChoiceFieldHandle* handle, PdfTextStringHandle* value) {
    ChoiceField* obj = reinterpret_cast<ChoiceField*>(handle);
    PdfTextString* text = reinterpret_cast<PdfTextString*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(text);

    try {
        obj->SetValueText(text);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_GetOptionCount(ChoiceFieldHandle* handle, size_type* result) {
    ChoiceField* obj = reinterpret_cast<ChoiceField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        types::size_type count;
        bool contains = obj->GetOptionCount(count);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        *result = count;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_GetOptionAt(ChoiceFieldHandle* handle, size_type index, ObjectHandle** result) {
    ChoiceField* obj = reinterpret_cast<ChoiceField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(obj);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputContainableObjectPtr direct;
        bool contains = obj->GetOptionAt(index, direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<ObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_GetSize(FieldCollectionHandle* handle, size_type* result) {
    FieldCollection* collection = reinterpret_cast<FieldCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = collection->GetSize();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_At(FieldCollectionHandle* handle, size_type at, FieldHandle** result) {
    FieldCollection* collection = reinterpret_cast<FieldCollection*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(collection);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto direct = collection->At(at);
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<FieldHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_GetValue(SignatureFieldHandle* handle, DigitalSignatureHandle** result) {
    SignatureField* field = reinterpret_cast<SignatureField*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(field);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OuputDigitalSignaturePtr direct;
        bool contains = field->Value(direct);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        auto ptr = direct.AddRefGet();
        *result = reinterpret_cast<DigitalSignatureHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_ToButton(FieldHandle* handle, ButtonFieldHandle** result) {
    return SafeObjectConvert<Field, ButtonField, FieldHandle, ButtonFieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_ToText(FieldHandle* handle, TextFieldHandle** result) {
    return SafeObjectConvert<Field, TextField, FieldHandle, TextFieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_ToChoice(FieldHandle* handle, ChoiceFieldHandle** result) {
    return SafeObjectConvert<Field, ChoiceField, FieldHandle, ChoiceFieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_ToSignature(FieldHandle* handle, SignatureFieldHandle** result) {
    return SafeObjectConvert<Field, SignatureField, FieldHandle, SignatureFieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_ToField(ButtonFieldHandle* handle, FieldHandle** result) {
    return SafeObjectConvert<ButtonField, Field, ButtonFieldHandle, FieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_FromField(FieldHandle* handle, ButtonFieldHandle** result) {
    return SafeObjectConvert<Field, ButtonField, FieldHandle, ButtonFieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_ToField(TextFieldHandle* handle, FieldHandle** result) {
    return SafeObjectConvert<TextField, Field, TextFieldHandle, FieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_FromField(FieldHandle* handle, TextFieldHandle** result) {
    return SafeObjectConvert<Field, TextField, FieldHandle, TextFieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_ToField(ChoiceFieldHandle* handle, FieldHandle** result) {
    return SafeObjectConvert<ChoiceField, Field, ChoiceFieldHandle, FieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_FromField(FieldHandle* handle, ChoiceFieldHandle** result) {
    return SafeObjectConvert<Field, ChoiceField, FieldHandle, ChoiceFieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_ToField(SignatureFieldHandle* handle, FieldHandle** result) {
    return SafeObjectConvert<SignatureField, Field, SignatureFieldHandle, FieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_FromField(FieldHandle* handle, SignatureFieldHandle** result) {
    return SafeObjectConvert<Field, SignatureField, FieldHandle, SignatureFieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION FieldCollection_Release(FieldCollectionHandle* handle) {
    return ObjectRelease<FieldCollection, FieldCollectionHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_ToUnknown(FieldHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<Field, IUnknown, FieldHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_FromUnknown(IUnknownHandle* handle, FieldHandle** result) {
    return SafeObjectConvert<IUnknown, Field, IUnknownHandle, FieldHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION Field_Release(FieldHandle* handle) {
    return ObjectRelease<Field, FieldHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ButtonField_Release(ButtonFieldHandle* handle) {
    return ObjectRelease<ButtonField, ButtonFieldHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION TextField_Release(TextFieldHandle* handle) {
    return ObjectRelease<TextField, TextFieldHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION ChoiceField_Release(ChoiceFieldHandle* handle) {
    return ObjectRelease<ChoiceField, ChoiceFieldHandle>(handle);
}

VANILLAPDF_API error_type CALLING_CONVENTION SignatureField_Release(SignatureFieldHandle* handle) {
    return ObjectRelease<SignatureField, SignatureFieldHandle>(handle);
}
