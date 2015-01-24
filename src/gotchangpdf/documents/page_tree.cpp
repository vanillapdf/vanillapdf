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

		dive:
			auto kids = node->Kids();
			auto count = kids->Size();
			for (int i = 0; i < count; ++i)
			{
				types::integer under = 0;
				PageTreeNodePtr tree_node;

				auto kid = kids->At(i);
				switch (kid->GetType())
				{
				case HighLevelObject::Type::PageTreeNode:
					tree_node = dynamic_wrapper_cast<PageTreeNode>(kid);
					under = tree_node->KidCount();
					if (current + under > number)
					{
						if (HasTreeChilds(tree_node))
						{
							node = tree_node;
							goto dive;
						}

						auto result = tree_node->Kids()->At(number - current);
						return dynamic_wrapper_cast<PageObject>(result);
					}
					else
					{
						assert(current + under >= current);
						current += under;
					}

					break;
				case HighLevelObject::Type::PageObject:
					if (current == number)
						return dynamic_wrapper_cast<PageObject>(kid);
					else
						current++;
					break;
				default:
					throw new exceptions::InvalidObjectTypeException(*kid->GetObject());
				}
			}

			throw new exceptions::Exception("Page number was not found: " + number);
		}

		bool PageTree::HasTreeChilds(PageTreeNodePtr node) const
		{
			auto kids = node->Kids();
			auto count = kids->Size();;
			for (int i = 0; i < count; ++i)
			{
				if (kids->At(i)->GetType() == HighLevelObject::Type::PageTreeNode)
					return true;
			}

			return false;
		}
	}
}
