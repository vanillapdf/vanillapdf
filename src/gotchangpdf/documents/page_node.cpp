#include "page_node.h"

#include "exception.h"
#include "name_object.h"
#include "dictionary_object.h"
#include "page_tree_node.h"
#include "page_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageNode::~PageNode() {}

		ObjectReferenceWrapper<PageNode> PageNode::Create(ObjectReferenceWrapper<DictionaryObject> obj)
		{
			auto type = obj->FindAs<NameObject>(Name::Type);

			if (*type == Name::Pages)
				return ObjectReferenceWrapper<PageTreeNode>(new PageTreeNode(obj));
			else if (*type == Name::Page)
				return ObjectReferenceWrapper<PageObject>(new PageObject(obj));
			else
				throw Exception("Cannot initialize PageTree from TODO");
		}
	}
}

