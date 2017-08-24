#include "precompiled.h"

#include "semantics/objects/page_node_base.h"
#include "semantics/objects/page_tree_node.h"
#include "semantics/objects/page_object.h"

#include "semantics/utils/semantic_exceptions.h"

namespace gotchangpdf {
namespace semantics {

PageNodeBase::PageNodeBase(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {
}

PageNodeBasePtr PageNodeBase::GetPageRoot() {
	PageNodeBasePtr result = PageNodeBasePtr(this);
	while (result->HasParent()) {
		result = result->GetParent();
	}

	return result;
}

bool PageNodeBase::HasParent(void) const {
	return _obj->Contains(constant::Name::Parent);
}

PageNodeBasePtr PageNodeBase::GetParent() const {
	auto parent = _obj->FindAs<syntax::DictionaryObjectPtr>(constant::Name::Parent);
	return make_deferred<PageTreeNode>(parent);
}

void PageNodeBase::SetParent(PageNodeBasePtr parent) {
	if (_obj->Contains(constant::Name::Parent)) {
		bool removed = _obj->Remove(constant::Name::Parent);
		assert(removed && "Unable to remove existing item"); UNUSED(removed);
	}

	syntax::IndirectObjectReferencePtr parent_ref = make_deferred<syntax::IndirectObjectReference>(parent->GetObject());
	_obj->Insert(constant::Name::Parent, parent_ref);
}

PageNodeBasePtr PageNodeBase::CreatePageNode(syntax::DictionaryObjectPtr obj) {
	if (!obj->Contains(constant::Name::Type))
		throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageNodeBase>(obj);

	auto type = obj->FindAs<syntax::NameObjectPtr>(constant::Name::Type);

	if (type == constant::Name::Pages) {
		auto result = make_deferred<PageTreeNode>(obj);
		return result;
	}

	if (type == constant::Name::Page) {
		auto result = make_deferred<PageObject>(obj);
		return result;
	}

	throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageNodeBase>(obj);
}

} // semantics
} // gotchangpdf
