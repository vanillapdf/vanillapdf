#ifndef _PAGE_NODE_H
#define _PAGE_NODE_H

#include "object_reference_wrapper.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageNode : public HighLevelObject
		{
		public:
			static ObjectReferenceWrapper<PageNode> Create(const DictionaryObject& obj);
			virtual ~PageNode() = 0;
		};
	}
}

#endif /* _PAGE_NODE_H */
