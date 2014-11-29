#include "page_tree.h"

#include "invalid_object_type_exception.h"

#include "c_page_tree.h"

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
			IntegerObject::value_type current = 1;

		dive:
			auto kids = node->Kids();
			auto count = kids->Size();
			for (int i = 0; i < count; ++i)
			{
				IntegerObject::value_type under = 0;

				auto kid = kids->At(i);
				switch (kid->GetType())
				{
				case HighLevelObject::Type::PageTreeNode:
					under = node->KidCount();
					if (current + under >= number)
					{
						auto tree_node = dynamic_wrapper_cast<PageTreeNode>(kid);
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
					throw new exceptions::InvalidObjectTypeException(*kid->Get());
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

GOTCHANG_PDF_API long long CALLING_CONVENTION PageTree_GetPageCount(PageTreeHandle handle)
{
	gotchangpdf::documents::PageTree* obj = reinterpret_cast<gotchangpdf::documents::PageTree*>(handle);
	return obj->PageCount();
}

GOTCHANG_PDF_API PageObjectHandle CALLING_CONVENTION PageTree_GetPage(PageTreeHandle handle, int at)
{
	gotchangpdf::documents::PageTree* obj = reinterpret_cast<gotchangpdf::documents::PageTree*>(handle);
	auto page = obj->Page(at);
	return reinterpret_cast<PageObjectHandle>(page.AddRefGet());
}

GOTCHANG_PDF_API void CALLING_CONVENTION PageTree_Release(PageTreeHandle handle)
{
	gotchangpdf::documents::PageTree* obj = reinterpret_cast<gotchangpdf::documents::PageTree*>(handle);
	obj->Release();
}
