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
    NameTreePtr<DestinationPtr> tree = make_deferred_container<NameTree<DestinationPtr>>(dict, &DestinationConversionFunction);

    result = tree;
    return true;
}

DestinationPtr NameDictionary::DestinationConversionFunction(const syntax::ContainableObjectPtr& obj) {
    return DestinationBase::CreateFromObject(obj);
}

} // semantics
} // vanillapdf
