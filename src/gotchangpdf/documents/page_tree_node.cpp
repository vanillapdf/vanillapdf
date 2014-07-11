#include "page_tree_node.h"

//#include "object.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		using namespace constant;
		using namespace exceptions;

		PageTreeNode::PageTreeNode(ObjectReferenceWrapper<DictionaryObject> obj) : PageNode(obj)
		{
			if (*_obj->FindAs<NameObject>(Name::Type) != Name::Pages)
				throw Exception("TODO");

			_count = _obj->FindAs<IntegerObject>(Name::Count);
			auto arr = _obj->FindAs<ArrayObject>(Name::Kids);
			_kids = ObjectReferenceWrapper<SpecializedArrayObject<IndirectObjectReference>>(new SpecializedArrayObject<IndirectObjectReference>(arr));
		}

		IntegerObject::ValueType PageTreeNode::KidCount(void) const
		{
			if (_count->GetContainer() != _obj)
				_count = _obj->FindAs<IntegerObject>(Name::Count);

			return _count->Value();
		}

		ObjectReferenceWrapper<PageNode> PageTreeNode::Kid(unsigned int number) const
		{
			if (_kids->GetContainer() != _obj)
			{
				auto arr = _obj->FindAs<ArrayObject>(Name::Kids);
				_kids = ObjectReferenceWrapper<SpecializedArrayObject<IndirectObjectReference>>(new SpecializedArrayObject<IndirectObjectReference>(arr));
			}

			return PageNode::Create(_kids->At(number)->GetReferencedObjectAs<DictionaryObject>());
		}

		ObjectReferenceWrapper<PageNode> PageTreeNode::operator[](unsigned int number) const
		{
			if (_kids->GetContainer() != _obj)
			{
				auto arr = _obj->FindAs<ArrayObject>(Name::Kids);
				_kids = ObjectReferenceWrapper<SpecializedArrayObject<IndirectObjectReference>>(new SpecializedArrayObject<IndirectObjectReference>(arr));
			}

			return PageNode::Create((*_kids)[number]->GetReferencedObjectAs<DictionaryObject>());
		}
	}
}
