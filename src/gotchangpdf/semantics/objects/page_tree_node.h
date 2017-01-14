#ifndef _PAGE_TREE_NODE_H
#define _PAGE_TREE_NODE_H

#include "semantics/utils/semantics_fwd.h"
#include "page_node.h"
#include "page_node_utils.h"

namespace gotchangpdf {
namespace semantics {

class PageTreeNode : public PageNodeBase {
public:
	explicit PageTreeNode(syntax::DictionaryObjectPtr obj);

	syntax::IntegerObjectPtr KidCount(void) const;
	syntax::ArrayObjectPtr<PageNodeBasePtr> Kids(void) const;

	virtual NodeType GetNodeType(void) const noexcept override { return NodeType::Tree; }
};

} // semantics
} // gotchangpdf

#endif /* _PAGE_TREE_NODE_H */
