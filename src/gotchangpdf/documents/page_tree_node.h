#ifndef _PAGE_TREE_NODE_H
#define _PAGE_TREE_NODE_H

#include "fwd.h"
#include "constants.h"
#include "array_object.h"
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
			explicit PageTreeNode(SmartPtr<DictionaryObject> obj);

			types::integer KidCount(void) const;
			SmartPtr<ArrayObject<PageNodePtr>> Kids(void) const;

			virtual inline HighLevelObject::Type GetType() const override { return HighLevelObject::Type::PageTreeNode; }

		private:
			Bind<DictionaryObject, IntegerObject> _count;
			Bind<DictionaryObject, ArrayObject<IndirectObjectReferencePtr>> _kids;

			SmartPtr<IntegerObject> GetCount(DictionaryObjectPtr obj);
			SmartPtr<ArrayObject<IndirectObjectReferencePtr>> GetKids(DictionaryObjectPtr obj);
		};
	}
}

#endif /* _PAGE_TREE_NODE_H */
