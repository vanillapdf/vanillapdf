#include "precompiled.h"

#include "semantics/objects/name_dictionary.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

NameDictionary::NameDictionary(syntax::DictionaryObjectPtr root) : HighLevelObject(root) {}

bool NameDictionary::Dests(OutputNameTreePtr<DestinationPtr>& result) const {
	if (!_obj->Contains(constant::Name::Dests)) {
		return false;
	}

	auto dict = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Dests);
	NameTreePtr<DestinationPtr> tree = make_deferred<NameTree<DestinationPtr>>(dict, &DestinationConversionFunction);

	result = tree;
	return true;
}

DestinationPtr NameDictionary::DestinationConversionFunction(const syntax::ContainableObjectPtr& obj) {
	auto destination = DestinationBase::Create(obj);
	auto raw_ptr = destination.release();
	return DestinationPtr(raw_ptr);
}

} // semantics
} // vanillapdf
