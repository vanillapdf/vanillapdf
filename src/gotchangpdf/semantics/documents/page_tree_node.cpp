#include "precompiled.h"
#include "page_tree_node.h"

#include "page_object.h"
#include "dictionary_object.h"
#include "name_object.h"
#include "integer_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		using namespace constant;
		using namespace syntax;

		PageTreeNode::PageTreeNode(DictionaryObjectPtr obj) :
			PageNodeBase(obj)
		{
			if (*_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Pages)
				throw Exception("TODO");
		}

		types::integer PageTreeNode::KidCount(void) const
		{
			return _obj->FindAs<IntegerObjectPtr>(Name::Count)->Value();
		}

		ArrayObjectPtr<PageNodePtr> PageTreeNode::Kids() const
		{
			auto kids = _obj->FindAs<ArrayObjectPtr<DictionaryObjectPtr>>(Name::Kids);

			return kids->Convert<PageNodePtr>(
				[] (DictionaryObjectPtr& obj)
			{
				return CreatePageNode(obj);
			});
		}
	}
}
