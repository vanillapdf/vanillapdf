#ifndef _PAGE_TREE_NODE_H
#define _PAGE_TREE_NODE_H

#include "fwd.h"
#include "array_object.h"
#include "integer_object.h"
#include "page_node.h"
#include "indirect_object_reference.h"
#include "dictionary_object.h"
#include "bind.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageTreeNode : public PageNode
		{
		public:
			//PageTreeNode();
			explicit PageTreeNode(ObjectReferenceWrapper<DictionaryObject> obj);

			IntegerObject::ValueType KidCount(void) const;
			ObjectReferenceWrapper<PageNode> Kid(unsigned int number) const;
			ObjectReferenceWrapper<PageNode> operator[](unsigned int number) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTreeNode; }

		private:
			Bind<DictionaryObject, IntegerObject> _count;
			Bind<DictionaryObject, ArrayObject<IndirectObjectReference>> _kids;

			ObjectReferenceWrapper<IntegerObject> GetCount(ObjectReferenceWrapper<DictionaryObject> obj);
			ObjectReferenceWrapper<ArrayObject<IndirectObjectReference>> GetKids(ObjectReferenceWrapper<DictionaryObject> obj);
		};
	}
}

#endif /* _PAGE_TREE_NODE_H */
