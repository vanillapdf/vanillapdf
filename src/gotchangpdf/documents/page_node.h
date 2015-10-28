#ifndef _PAGE_NODE_H
#define _PAGE_NODE_H

#include "fwd.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace documents
	{
		class PageNodeBase : public HighLevelObject<DictionaryObjectPtr>
		{
		public:
			explicit PageNodeBase(DictionaryObjectPtr obj) : HighLevelObject(obj) {}
			virtual ~PageNodeBase() = 0;
		};
	}
}

#endif /* _PAGE_NODE_H */
