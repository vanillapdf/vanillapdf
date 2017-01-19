#include "precompiled.h"

#include "semantics/objects/interactive_forms.h"

#include "gotchangpdf/c_interactive_forms.h"
#include "c_helper.h"

using namespace gotchangpdf::semantics;

GOTCHANG_PDF_API error_type CALLING_CONVENTION InteractiveForm_GetFields(InteractiveFormHandle handle, FieldCollectionHandle* result) {
	InteractiveForm* form = reinterpret_cast<InteractiveForm*>(handle);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(form);
	RETURN_ERROR_PARAM_VALUE_IF_NULL(result);

	try {
		auto fields = form->Fields();
		auto ptr = fields.AddRefGet();
		*result = reinterpret_cast<FieldCollectionHandle>(ptr);
		return GOTCHANG_PDF_ERROR_SUCCES;
	} CATCH_GOTCHNGPDF_EXCEPTIONS
}

GOTCHANG_PDF_API error_type CALLING_CONVENTION InteractiveForm_Release(InteractiveFormHandle handle) {
	return ObjectRelease<InteractiveForm, InteractiveFormHandle>(handle);
}