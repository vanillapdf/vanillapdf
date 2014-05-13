#include "page_tree.h"
#include "indirect_object.h"
#include "dictionary_object.h"
#include "exception.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageNode::~PageNode() {}

		PageTree::PageTree() : _nodes() {}

		PageTree::PageTree(const IndirectObject& root) : _nodes()
		{
			auto dict = root.GetObject().GetAs<DictionaryObject>();
			auto type = dict->Find<NameObject>(Name::Type);

			if (*type == Name::Pages)
				_nodes.push_back(std::shared_ptr<PageTreeNode>(new PageTreeNode(root)));
			else if (*type == Name::Page)
				_nodes.push_back(std::shared_ptr<PageObject>(new PageObject(root)));
			else
				throw Exception("Cannot initialize PageTree from TODO");
		}

		PageObject::PageObject() {}

		PageObject::PageObject(const IndirectObject& obj)
		{
			auto dict = obj.GetObject().GetAs<DictionaryObject>();

			if (*dict->Find<NameObject>(Name::Type) != Name::Page)
				throw Exception("TODO");

			_parent = dict->Find<IndirectObjectReference>(Name::Parent);

			//TODO rectangle
			_media_box = dict->Find<Rectangle>(Name::MediaBox);

			_resources = dict->Find<DictionaryObject>(Name::Resources);
		}

		PageTreeNode::PageTreeNode() : _count(ObjectReferenceWrapper<IntegerObject>(new IntegerObject())), _kids(ObjectReferenceWrapper<ArrayObject>(new ArrayObject())) {}

		PageTreeNode::PageTreeNode(const IndirectObject& obj)
		{
			auto dict = obj.GetObject().GetAs<DictionaryObject>();

			if (*dict->Find<NameObject>(Name::Type) != Name::Pages)
				throw Exception("TODO");

			_count = dict->Find<IntegerObject>(Name::Count);
			_kids = dict->Find<ArrayObject>(Name::Kids);
		}
	}
}
