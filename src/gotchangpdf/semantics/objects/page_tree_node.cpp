#include "precompiled.h"
#include "semantics/objects/page_tree_node.h"

#include "semantics/objects/page_object.h"
#include "syntax/objects/dictionary_object.h"
#include "syntax/objects/name_object.h"
#include "syntax/objects/integer_object.h"
#include "semantics/utils/semantic_exceptions.h"

namespace gotchangpdf {
namespace semantics {

using namespace constant;
using namespace syntax;

PageTreeNode::PageTreeNode(DictionaryObjectPtr obj) :
	PageNodeBase(obj) {
	if (!_obj->Contains(Name::Type) || _obj->FindAs<NameObjectPtr>(Name::Type) != Name::Pages)
		throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageTreeNode>(obj);
}

IntegerObjectPtr PageTreeNode::KidCount(void) const {
	return _obj->FindAs<IntegerObjectPtr>(Name::Count);
}

ArrayObjectPtr<PageNodeBasePtr> PageTreeNode::Kids() const {
	auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(Name::Kids);
	return kids->Convert<PageNodeBasePtr>([this](const DictionaryObjectPtr& obj) { return CreatePageNode(obj); });
}

} // semantics
} // gotchangpdf
