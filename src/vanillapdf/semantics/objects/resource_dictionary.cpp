#include "precompiled.h"
#include "semantics/objects/resource_dictionary.h"

namespace vanillapdf {
namespace semantics {

ResourceDictionary::ResourceDictionary(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}

bool ResourceDictionary::Font(OutputFontMapPtr& result) const {
	if (!_obj->Contains(constant::Name::Font)) {
		return false;
	}

	auto dict = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Font);
	result = make_deferred<FontMap>(dict);
	return true;
}

} // semantics
} // vanillapdf
