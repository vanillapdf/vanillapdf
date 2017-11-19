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
		auto fields = form->Fields();
		auto ptr = fields.AddRefGet();
		*result = reinterpret_cast<FieldCollectionHandle*>(ptr);
		return VANILLAPDF_ERROR_SUCCESS;
	} CATCH_VANILLAPDF_EXCEPTIONS
}

VANILLAPDF_API error_type CALLING_CONVENTION InteractiveForm_Release(InteractiveFormHandle* handle) {
	return ObjectRelease<InteractiveForm, InteractiveFormHandle*>(handle);
}