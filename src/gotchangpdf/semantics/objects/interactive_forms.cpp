#include "precompiled.h"

#include "semantics/objects/interactive_forms.h"

namespace gotchangpdf {
namespace semantics {

InteractiveForm::InteractiveForm(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

syntax::ArrayObjectPtr<FieldPtr> InteractiveForm::Fields(void) const {
	auto fields = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Fields);
	return fields->Convert<FieldPtr>([](const syntax::DictionaryObjectPtr& obj) {
		auto field_ptr = Field::Create(obj);
		auto raw_ptr = field_ptr.release();
		return FieldPtr(raw_ptr);
	});
}

} // semantics
} // gotchangpdf
