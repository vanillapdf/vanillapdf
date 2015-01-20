#ifndef _PAGE_NODE_H
#define _PAGE_NODE_H

#include "fwd.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageNode : public HighLevelObject<DictionaryObjectPtr>
		{
		public:
			explicit PageNode(DictionaryObjectPtr obj) : HighLevelObject(obj) {}
			static PageNodePtr Create(DictionaryObjectPtr obj);
			virtual ~PageNode() = 0;
		};
	}
}

#endif /* _PAGE_NODE_H */
