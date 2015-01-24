#include "precompiled.h"
#include "page_tree_node.h"

#include "dictionary_object.h"
#include "name_object.h"
#include "integer_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageTreeNode::PageTreeNode(DictionaryObjectPtr obj) :
			PageNode(obj)
		{
			if (*_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Pages)
				throw Exception("TODO");
		}

		types::integer PageTreeNode::KidCount(void) const
		{
			return _obj->FindAs<IntegerObjectPtr>(Name::Count)->Value();
		}

		ArrayObjectPtr<PageNodePtr> PageTreeNode::Kids() const
		{
			auto kids = _obj->FindAs<MixedArrayObjectPtr>(Name::Kids);
			auto specialized = kids->CastToArrayType<IndirectObjectReferencePtr>();

			return specialized->Convert<PageNodePtr>(
				[](IndirectObjectReferencePtr& obj)
			{
				auto page = obj->GetReferencedObjectAs<DictionaryObjectPtr>();
				return PageNode::Create(page);
			});
		}
	}
}
