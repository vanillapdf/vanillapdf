#include "page_tree_node.h"

#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageTreeNode::PageTreeNode(DictionaryObjectPtr obj) :
			PageNode(obj),
			_count(Bind<DictionaryObjectPtr, IntegerObjectPtr>(_obj, std::bind(&PageTreeNode::GetCount, this, _obj))),
			_kids(Bind<DictionaryObjectPtr, Deferred<ArrayObject<IndirectObjectReferencePtr>>>(_obj, std::bind(&PageTreeNode::GetKids, this, _obj)))
		{
			if (*_obj->FindAs<NameObjectPtr>(Name::Type) != Name::Pages)
				throw Exception("TODO");
		}

		types::integer PageTreeNode::KidCount(void) const
		{
			return _count->Value();
		}

		Deferred<ArrayObject<PageNodePtr>> PageTreeNode::Kids() const
		{
			return _kids->Convert<PageNodePtr>(
				[](IndirectObjectReferencePtr& obj)
			{
				auto page = obj->GetReferencedObjectAs<DictionaryObjectPtr>();
				return PageNode::Create(page);
			});
		}

		IntegerObjectPtr PageTreeNode::GetCount(DictionaryObjectPtr obj)
		{
			auto count = obj->FindAs<IntegerObjectPtr>(Name::Count);
			return count;
		}

		Deferred<ArrayObject<IndirectObjectReferencePtr>> PageTreeNode::GetKids(DictionaryObjectPtr obj)
		{
			auto kids = obj->FindAs<MixedArrayObjectPtr>(Name::Kids);
			auto specialized = kids->CastToArrayType<IndirectObjectReferencePtr>();

			return specialized;
		}
	}
}
