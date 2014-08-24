#ifndef _PAGE_NODE_H
#define _PAGE_NODE_H

#include "smart_ptr.h"
#include "high_level_object.h"
#include "dictionary_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageNode : public HighLevelObject<DictionaryObject>
		{
		public:
			explicit PageNode(SmartPtr<DictionaryObject> obj) : HighLevelObject(obj) {}
			static SmartPtr<PageNode> Create(SmartPtr<DictionaryObject> obj);
			virtual ~PageNode() = 0;
		};
	}
}

#endif /* _PAGE_NODE_H */
