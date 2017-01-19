#include "precompiled.h"

#include "semantics/objects/interactive_forms.h"

namespace gotchangpdf {
namespace semantics {

InteractiveForm::InteractiveForm(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

FieldCollectionPtr InteractiveForm::Fields(void) const {
	auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
	return FieldCollectionPtr(fields);
}

} // semantics
} // gotchangpdf
