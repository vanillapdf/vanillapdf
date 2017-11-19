#include "precompiled.h"

#include "semantics/objects/interactive_forms.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

InteractiveForm::InteractiveForm(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

FieldCollectionPtr InteractiveForm::Fields(void) const {
	auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
	return make_deferred<FieldCollection>(fields);
}

} // semantics
} // vanillapdf
