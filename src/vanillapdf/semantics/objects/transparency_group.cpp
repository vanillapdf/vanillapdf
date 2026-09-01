#include "precompiled.h"

#include "semantics/objects/transparency_group.h"
#include "semantics/utils/semantic_exceptions.h"

#include "syntax/objects/boolean_object.h"
#include "syntax/objects/name_object.h"

#include "syntax/utils/name_constants.h"

namespace vanillapdf {
namespace semantics {

TransparencyGroup::TransparencyGroup() {
    _obj->Insert(constant::Name::S, constant::Name::Transparency.Clone());
}

TransparencyGroup::TransparencyGroup(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {
    // /S is required by the specification, but files omitting it are
    // still readable - only a different group subtype is rejected
    if (!_obj->Contains(constant::Name::S)) {
        return;
    }

    if (_obj->FindAs<syntax::NameObjectPtr>(constant::Name::S) != constant::Name::Transparency) {
        throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, TransparencyGroup>(obj);
    }
}

bool TransparencyGroup::GetIsolated(bool& result) const {
    if (!_obj->Contains(constant::Name::I)) {
        return false;
    }

    auto isolated = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::I);
    result = isolated->GetValue();
    return true;
}

void TransparencyGroup::SetIsolated(bool value) {
    syntax::BooleanObjectPtr isolated = make_deferred<syntax::BooleanObject>(value);
    _obj->Insert(constant::Name::I, isolated, true);
}

bool TransparencyGroup::GetKnockout(bool& result) const {
    if (!_obj->Contains(constant::Name::K)) {
        return false;
    }

    auto knockout = _obj->FindAs<syntax::BooleanObjectPtr>(constant::Name::K);
    result = knockout->GetValue();
    return true;
}

void TransparencyGroup::SetKnockout(bool value) {
    syntax::BooleanObjectPtr knockout = make_deferred<syntax::BooleanObject>(value);
    _obj->Insert(constant::Name::K, knockout, true);
}

} // semantics
} // vanillapdf
