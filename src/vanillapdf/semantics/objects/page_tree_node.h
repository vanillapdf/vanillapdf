#ifndef _PAGE_TREE_NODE_H
#define _PAGE_TREE_NODE_H

#include "semantics/utils/semantics_fwd.h"
#include "semantics/objects/page_node_base.h"

namespace vanillapdf {
namespace semantics {

class PageTreeNode : public PageNodeBase {
public:
	explicit PageTreeNode(syntax::DictionaryObjectPtr obj);

	syntax::IntegerObjectPtr KidCount(void) const;
	syntax::ArrayObjectPtr<PageNodeBasePtr> Kids(void) const;

	virtual NodeType GetNodeType(void) const noexcept override { return NodeType::Tree; }

private:
	static PageNodeBasePtr ConvertFunction(syntax::DictionaryObjectPtr obj);
};

} // semantics
} // vanillapdf

#endif /* _PAGE_TREE_NODE_H */
