#include "precompiled.h"

#include "semantics/objects/interactive_forms.h"
#include "semantics/objects/signature_flags.h"
#include "semantics/objects/fields.h"
#include "semantics/objects/document.h"

#include "syntax/objects/dictionary_object.h"

#include "vanillapdf/semantics/c_interactive_forms.h"
#include "implementation/c_helper.h"

using namespace vanillapdf;
using namespace vanillapdf::syntax;
using namespace vanillapdf::semantics;

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateFromDictionary(DictionaryObjectHandle* handle, InteractiveFormHandle** result) {
    DictionaryObject* dictionary = reinterpret_cast<DictionaryObject*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(dictionary);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto form = make_deferred<InteractiveForm>(dictionary);
        auto ptr = form.AddRefGet();
        *result = reinterpret_cast<InteractiveFormHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateFromDocument(DocumentHandle* handle, InteractiveFormHandle** result) {
    Document* document = reinterpret_cast<Document*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(document);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto form = InteractiveForm::Create(document);
        auto ptr = form.AddRefGet();
        *result = reinterpret_cast<InteractiveFormHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFields(InteractiveFormHandle* handle, FieldCollectionHandle** result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OuputFieldCollectionPtr fields;
        bool contains = form->GetFields(fields);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = fields.AddRefGet();
        *result = reinterpret_cast<FieldCollectionHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetFields(InteractiveFormHandle* handle, FieldCollectionHandle* value) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    FieldCollection* fields = reinterpret_cast<FieldCollection*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(fields);

    try {
        form->SetFields(fields);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_AddField(InteractiveFormHandle* handle, FieldHandle* value) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    Field* field = reinterpret_cast<Field*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(field);

    try {
        form->AddField(field);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetSignatureFlags(InteractiveFormHandle* handle, SignatureFlagsHandle** result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputSignatureFlagsPtr flags;
        bool contains = form->GetSignatureFlags(flags);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = flags.AddRefGet();
        *result = reinterpret_cast<SignatureFlagsHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetSignatureFlags(InteractiveFormHandle* handle, SignatureFlagsHandle* value) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    SignatureFlags* flags = reinterpret_cast<SignatureFlags*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(flags);

    try {
        form->SetSignatureFlags(flags);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetNeedAppearances(InteractiveFormHandle* handle, boolean_type* result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        bool value;
        bool contains = form->GetNeedAppearances(value);
        if (!contains) return VANILLAPDF_ERROR_OBJECT_MISSING;
        *result = value ? VANILLAPDF_RV_TRUE : VANILLAPDF_RV_FALSE;
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetNeedAppearances(InteractiveFormHandle* handle, boolean_type value) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);

    try {
        form->SetNeedAppearances(value == VANILLAPDF_RV_TRUE);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetFieldCount(InteractiveFormHandle* handle, size_type* result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        *result = form->GetFieldCount();
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetField(InteractiveFormHandle* handle, size_type index, FieldHandle** result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto field = form->GetField(index);
        auto ptr = field.AddRefGet();
        *result = reinterpret_cast<FieldHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_FindField(InteractiveFormHandle* handle, string_type qualified_name, FieldHandle** result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(qualified_name);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OuputFieldPtr field;
        bool found = form->TryFindField(qualified_name, field);
        if (!found) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = field.AddRefGet();
        *result = reinterpret_cast<FieldHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetDefaultAppearance(InteractiveFormHandle* handle, StringObjectHandle** result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        OutputStringObjectPtr appearance;
        bool contains = form->GetDefaultAppearance(appearance);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        auto ptr = appearance.AddRefGet();
        *result = reinterpret_cast<StringObjectHandle*>(ptr);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetDefaultAppearance(InteractiveFormHandle* handle, StringObjectHandle* value) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    StringObjectBase* appearance = reinterpret_cast<StringObjectBase*>(value);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(appearance);

    try {
        form->SetDefaultAppearance(appearance);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_SetQuadding(InteractiveFormHandle* handle, QuaddingType value) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);

    try {
        Field::Quadding quadding = Field::Quadding::LeftJustified;
        switch (value) {
            case QuaddingType_LeftJustified:
                quadding = Field::Quadding::LeftJustified; break;
            case QuaddingType_Centered:
                quadding = Field::Quadding::Centered; break;
            case QuaddingType_RightJustified:
                quadding = Field::Quadding::RightJustified; break;
            default:
                return VANILLAPDF_ERROR_PARAMETER_VALUE;
        }

        form->SetQuadding(quadding);
        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_GetQuadding(InteractiveFormHandle* handle, QuaddingType* result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        Field::Quadding quadding = Field::Quadding::LeftJustified;
        bool contains = form->GetQuadding(quadding);
        if (!contains) {
            return VANILLAPDF_ERROR_OBJECT_MISSING;
        }

        switch (quadding) {
            case Field::Quadding::LeftJustified:
                *result = QuaddingType_LeftJustified; break;
            case Field::Quadding::Centered:
                *result = QuaddingType_Centered; break;
            case Field::Quadding::RightJustified:
                *result = QuaddingType_RightJustified; break;
            default:
                return VANILLAPDF_ERROR_GENERAL;
        }

        return VANILLAPDF_ERROR_SUCCESS;
    } CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_ToUnknown(InteractiveFormHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<InteractiveForm, IUnknown, InteractiveFormHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_FromUnknown(IUnknownHandle* handle, InteractiveFormHandle** result) {
    return SafeObjectConvert<IUnknown, InteractiveForm, IUnknownHandle, InteractiveFormHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_Release(InteractiveFormHandle* handle) {
    return ObjectRelease<InteractiveForm, InteractiveFormHandle>(handle);
}