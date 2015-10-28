#include "precompiled.h"
#include "page_tree.h"

#include "array_object.h"
#include "invalid_object_type_exception.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;

		//PageTree::PageTree() {}

		PageTree::PageTree(DictionaryObjectPtr root) : HighLevelObject(root), _root(new PageTreeNode(root)) {}

		PageObjectPtr PageTree::PageInternal(types::integer number) const
		{
			auto node = _root;
			types::integer current = 1;

			PageNodeBaseVisitor base_visitor;
			PageNodeVisitor<PageTreeNodePtr> tree_visitor;
			PageNodeVisitor<PageObjectPtr> object_visitor;

		dive:
			auto kids = node->Kids();
			auto count = kids->Size();
			for (int i = 0; i < count; ++i)
			{
				types::integer under = 0;
				PageTreeNodePtr tree_node;

				auto kid = kids->At(i);
				auto kid_base = kid.apply_visitor(base_visitor);

				switch (kid_base->GetType())
				{
				case HighLevelObject::Type::PageTreeNode:
					tree_node = kid.apply_visitor(tree_visitor);
					under = tree_node->KidCount();
					if (current + under > number)
					{
						if (HasTreeChilds(tree_node))
						{
							node = tree_node;
							goto dive;
						}

						auto result = tree_node->Kids()->At(number - current);
						return result.apply_visitor(object_visitor);
					}
					else
					{
						assert(current + under >= current);
						current += under;
					}

					break;
				case HighLevelObject::Type::PageObject:
					if (current == number)
						return kid.apply_visitor(object_visitor);
					else
						current++;
					break;
				default:
					throw new exceptions::InvalidObjectTypeException(*kid_base->GetObject());
				}
			}

			throw new exceptions::Exception("Page number was not found: " + number);
		}

		bool PageTree::HasTreeChilds(PageTreeNodePtr node) const
		{
			PageNodeBaseVisitor base_visitor;
			auto kids = node->Kids();
			auto count = kids->Size();;
			for (int i = 0; i < count; ++i)
			{
				auto kid = kids->At(i);
				auto kid_base = kid.apply_visitor(base_visitor);
				if (kid_base->GetType() == HighLevelObject::Type::PageTreeNode)
					return true;
			}

			return false;
		}
	}
}
