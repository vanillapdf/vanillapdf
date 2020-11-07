#ifndef _PAGE_NODE_H
#define _PAGE_NODE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/high_level_object.h"

namespace vanillapdf {
namespace semantics {

class PageNodeBase : public HighLevelObject<syntax::DictionaryObjectPtr> {
public:
	enum class NodeType {
		Undefined = 0,
		Tree,
		Object
	};

	explicit PageNodeBase(syntax::DictionaryObjectPtr obj);
	PageNodeBasePtr GetPageRoot();

	bool HasParent(void) const;
	PageNodeBasePtr GetParent(void) const;
	void SetParent(PageNodeBasePtr parent);

	virtual NodeType GetNodeType(void) const noexcept = 0;

	static PageNodeBasePtr CreatePageNode(syntax::DictionaryObjectPtr obj);
};

} // semantics
} // vanillapdf

#endif /* _PAGE_NODE_H */
