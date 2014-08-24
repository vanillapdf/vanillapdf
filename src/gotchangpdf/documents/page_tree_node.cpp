#include "page_tree_node.h"

//#include "object.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageTreeNode::PageTreeNode(SmartPtr<DictionaryObject> obj) :
			PageNode(obj),
			_count(Bind<DictionaryObject, IntegerObject>(_obj, std::bind(&PageTreeNode::GetCount, this, _obj))),
			_kids(Bind<DictionaryObject, ArrayObject<IndirectObjectReference>>(_obj, std::bind(&PageTreeNode::GetKids, this, _obj)))
		{
			if (*_obj->FindAs<NameObject>(Name::Type) != Name::Pages)
				throw Exception("TODO");
		}

		IntegerObject::ValueType PageTreeNode::KidCount(void) const
		{
			return _count->Value();
		}

		SmartPtr<ArrayObject<PageNode>> PageTreeNode::Kids() const
		{
			return _kids->Convert<PageNode>(
				[](SmartPtr<IndirectObjectReference>& obj)->SmartPtr<PageNode>
			{
				auto page = obj->GetReferencedObjectAs<DictionaryObject>();
				return PageNode::Create(page);
			});
		}

		SmartPtr<IntegerObject> PageTreeNode::GetCount(SmartPtr<DictionaryObject> obj)
		{
			auto count = obj->FindAs<IntegerObject>(Name::Count);
			return count;
		}

		SmartPtr<ArrayObject<IndirectObjectReference>> PageTreeNode::GetKids(SmartPtr<DictionaryObject> obj)
		{
			auto kids = obj->FindAs<MixedArrayObject>(Name::Kids);
			auto specialized = kids->CastToArrayType<IndirectObjectReference>();

			return specialized;
		}
	}
}
