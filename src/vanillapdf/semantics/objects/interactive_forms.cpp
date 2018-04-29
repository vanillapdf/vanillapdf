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
		auto file = _obj->GetFile().GetReference();
		auto chain = file->GetXrefChain();
		auto entry = chain->AllocateNewEntry();

		syntax::MixedArrayObjectPtr raw_dictionary;
		raw_dictionary->SetFile(file);
		raw_dictionary->SetInitialized();
		entry->SetReference(raw_dictionary);
		entry->SetFile(file);
		entry->SetInitialized();

		syntax::IndirectObjectReferencePtr ref = make_deferred<syntax::IndirectObjectReference>(raw_dictionary);
		_obj->Insert(constant::Name::Fields, ref);
	}

	auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
	return make_deferred<FieldCollection>(fields);
}

} // semantics
} // vanillapdf
