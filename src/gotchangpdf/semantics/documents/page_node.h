#ifndef _PAGE_NODE_H
#define _PAGE_NODE_H

#include "semantics_fwd.h"
#include "high_level_object.h"

namespace gotchangpdf
{
	namespace semantics
	{
		class PageNodeBase : public HighLevelObject<syntax::DictionaryObjectPtr>
		{
		public:
			enum class NodeType
			{
				Tree,
				Object
			};

			explicit PageNodeBase(syntax::DictionaryObjectPtr obj) : HighLevelObject(obj) {}

			virtual NodeType GetNodeType(void) const noexcept = 0;
			virtual ~PageNodeBase() = 0;
		};

		inline PageNodeBase::~PageNodeBase() {}
	}
}

#endif /* _PAGE_NODE_H */
