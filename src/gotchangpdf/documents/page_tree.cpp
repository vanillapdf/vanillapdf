#include "page_tree.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "exception.h"

#include "c_page_tree.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageNode::~PageNode() {}

		void PageNode::Release() { boost::intrusive_ptr_release(this); }

		ObjectReferenceWrapper<PageNode> PageNode::Create(const IndirectObject& obj)
		{
			auto dict = obj.GetObjectAs<DictionaryObject>();
			auto type = dict->FindAs<NameObject>(Name::Type);

			if (*type == Name::Pages)
				return ObjectReferenceWrapper<PageTreeNode>(new PageTreeNode(obj));
			else if (*type == Name::Page)
				return ObjectReferenceWrapper<PageObject>(new PageObject(obj));
			else
				throw Exception("Cannot initialize PageTree from TODO");
		}

		//PageTree::PageTree() {}

		PageTree::PageTree(const IndirectObject& root) : _root(dynamic_wrapper_cast<PageTreeNode>(PageNode::Create(root))) {}

		void PageTree::Release() { boost::intrusive_ptr_release(this); }

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

		PageObject::PageObject(const IndirectObject& obj)
		{
			auto dict = obj.GetObjectAs<DictionaryObject>();

			if (*dict->FindAs<NameObject>(Name::Type) != Name::Page)
				throw Exception("TODO");

			_parent = dict->FindAs<IndirectObjectReference>(Name::Parent);

			//TODO rectangle
			_media_box = dict->FindAs<Rectangle>(Name::MediaBox);

			_resources = dict->FindAs<DictionaryObject>(Name::Resources);
		}

		//PageTreeNode::PageTreeNode() : _count(ObjectReferenceWrapper<IntegerObject>(new IntegerObject())), _kids(ObjectReferenceWrapper<ArrayObject>(new ArrayObject())) {}

		PageTreeNode::PageTreeNode(const IndirectObject& obj)
		{
			auto dict = obj.GetObjectAs<DictionaryObject>();

			if (*dict->FindAs<NameObject>(Name::Type) != Name::Pages)
				throw Exception("TODO");

			_count = dict->FindAs<IntegerObject>(Name::Count);
			_kids = dict->FindAs<ArrayObject<IndirectObjectReference>>(Name::Kids);
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

GOTCHANG_PDF_API void CALLING_CONVENTION PageObject_Release(PageObjectHandle handle)
{
	gotchangpdf::documents::PageObject* obj = reinterpret_cast<gotchangpdf::documents::PageObject*>(handle);
	obj->Release();
}
