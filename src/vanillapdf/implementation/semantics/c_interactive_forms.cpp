#include "precompiled.h"

#include "semantics/objects/interactive_forms.h"
#include "semantics/objects/signature_flags.h"

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

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateFields(InteractiveFormHandle* handle, FieldCollectionHandle** result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto fields = form->CreateFields();
        auto ptr = fields.AddRefGet();
        *result = reinterpret_cast<FieldCollectionHandle*>(ptr);
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

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_CreateSignatureFlags(InteractiveFormHandle* handle, SignatureFlagsHandle** result) {
    InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
    RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

    try {
        auto flags = form->CreateSignatureFlags();
        auto ptr = flags.AddRefGet();
        *result = reinterpret_cast<SignatureFlagsHandle*>(ptr);
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

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_ToUnknown(InteractiveFormHandle* handle, IUnknownHandle** result) {
    return SafeObjectConvert<InteractiveForm, IUnknown, InteractiveFormHandle, IUnknownHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_FromUnknown(IUnknownHandle* handle, InteractiveFormHandle** result) {
    return SafeObjectConvert<IUnknown, InteractiveForm, IUnknownHandle, InteractiveFormHandle>(handle, result);
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_Release(InteractiveFormHandle* handle) {
    return ObjectRelease<InteractiveForm, InteractiveFormHandle>(handle);
}