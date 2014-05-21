#ifndef _PAGE_TREE_NODE_H
#define _PAGE_TREE_NODE_H

#include "fwd.h"
#include "integer_object.h"
#include "specialized_array.h"
#include "page_node.h"
#include "indirect_object_reference.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageTreeNode : public PageNode
		{
		public:
			//PageTreeNode();
			explicit PageTreeNode(const DictionaryObject& obj);

			inline IntegerObject::ValueType KidCount(void) const { return _count->Value(); }
			inline ObjectReferenceWrapper<PageNode> Kid(unsigned int number) const { return PageNode::Create(*_kids->At(number)->GetReferencedObjectAs<DictionaryObject>()); }
			inline ObjectReferenceWrapper<PageNode> operator[](unsigned int number) const { return PageNode::Create(*(*_kids)[number]->GetReferencedObjectAs<DictionaryObject>()); }

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTreeNode; }

		private:
			ObjectReferenceWrapper<DictionaryObject> _dictionary;

			ObjectReferenceWrapper<IntegerObject> _count;
			ObjectReferenceWrapper<SpecializedArrayObject<IndirectObjectReference>> _kids;
		};
	}
}

#endif /* _PAGE_TREE_NODE_H */
