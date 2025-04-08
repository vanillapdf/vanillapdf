#include "precompiled.h"

#include "semantics/objects/tree.h"

namespace vanillapdf {
namespace semantics {

#pragma region Tree nodes

#pragma region Tree node Base

TreeNodeBase::TreeNodeBase(
    const IValueNameProvider* parent,
    syntax::DictionaryObjectPtr obj)
    : HighLevelObject(obj), _parent(parent) {
}

TreeNodeBasePtr TreeNodeBase::Create(
    const IValueNameProvider* parent,
    syntax::DictionaryObjectPtr obj) {
    bool kids = obj->Contains(constant::Name::Kids);
    bool limits = obj->Contains(constant::Name::Limits);
    bool names = obj->Contains(constant::Name::Names);

    if (kids && !limits && !names) {
        return make_deferred<TreeNodeRoot>(parent, obj);
    }

    if (kids && limits && !names) {
        return make_deferred<TreeNodeIntermediate>(parent, obj);
    }

    if (!kids && limits && names) {
        return make_deferred<TreeNodeLeaf>(parent, obj);
    }

    throw GeneralException("Unknown tree node: " + obj->ToString());
}

#pragma endregion

#pragma region Tree node root

TreeNodeRoot::TreeNodeRoot(
    const IValueNameProvider* parent,
    syntax::DictionaryObjectPtr obj)
    : TreeNodeBase(parent, obj) {
}

TreeNodeRoot::TreeNodeType TreeNodeRoot::NodeType(void) const noexcept {
    return TreeNodeType::Root;
}

bool TreeNodeRoot::HasValues(void) const {
    return _obj->Contains(_parent->GetValueName());
}

bool TreeNodeRoot::HasKids(void) const {
    return _obj->Contains(constant::Name::Kids);
}

syntax::ArrayObjectPtr<TreeNodeBasePtr> TreeNodeRoot::Kids(void) const {
    auto kids = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Kids);
    return kids->Convert<TreeNodeBasePtr>(
        [this](const syntax::DictionaryObjectPtr& obj) {
        return TreeNodeBase::Create(_parent, obj);
    });
}

syntax::MixedArrayObjectPtr TreeNodeRoot::Values(void) const {
    return _obj->FindAs<syntax::MixedArrayObjectPtr>(_parent->GetValueName());
}

#pragma endregion

#pragma region Tree node intermediate

TreeNodeIntermediate::TreeNodeIntermediate(
    const IValueNameProvider* parent,
    const syntax::DictionaryObjectPtr& obj)
    : TreeNodeBase(parent, obj) {
}

TreeNodeIntermediate::TreeNodeType TreeNodeIntermediate::NodeType(void) const noexcept {
    return TreeNodeType::Intermediate;
}

syntax::ArrayObjectPtr<Deferred<TreeNodeBase>> TreeNodeIntermediate::Kids(void) const {
    auto kids = _obj->FindAs<syntax::ArrayObjectPtr<syntax::DictionaryObjectPtr>>(constant::Name::Kids);
    return kids->Convert<Deferred<TreeNodeBase>>(
        [this](const syntax::DictionaryObjectPtr& obj) {
        return TreeNodeBase::Create(_parent, obj);
    });
}

syntax::MixedArrayObjectPtr TreeNodeIntermediate::Limits(void) const {
    return _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Limits);
}

#pragma endregion

#pragma region Tree node leaf

TreeNodeLeaf::TreeNodeLeaf(
    const IValueNameProvider* parent,
    const syntax::DictionaryObjectPtr& obj)
    : TreeNodeBase(parent, obj) {
}

TreeNodeLeaf::TreeNodeType TreeNodeLeaf::NodeType(void) const noexcept {
    return TreeNodeType::Leaf;
}

syntax::MixedArrayObjectPtr TreeNodeLeaf::Limits(void) const {
    return _obj->FindAs<syntax::MixedArrayObjectPtr>(constant::Name::Limits);
}

syntax::MixedArrayObjectPtr TreeNodeLeaf::Values(void) const {
    return _obj->FindAs<syntax::MixedArrayObjectPtr>(_parent->GetValueName());
}

#pragma endregion

#pragma endregion

} // semantics
} // vanillapdf
