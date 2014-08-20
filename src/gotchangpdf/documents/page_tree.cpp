#include "page_tree.h"

#include "c_page_tree.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		//PageTree::PageTree() {}

		PageTree::PageTree(ObjectReferenceWrapper<DictionaryObject> root) : HighLevelObject(root), _root(new PageTreeNode(root)) {}

		ObjectReferenceWrapper<PageObject> PageTree::PageInternal(unsigned int number) const
		{
			//int count = 0, result = 0;
			//ObjectReferenceWrapper<PageTreeNode> tree_node;
			return dynamic_wrapper_cast<PageObject>(_root->Kid(0));
		}

		/*
		IntegerObject::ValueType PageTree::PageCountInternal(ObjectReferenceWrapper<PageNode> node) const
		{
			int count = 0, result = 0;
			ObjectReferenceWrapper<PageTreeNode> tree_node;

			auto type = node->GetType();
			switch (type)
			{
			case PageNode::Type::PAGE_TREE_NODE:
				tree_node = dynamic_wrapper_cast<PageTreeNode>(node);
				count = tree_node->KidCount();
				for (unsigned int i = 0; i < count; ++i)
					result += PageCountInternal(tree_node->Kid(i));
				break;
			case PageNode::Type::PAGE_OBJECT_NODE:
				break;
			default:
				assert(false);
				throw Exception("FIXME: Unknown PageNodeType");
			}

			return result;
		}
		*/
		//PageObject::PageObject() {}

		//PageTreeNode::PageTreeNode() : _count(ObjectReferenceWrapper<IntegerObject>(new IntegerObject())), _kids(ObjectReferenceWrapper<MixedArrayObject>(new MixedArrayObject())) {}


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
