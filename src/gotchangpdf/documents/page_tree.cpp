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
			_nodes.push_back(std::shared_ptr<PageTreeNode>(new PageTreeNode(root)));
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
