#include "page_tree_node.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageTreeNode::PageTreeNode(const DictionaryObject& obj) : _dictionary(new DictionaryObject(obj))
		{
			if (*_dictionary->FindAs<NameObject>(Name::Type) != Name::Pages)
				throw Exception("TODO");

			_count = _dictionary->FindAs<IntegerObject>(Name::Count);
			auto arr = _dictionary->FindAs<ArrayObject>(Name::Kids);
			_kids = ObjectReferenceWrapper<SpecializedArrayObject<IndirectObjectReference>>(new SpecializedArrayObject<IndirectObjectReference>(*arr));
		}
	}
}
