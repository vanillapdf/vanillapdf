#ifndef _PAGE_H
#define _PAGE_H

#include "semantics/utils/semantics_fwd.h"
#include "page_node.h"
#include "semantics/objects/rectangle.h"
#include "semantics/objects/resource_dictionary.h"
#include "semantics/objects/contents.h"
#include "semantics/objects/annotations.h"

namespace gotchangpdf {
namespace semantics {

class PageObject : public PageNodeBase {
public:
	explicit PageObject(syntax::DictionaryObjectPtr obj);

	static std::unique_ptr<PageObject> Create(DocumentPtr document);
	static std::unique_ptr<PageObject> Create(syntax::DictionaryObjectPtr obj);

	PageTreeNodePtr GetParent(void) const;
	void SetParent(PageTreeNodePtr parent);

	ResourceDictionaryPtr GetResources(void) const;
	void SetResources(ResourceDictionaryPtr resources);

	RectanglePtr GetMediaBox(void) const;
	void SetMediaBox(RectanglePtr);

	bool GetAnnotations(OutputPageAnnotationsPtr& result) const;
	void SetAnnotations(PageAnnotationsPtr annots);

	bool GetContents(OutputContentsPtr& result) const;
	void SetContents(ContentsPtr contents);

	virtual NodeType GetNodeType(void) const noexcept override { return NodeType::Object; }

private:
	mutable OutputContentsPtr m_contents;
};

} // semantics
} // gotchangpdf

#endif /* _PAGE_H */
