#ifndef _PAGE_TREE_NODE_H
#define _PAGE_TREE_NODE_H

#include "fwd.h"
#include "constants.h"
#include "array_object.h"
#include "page_node.h"
#include "indirect_object_reference.h"
#include "dictionary_object.h"
#include "integer_object.h"
#include "bind.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageTreeNode : public PageNode
		{
		public:
			explicit PageTreeNode(DictionaryObjectPtr obj);

			types::integer KidCount(void) const;
			Deferred<ArrayObject<PageNodePtr>> Kids(void) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTreeNode; }

		private:
			Bind<DictionaryObjectPtr, IntegerObjectPtr> _count;
			Bind<DictionaryObjectPtr, Deferred<ArrayObject<IndirectObjectReferencePtr>>> _kids;

			IntegerObjectPtr GetCount(DictionaryObjectPtr obj);
			Deferred<ArrayObject<IndirectObjectReferencePtr>> GetKids(DictionaryObjectPtr obj);
		};

		typedef SmartPtr<PageTreeNode> PageTreeNodePtr;
	}
}

#endif /* _PAGE_TREE_NODE_H */
