#ifndef _PAGE_NODE_H
#define _PAGE_NODE_H

#include "object_reference_wrapper.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageNode : public HighLevelObject<DictionaryObject>
		{
		public:
			explicit PageNode(ObjectReferenceWrapper<DictionaryObject> obj) : HighLevelObject(obj) {}
			static ObjectReferenceWrapper<PageNode> Create(ObjectReferenceWrapper<DictionaryObject> obj);
			virtual ~PageNode() = 0;
		};
	}
}

#endif /* _PAGE_NODE_H */
