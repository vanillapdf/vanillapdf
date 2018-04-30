#include "precompiled.h"

#include "semantics/objects/interactive_forms.h"

#include "syntax/files/file.h"
#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

InteractiveForm::InteractiveForm(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

bool InteractiveForm::Fields(OuputFieldCollectionPtr& result) const {
	if (!_obj->Contains(constant::Name::Fields)) {
		return false;
	}

	auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
	result = make_deferred<FieldCollection>(fields);
	return true;
}

FieldCollectionPtr InteractiveForm::CreateFields() {
	if (!_obj->Contains(constant::Name::Fields)) {
		syntax::MixedArrayObjectPtr mixed_array;
		mixed_array->SetFile(_obj->GetFile());
		mixed_array->SetInitialized();

		_obj->Insert(constant::Name::Fields, mixed_array);
	}

	auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
	return make_deferred<FieldCollection>(fields);
}

} // semantics
} // vanillapdf
