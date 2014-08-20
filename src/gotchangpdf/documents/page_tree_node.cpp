#include "page_tree_node.h"

//#include "object.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageTreeNode::PageTreeNode(ObjectReferenceWrapper<DictionaryObject> obj) :
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

		ObjectReferenceWrapper<PageNode> PageTreeNode::Kid(unsigned int number) const
		{
			auto page = _kids->At(number)->GetReferencedObjectAs<DictionaryObject>();
			return PageNode::Create(page);
		}

		ObjectReferenceWrapper<PageNode> PageTreeNode::operator[](unsigned int number) const
		{
			auto page = (*_kids)[number]->GetReferencedObjectAs<DictionaryObject>();
			return PageNode::Create(page);
		}

		ObjectReferenceWrapper<IntegerObject> PageTreeNode::GetCount(ObjectReferenceWrapper<DictionaryObject> obj)
		{
			auto count = obj->FindAs<IntegerObject>(Name::Count);
			return count;
		}

		ObjectReferenceWrapper<ArrayObject<IndirectObjectReference>> PageTreeNode::GetKids(ObjectReferenceWrapper<DictionaryObject> obj)
		{
			auto kids = obj->FindAs<MixedArrayObject>(Name::Kids);
			auto specialized = kids->ToArrayType<IndirectObjectReference>();

			return specialized;
		}
	}
}
