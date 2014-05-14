#include "page_tree.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "exception.h"
#include "gotchangpdf.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageNode::~PageNode() {}

		//PageTree::PageTree() {}

		PageTree::PageTree(const IndirectObject& root)
		{
			auto dict = root.GetObjectAs<DictionaryObject>();
			auto type = dict->FindAs<NameObject>(Name::Type);

			if (*type == Name::Pages)
				_root = ObjectReferenceWrapper<PageTreeNode>(new PageTreeNode(root));
			//else if (*type == Name::Page)
			//	_root = ObjectReferenceWrapper<PageObject>(new PageObject(root));
			else
				throw Exception("Cannot initialize PageTree from TODO");
		}

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
			_kids = dict->FindAs<ArrayObject>(Name::Kids);
		}
	}
}

GOTCHANG_PDF_API PageTreeNodeHandle CALLING_CONVENTION PageTree_GetRoot(PageTreeHandle handle)
{
	gotchangpdf::documents::PageTree* obj = reinterpret_cast<gotchangpdf::documents::PageTree*>(handle);
	auto root = obj->GetRoot();
	return reinterpret_cast<PageTreeNodeHandle>(root.AddRefGet());
}

GOTCHANG_PDF_API int CALLING_CONVENTION PageTreeNode_GetCount(PageTreeNodeHandle handle)
{
	gotchangpdf::documents::PageTreeNode* obj = reinterpret_cast<gotchangpdf::documents::PageTreeNode*>(handle);
	return *obj->Count();
}

GOTCHANG_PDF_API ArrayObjectHandle CALLING_CONVENTION PageTreeNode_GetKids(PageTreeNodeHandle handle)
{
	gotchangpdf::documents::PageTreeNode* obj = reinterpret_cast<gotchangpdf::documents::PageTreeNode*>(handle);
	auto kids = obj->Kids();
	return reinterpret_cast<ArrayObjectHandle>(kids.AddRefGet());
}
