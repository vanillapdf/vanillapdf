#include "precompiled.h"
#include "page_tree_node.h"

#include "page_object.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "integer_object.h"
#include "semantic_exceptions.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace constant;
		using namespace syntax;

		PageTreeNode::PageTreeNode(DictionaryObjectPtr obj) :
			PageNodeBase(obj)
		{
			if (!_obj->Contains(Name::Type) || _obj->FindAs<NameObjectPtr>(Name::Type) != Name::Pages)
				throw SemanticContextExceptionFactory::Construct<syntax::DictionaryObject, PageTreeNode>(obj);
		}

		IntegerObjectPtr PageTreeNode::KidCount(void) const { return _obj->FindAs<IntegerObjectPtr>(Name::Count); }

		ArrayObjectPtr<PageNodeBasePtr> PageTreeNode::Kids() const
		{
			auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(Name::Kids);
			return kids->Convert<PageNodeBasePtr>([](const DictionaryObjectPtr& obj) { return CreatePageNode(obj); });
		}
	}
}
