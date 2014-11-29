#ifndef _PAGE_NODE_H
#define _PAGE_NODE_H

#include "high_level_object.h"
#include "dictionary_object.h"
#include "smart_ptr.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageNode;
		typedef SmartPtr<PageNode> PageNodePtr;

		class PageNode : public HighLevelObject<DictionaryObject>
		{
		public:
			explicit PageNode(DictionaryObjectPtr obj) : HighLevelObject(obj) {}
			static PageNodePtr Create(DictionaryObjectPtr obj);
			virtual ~PageNode() = 0;
		};
	}
}

#endif /* _PAGE_NODE_H */
