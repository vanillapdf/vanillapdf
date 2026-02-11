#include "precompiled.h"

#include "semantics/objects/interactive_forms.h"

#include "vanillapdf/semantics/c_interactive_forms.h"
#include "implementation/c_helper.h"

using namespace vanillapdf::semantics;

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

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_Release(InteractiveFormHandle* handle) {
    return ObjectRelease<InteractiveForm, InteractiveFormHandle>(handle);
}